#include "inode.h"
#include "block_allocation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//statisk variabel for å holde på sist brukte ID
static uint32_t last_id = 0;

struct inode* create_file( struct inode* parent, const char* name, char readonly, int size_in_bytes )
{
    if(parent == NULL || !parent->is_directory){
        return NULL;
    }

    if (find_inode_by_name(parent, name) != NULL){ //Finnes allerede en inode med dette navnet 
        return NULL;
    }

    int name_length = strlen(name) +1; //Må huske å ha med +1 for \0 her
    int nr_blocks = (size_in_bytes + 4095) / 4096;
    int nr_extents = (nr_blocks +3) / 4;


    struct inode *new = calloc(1, sizeof(struct inode));
    if(new == NULL){
        perror("Calloc til ny fil feilet");
        return NULL;
    }

    new->name = malloc(name_length);
    if(new->name == NULL){
        perror("Malloc av name feilet");
        return NULL;
    }
    strcpy(new->name, name);

    new->id = ++last_id;
    new->is_directory = 0;
    new->is_readonly = readonly;
    new->filesize = size_in_bytes;
    new->num_entries = nr_extents;

    new->entries = malloc(nr_extents * sizeof(struct Extent));
    if(new->entries == NULL){
        perror("malloc til entries feilet");
        free(new->name);
        free(new);
        return NULL;
    }


    /*. ***MÅ SETTE MEG INN I DETTE***
    
    struct Extent *ext = (struct Extent *) new->entries;
    int remaining = nr_blocks;
    int current;

    for(int i = 0; i < nr_extents; i++){
        if(remaining > 4){
            current = 4;
        } else{
            current = remaining;
        }
        
        int first_block = allocate_blocks(current);

        if(first_block == -1){
            perror("Ikke nok minne igjen!");
            free(new->entries);
            free(new->name);
            free(new);
            return NULL;
        } 

        ext[i].blockno = first_block;
        ext[i].extent = current;

        remaining = remaining - current;

    }

    uintptr_t *tmp = realloc(parent->entries,
                             (parent->num_entries + 1) * sizeof(uintptr_t));
    if (tmp == NULL) {
        free(new->entries);
        free(new->name);
        free(new);
        return NULL;
    }

    parent->entries = tmp;
    parent->entries[parent->num_entries] = (uintptr_t) new;
    parent->num_entries++;

    return new;

*/
    
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


//Hjelpefunksjon for å frigjøre ressurser fra heapen dersom noe feiler 
void free_helper(struct inode *inode){
    if(inode != NULL){
        free(inode->name);
        free(inode->entries);
        free(inode);
    }
}

struct inode* read_one_inode(FILE *file){

    //Bruker resultatet fra første fread til å vurdere om vi har nådd enden av filen eller ikke 
    uint32_t id;
    size_t rc = fread(&id, sizeof(uint32_t), 1, file);

    if (rc != 1) {
        if (feof(file)) { // Har nådd slutten av filen
            return NULL;  
        } else {
            perror("Feil ved lesing av ID");
            return NULL;
        }
    }

    //Vi skal returnere en peker som må kunne "leve videre" etter at funksjonen er ferdig, må derfor legge den i heapen  
    // Bruker calloc i stedet for malloc slik at alle felt i structen settes, og jeg da trygt kan kalle free() senere i hjelpemetoden 
    struct inode *inode = calloc(1, sizeof(struct inode));
    if(inode == NULL){
        perror("Calloc feilet");
        return NULL;
    }

    //Setter den leste IDen til inode sin ID
    inode->id = id;


    //Så må vi håndtere navnet. Navnet er lagret som størrelse + selve navnet.
    //Henter først størrelse: 
    uint32_t size;
    if(fread(&size, sizeof(uint32_t), 1, file) != 1){
        printf("Feil ved lesing av navnelengde\n");
        free_helper(inode);
        return NULL;
    }

    //Nå kan vi allokere minne til navnet 
    inode->name = malloc(size);
    if(inode->name == NULL){
        printf("Malloc feilet\n");
        free_helper(inode);
        return NULL;
    }

    //Nå kan vi lese navnet, fordi vi vet lengden på det 
    if(fread(inode->name, sizeof(char), size, file) != size){
        printf("Feil ved lesing av navn\n");
        free_helper(inode);
        return NULL; 
    }

    //Vi leser nå is_directory 
    if(fread(&inode->is_directory, sizeof(char), 1, file) != 1){
        printf("Feil ved lesing av is_directory\n");
        free_helper(inode);
        return NULL; 
    }

    //Sjekker om is_directory har gyldig verdi 
    if (inode->is_directory != 0 && inode->is_directory != 1) {
        printf("Ugyldig verdi for is_directory\n");
        free_helper(inode);
        return NULL;
    }

    //leser inn is_readonly
    if(fread(&inode->is_readonly, sizeof(char), 1, file) != 1){
        printf("Feil ved lesing av is_readonly\n");
        free_helper(inode);
        return NULL; 
    }


    //leser filesize hvis inoden er en file (!is_directory)
    if(!inode->is_directory){
        if(fread(&inode->filesize, sizeof(uint32_t), 1, file)!= 1){
            printf("Feil ved lesing av filesize\n");
            free_helper(inode);
            return NULL; 
        }
    }else{
            inode->filesize = 0;
        }
    
    //leser num_entries 
    if(fread(&inode->num_entries, sizeof(uint32_t), 1, file)!= 1){
        printf("Feil ved lesing av num_entries\n");
        free_helper(inode);
        return NULL; 
    }

    //Nå må det settes av minne til entries (som er et array av størrelse num_entries * sizeof(uintptr_t)), hvis det er entries
    if(inode->num_entries > 0){
        inode->entries = malloc(inode->num_entries * sizeof(uintptr_t));
        if(inode->entries == NULL){
            perror("Malloc feilet");
            free_helper(inode);
            return NULL;
        }
    } 

    //Til slutt leser vi inn entries, avhengig av om det er directory eller file 
    if(inode->is_directory){
        //Hvis inode er en directory, så skal vi lagre IDene til de andre inodene i entries 
        for(int i = 0; i<inode->num_entries; i++){
            if(fread(&inode->entries[i], sizeof(uintptr_t), 1, file)!= 1){
                printf("Feilet å lese ID til entries i directory\n");
                free_helper(inode);
                return NULL;
            }
        }
    }else{
        //Hvis inode er en fil, så kan vi bruke struct Extent til å "lagre" denne informasjonen 
        //bruk inode->entries-minnet, men tolk det som Extent
        struct Extent *ext = (struct Extent *) inode->entries;

        for (int i = 0; i<inode->num_entries; i++){

            if(fread(&ext[i], sizeof(struct Extent), 1, file)!=1){
                printf("Feilet å lese extend (=blocknr + extent) til entries i fil\n");
                free_helper(inode);    
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


    //Bruker en hjelpemetode read_one_inode for å lage en struct inode for hver inode i master_file_table
    //Disse nodene lagres midlertidig i et array. Siden størrelsen er ukjent på forhånd lages et "dynamisk" array 

    int array_size = 10;
    int p = 0; //posisjon

    struct inode **inodes = malloc(array_size*sizeof(struct inode*));
    if(inodes == NULL){
        perror("Malloc feilet");
        return NULL;
    }

    while(1){
        struct inode *inode = read_one_inode(file);
        if(inode == NULL){
            break; //Har nådd enden av filen 
        }

        if(p == array_size){
            array_size *= 2;
            //lagrer i en ny peker i tilfelle realloc feiler, slik at vi ikke mister den "gamle"
            struct inode **tmp = realloc(inodes, array_size * sizeof(struct inode*));
            if(tmp == NULL){
                perror("Realloc feilet");
                fclose(file);
                return NULL;
            }
            inodes = tmp;
        }

        last_id = inode->id;

        inodes[p] = inode;
        p++;        
    }

        //Går nå gjennom alle nodene 
        for(int i = 0; i < p; i++){
            struct inode *current_inode = inodes[i];
            if(current_inode->is_directory){
                //Gå gjennom alle ID'er i entries og erstatt de med pekere til inodene med gitt ID
                for(int j = 0; j < current_inode->num_entries; j++){
                    uint32_t current_id = current_inode->entries[j];
                    //Må nå finne inoden med current_id i inodes 
                    for(int k = 0; k<p; k++){
                        struct inode *child = inodes[k];
                        if(child->id == current_id){
                            current_inode->entries[j] = (uintptr_t) child;
                            break;
                        }
                    }
                }
                
            }
        }
    
    struct inode *root = NULL;

    for(int i = 0; i<p; i++){
        if(strcmp(inodes[i]->name, "/") == 0){
            root = inodes[i];
            break;
        }
    }

    if (root == NULL) {
        printf("Fant ikke root inode\n");
        free(inodes);
        fclose(file);
        return NULL;
    }

    fclose(file);
    free(inodes);
    return root;
    

}

void fs_shutdown( struct inode* inode )
{
    if (inode == NULL) {
        return;
    }

    if (inode->is_directory) {
        for (int i = 0; i < inode->num_entries; i++) {
            struct inode *entry = (struct inode*) inode->entries[i];
            fs_shutdown(entry); //Kaller fs_shutdown rekursivt på alle barn av et directory
        }
    }

    //Hvis inode ikke er en directory, eller alle barn er frigjort rekursivt, så frigjøres inode
    //Hvis inode frigjøres for dens entries er frigjort, så har vi ingen måte i frigjøre de på senere 
    free(inode->name);
    free(inode->entries);
    free(inode);
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

