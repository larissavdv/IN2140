#include "inode.h"
#include "block_allocation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* **HJELPEMETODER** */
void free_helper(FILE *file, struct inode *inode){
    if (file != NULL){
        fclose(file);
    }

    if(inode != NULL){
        free(inode->name);
        free(inode->entries);
        free(inode);
    }
}


struct inode* create_file( struct inode* parent, const char* name, char readonly, int size_in_bytes )
{
    fprintf( stderr, "%s is not implemented\n", __FUNCTION__ );
    return NULL;
}

struct inode* create_dir( struct inode* parent, const char* name )
{
    fprintf( stderr, "%s is not implemented\n", __FUNCTION__ );
    return NULL;
}

struct inode* find_inode_by_name( struct inode* parent, const char* name )
{
    if(parent == NULL){
        return NULL;
    }

    if(!parent->is_directory){
        return NULL;
    }

    for(int i = 0; i < parent->num_entries; i++){
        struct inode *child = (struct inode*)parent->entries[i];  //caster uintptr_t til peker til inode 
        if(child != NULL && strcmp(child->name, name) == 0){
            return child;
        }
    }

    return NULL;
}

int delete_file( struct inode* parent, struct inode* node )
{
    fprintf( stderr, "%s is not implemented\n", __FUNCTION__ );
    return -1;
}

int delete_dir( struct inode* parent, struct inode* node )
{
    fprintf( stderr, "%s is not implemented\n", __FUNCTION__ );
    return -1;
}

void save_inodes( const char* master_file_table, struct inode* root )
{
    fprintf( stderr, "%s is not implemented\n", __FUNCTION__ );
    return;
}

struct inode* load_inodes( const char* master_file_table ){
    
    FILE *file = fopen(master_file_table, "rb");
    if(file == NULL){
        perror("Kunne ikke åpne master file table");
        return NULL;
    }

    //lager et (dynamisk) array som kan holde pekere til alle inodene vi lager når vi leser filen 
    int array_size = 10;
    int p = 0;  //Posisjon
    struct inode **inodes = malloc(array_size * sizeof(struct inode*));
    if(inodes == NULL){
        perror("malloc av inodes array feilet");
        fclose(file);
        return NULL;
    } 

    int running = 1;
    while(running){
        //Lager en tom inode som vi kan "fylle" med data. Den må legges på heapen (ikke stacken), fordi den må kunne "leve videre"
        // Bruker calloc i stedet for malloc slik at alle felt i structen initialiseres, og jeg da trygt kan kalle free() ved feil 
        struct inode *inode = calloc(1, sizeof(struct inode));
        if(inode == NULL){
            perror("Calloc av inode feilet");
            fclose(file);
            return NULL;
        }
       
        //Leser først inn ID
        uint32_t id;

        size_t rc = fread(&inode->id, sizeof(uint32_t), 1, file);  //rc = read count

        //Dersom fread på ID feiler, kan det være fordi vi har kommet til slutten av filen.
        //Dette kan vi sjekke med feof(file). Hvis den returnerer nonzero (= true), så betyr det at vi har nådd slutten av filen, og kan bryte ut av løkka
        //Dersom feof(file)returnerer zero (false), så betyr det at en annen feil har oppstått ved lesing. 
        if(rc < 1){
            if(feof(file)){
                break; //Vi har nådd slutten av filen, så vi bryter ut av løkka 
            } else if(ferror(file)){
                perror("Feil ved lesing av ID");  //En annen feil har oppstått 
                free_helper(file, inode);
                return NULL;
            }
        }

        //Leser lengden på navnet
        uint32_t name_length;
        rc = fread(&name_length, sizeof(uint32_t), 1, file);
        if(rc != 1){
            perror("Kunne ikke lese lengde på navn");
            free_helper(file, inode);
            return NULL;
        }

        inode->name = malloc(name_length);
        if(inode->name == NULL){
            perror("Malloc for navn feilet");
            free_helper(file, inode);
            return NULL;
        }

        //leser selve navnet 
        rc = fread(inode->name, sizeof(char), name_length, file);
        if(rc != name_length){
            perror("Kunne ikke lese navnet");
            free_helper(file, inode);
            return NULL;
        }

        //leser is_directory 
        rc = fread(&inode->is_directory, sizeof(char), 1, file);
        if(rc != 1){
            perror("Kunne ikke lese is_directory");
            free_helper(file, inode);
            return NULL;
        }
        //Sjekker om is_directory har gyldig verdi 
        if (inode->is_directory != 0 && inode->is_directory != 1) {
            perror("Ugyldig verdi for is_directory\n");
            free_helper(file, inode);
            return NULL;
        }

        //leser is_readonly
        rc = fread(&inode->is_readonly, sizeof(char), 1, file);
        if(rc != 1){
            perror("Kunne ikke lese is_readonly\n");
            free_helper(file, inode);
            return NULL;
        }

        //leser filesize
        if(inode->is_directory){
            inode->filesize = 0;
        } else{
            rc = fread(&inode->filesize, sizeof(uint32_t), 1, file);
            if(rc != 1){
                perror("Kunne ikke lese filesize\n");
                free_helper(file, inode);
                return NULL;
            }
        }

        //leser num_entries 
        rc = fread(&inode->num_entries, sizeof(uint32_t), 1, file);
        if (rc != 1){
            perror("Kunne ikke lese num_entries\n");
            free_helper(file, inode);
            return NULL;
        }

        //Nå må det settes av minne til entries (som er et array av størrelse num_entries * sizeof(uintptr_t)), hvis det er entries
        if(inode->num_entries > 0){
            inode->entries = malloc(inode->num_entries * sizeof(uintptr_t));
            if(inode->entries == NULL){
                perror("Malloc feilet");
                free_helper(file, inode);
                return NULL;
            }
        }
        //Hvis inode er en directory, så inneholder entries IDer til andre inoder 
        if(inode->is_directory){
            for(uint32_t i = 0; i<inode->num_entries; i++){
                rc = fread(&inode->entries[i], sizeof(uintptr_t), 1, file);
                if(rc != 1){
                    perror("Klarte ikke å lese ID i entries");
                    free_helper(file, inode);
                    return NULL;
                }
            }
        } else {
            //dersom inode er en fil 
            struct Extent *ext = (struct Extent *) inode->entries;

            for (int i = 0; i<inode->num_entries; i++){
                if(fread(&ext[i], sizeof(struct Extent), 1, file) != 1){
                    perror("Feilet å lese extend (=blocknr + extent) til entries i fil\n");
                    free_helper(file, inode);
                    return NULL;
                }
            }
            }

            //legger inoden i arrayet (men sjekker først om det trenger mer plass)
            if(p == array_size){
                array_size *= 2;
                inodes = realloc(inodes, array_size * sizeof(struct inode*));
            }

            inodes[p] = inode;
            p++;
    }

    //Nå skal nodene kobles
    for(int i = 0; i<p; i++){
        struct inode *current_inode = inodes[i];

        if(current_inode->is_directory){
            //Gå gjennom alle ID'er i entries og erstatt de med pekere til inodene med gitt ID
            for(int j = 0; j < current_inode->num_entries; j++){
                uint32_t id = current_inode->entries[j];
                current_inode->entries[j] = (uintptr_t) inodes[id];
            }
        }
    }
    
    fclose(file);
    return inodes[0];
}


void fs_shutdown( struct inode* inode )
{
    fprintf( stderr, "%s is not implemented\n", __FUNCTION__ );
    return;
}

/* This static variable is used to change the indentation while debug_fs
 * is walking through the tree of inodes and prints information.
 */
static int indent = 0;

static void debug_fs_print_table( const char* table );
static void debug_fs_tree_walk( struct inode* node, char* table );

void debug_fs( struct inode* node )
{
    char* table = calloc( NUM_BLOCKS, 1 );
    debug_fs_tree_walk( node, table );
    debug_fs_print_table( table );
    free( table );
}

static void debug_fs_tree_walk( struct inode* node, char* table )
{
    if( node == NULL ) return;
    for( int i=0; i<indent; i++ )
        printf("  ");
    if( node->is_directory )
    {
        printf("%s (id %d)\n", node->name, node->id );
        indent++;
        for( int i=0; i<node->num_entries; i++ )
        {
            struct inode* child = (struct inode*)node->entries[i];
            debug_fs_tree_walk( child, table );
        }
        indent--;
    }
    else
    {
        printf("%s (id %d size %d)\n", node->name, node->id, node->filesize );

        /* The following is an ugly solution. We expect you to discover a
         * better way of handling extents in the node->entries array, and did
         * it like this because we don't want to give away a good solution here.
         */
        uint32_t* extents = (uint32_t*)node->entries;

        for( int i=0; i<node->num_entries; i++ )
        {
            for( int j=0; j<extents[2*i+1]; j++ )
            {
                table[ extents[2*i]+j ] = 1;
            }
        }
    }
}

static void debug_fs_print_table( const char* table )
{
    printf("Blocks recorded in master file table:");
    for( int i=0; i<NUM_BLOCKS; i++ )
    {
        if( i % 20 == 0 ) printf("\n%03d: ", i);
        printf("%d", table[i] );
    }
    printf("\n\n");
}

