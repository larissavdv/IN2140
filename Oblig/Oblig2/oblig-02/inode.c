#include "inode.h"
#include "block_allocation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
    fprintf( stderr, "%s is not implemented\n", __FUNCTION__ );
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





//Hjelpefunksjon for å sjekke om fread() feiler
int read_failed(void *destination, size_t size, size_t count, FILE *file){
    return fread(destination, size, count, file) != count;
}

//Hjelpefunksjon for å frigjøre ressurser fra heapen dersom noe feiler 
void free_helper(FILE *file, struct inode *i){
    if(file != NULL){
       fclose(file); 
    }
    if(i != NULL){
        free(i->name);
        free(i->entries);
        free(i);
    }

}

struct inode* read_inode(FILE *file){

    //Vi skal returnere en peker som må kunne "leve videre" etter at funksjonen er ferdig, må derfor legge den i heapen  
    // Bruker calloc i stedet for malloc slik at alle felt i structen settes, og jeg da trygt kan kalle free() senere i hjelpemetoden 
    struct inode *inode = calloc(1, sizeof(struct inode));
    if(inode == NULL){
        perror("Calloc feilet");
        return NULL;
    }
    

    //Leser først ID'en:
    if(read_failed(&inode->id, sizeof(uint32_t), 1, file)){
        printf("Feil ved lesing av ID\n");
        free_helper(file, inode);
        return NULL;
    }

    //Så må vi håndtere navnet. Navnet er lagret som størrelse + selve navnet.
    //Henter først størrelse: 
    uint32_t size;
    if(read_failed(&size, sizeof(uint32_t), 1, file)){
        printf("Feil ved lesing av navnelengde\n");
        free_helper(file, inode);
        return NULL;
    }

    //nå kan vi allokere minne til navnet 
    inode->name = malloc(size);
    if(inode->name == NULL){
        printf("Malloc feilet\n");
        free_helper(file, inode);
        return NULL;
    }

    //Nå kan vi lese navnet, fordi vi vet lengden på det 
    if(read_failed(inode->name, sizeof(char), size, file)){
        printf("Feil ved lesing av navn\n");
        free_helper(file, inode);
        return NULL; 
    }

    //Vi leser nå is_directory 
    if(read_failed(&inode->is_directory, sizeof(char), 1, file)){
        printf("Feil ved lesing av is_directory\n");
        free_helper(file, inode);
        return NULL; 
    }

    //Sjekker om is_directory har gyldig verdi 
    if (inode->is_directory != 0 && inode->is_directory != 1) {
        printf("Ugyldig verdi for is_directory\n");
        free_helper(file, inode);
        return NULL;
    }

    //leser inn is_readonly
    if(read_failed(&inode->is_readonly, sizeof(char), 1, file)){
        printf("Feil ved lesing av is_readonly\n");
        free_helper(file, inode);
        return NULL; 
    }


    //leser filesize hvis inoden er en file (!is_directory)
    if(!inode->is_directory){
        if(read_failed(&inode->filesize, sizeof(uint32_t), 1, file)){
            printf("Feil ved lesing av filesize\n");
            free_helper(file, inode);
            return NULL; 
        }
    }else{
            inode->filesize = 0;
        }
    
    //leser num_entries 
    if(read_failed(&inode->num_entries, sizeof(uint32_t), 1, file)){
        printf("Feil ved lesing av num_entries\n");
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

    //Til slutt leser vi inn entries, avhengig av om det er directory eller file 
    if(inode->is_directory){
        //Hvis inode er en directory, så skal vi lagre IDene til de andre inodene i entries 
        for(int i = 0; i<inode->num_entries; i++){
            if(read_failed(&inode->entries[i], sizeof(uintptr_t), 1, file)){
                printf("Feilet å lese ID til entries i directory\n");
                free_helper(file, inode);
                return NULL;
            }
        }
    }else{
        //Hvis inode er en fil, så kan vi bruke struct Extent til å "lagre" denne informasjonen 
        //bruk inode->entries-minnet, men tolk det som Extent
        struct Extent *ext = (struct Extent *) inode->entries;

        for (int i = 0; i<inode->num_entries; i++){

            if(read_failed(&ext[i], sizeof(struct Extent), 1, file)){
                printf("Feilet å lese extend (=blocknr + extent) til entries i fil\n");
                free_helper(file, inode);
                return NULL;
            }    
        }
    }
    return inode;
}

struct inode* load_inodes( const char* master_file_table ){

    FILE *file = fopen(master_file_table, "rb");
    if(file == NULL){
        perror("Kunne ikke åpne fil");
        return NULL;
    }

    fseek(file, 0, SEEK_END);       //setter pekeren til slutten av filen
    long file_end = ftell(file);    //Henter verdien til pekeren
    fseek(file, 0, SEEK_SET);       //Setter pekeren tilbake til starten av filen, slik at vi kan begynne å lese fra start 

    //Bruker en hjelpemetode read_inode for å lage en struct inode for hver inode i master_file_table
    //For å holde på alle inodene lagres de i et array. Siden størrelsen er ukjent på forhånd lages et "dynamisk" array 

    int array_size = 10;
    int i = 0; 

    struct inode **inodes = malloc(array_size*sizeof(struct inode*));
    if(inodes == NULL){
        perror("Malloc feilet");
        return NULL;
    }

    while(ftell(file) < file_end){
        struct inode *inode = read_inode(file);
        if(inode == NULL){
            printf("Feil ved innlesing av noder");
            free(inodes);
            return NULL;
        }
        if(i == array_size){
            array_size *= 2;
            inodes = realloc(inodes, array_size * sizeof(struct inode*));
        }
        inodes[i] = inode;
        i++;        
    }

    //Går nå gjennom alle nodene 
    for(int j = 0; j < i; j++){
        struct inode *current_inode = inodes[j];

        if(current_inode->is_directory){
            //Gå gjennom alle ID'er i entries og erstatt de med pekere til inodene med gitt ID
            for(int k = 0; k < current_inode->num_entries; k++){
                uint32_t id = current_inode->entries[k];
                //struct inode *child_inode = find_inode_by_id(inodes,i, id); //hjelpemetode?? 
                //current_inode->entries[k] = child_inode;
                current_inode->entries[k] = (uintptr_t) inodes[id];

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

