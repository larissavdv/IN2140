#include "inode.h"
#include "block_allocation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//statisk variabel for å holde på sist brukte ID
static uint32_t last_id = 0;
static int block_size = 4096;

struct inode* create_file( struct inode* parent, const char* name, char readonly, int size_in_bytes )
{

    if(parent == NULL || !parent->is_directory){ //en ny fil må lages fra/inni en directory
        return NULL;
    }

    if (find_inode_by_name(parent, name) != NULL){ //Finnes allerede en inode med dette navnet 
        return NULL;
    }

    struct inode *new = calloc(1, sizeof(struct inode));  //Lager en ny inode til filen 
    if(new == NULL){
        perror("Calloc til ny fil feilet");
        return NULL;
    }

    int name_length = strlen(name) +1; //Må huske å ha med +1 for \0 her
    new->name = malloc(sizeof(char)*name_length);
    if(new->name == NULL){
        perror("Malloc til name feilet");
        free(new);
        return NULL;
    }

    new->id = last_id++;
    new->name = strcpy(new->name, name);
    new->is_directory = 0;
    new->is_readonly = readonly;
    new->filesize = size_in_bytes;
   
    int blocks_total = (size_in_bytes + (block_size-1)) / block_size;  //Antall blokker som trengs for hele filen
    int nr_entries = (blocks_total+3)/4;  //En Extend kan holde maks 4 blokker, så dette blir hvor mange strct Extend vi trenger 

    new->num_entries = nr_entries;
    new->entries = malloc(sizeof(struct Extent)*nr_entries);

    if(new->entries == NULL){
        perror("Malloc til entries feilet");
        free(new->name);
        free(new);
        return NULL;
    }

    int remaining = blocks_total;
    int calling;
    int start_block;

    //Caster new->entries pekeren til å være av typen struct Extent *
    struct Extent *ext = (struct Extent *) new->entries;
    
    int i = 0;

    while(remaining > 0){
        if(remaining > 4){
            calling = 4;
        } else{
            calling = remaining;
        }

        start_block = allocate_blocks(calling);

        while(start_block == -1 && calling > 1){
            //Prøver på nytt med færre extends
            calling--;
            start_block = allocate_blocks(calling);
        }

        if(start_block == -1){
            perror("Ikke nok minne igjen på disk");
            free(new->name);
            free(new->entries);
            free(new);
            return NULL;
        }

        //hvis allokering var suksessfull
        remaining = remaining - calling;
        

        ext[i].blockno = start_block;
        ext[i].extent = calling;

        i++;

    }
   
    //må realoccere minnet til parent sine entries, for å gi plass til den nye filen 
    int entries = parent->num_entries;
    entries++;
    uintptr_t *parent_entries = realloc(parent->entries, (sizeof(uintptr_t)*entries));
    if(parent_entries == NULL){
        perror("realloc av parent entries feilet");
        free(new->name);
        free(new->entries);
        free(new);
        return NULL;
    }

    int next_pos = parent->num_entries;
    parent->entries = parent_entries;
    parent->entries[next_pos] = (uintptr_t) new;
    parent->num_entries++;

    return new;
    
}

struct inode* create_dir( struct inode* parent, const char* name )
{
    if (parent == NULL && strcmp(name, "/") != 0) { //parent kan være null, men bare når vi opprettet root
        return NULL;
    }
    else if(parent != NULL && !parent->is_directory){
        return NULL;
    }
    if (find_inode_by_name(parent, name) != NULL){ //Finnes allerede en inode med dette navnet 
        return NULL;
    }

    struct inode *new = calloc(1, sizeof(struct inode));  
    if(new == NULL){
        perror("Calloc til nytt directory feilet");
        return NULL;
    }

    int name_length = strlen(name) +1;
    new->name = malloc(sizeof(char)*name_length);
    if(new->name == NULL){
        perror("Malloc til name feilet");
        free(new);
        return NULL;
    }

    new->id = last_id++;
    new->name = strcpy(new->name, name);
    new->is_directory = 1;
    new->is_readonly = 0;
    new->filesize = 0;
    new->num_entries = 0;
    new->entries = NULL;   //Et nytt directory har ingen entries ennå 

    if(parent == NULL){ //I tilfellet der vi har opprettet root, så er det ingen parent 
        return new;
    }

    int entries = parent->num_entries;
    entries++;
    uintptr_t *parent_entries = realloc(parent->entries, (sizeof(uintptr_t)*entries));

    if(parent_entries == NULL){
        perror("realloc av parent entries feilet");
        free(new->name);
        free(new);
        return NULL;
    }

    int next_pos = parent->num_entries;
    parent->entries = parent_entries;
    parent->entries[next_pos] = (uintptr_t) new;
    parent->num_entries++;

    return new;

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
    if(parent == NULL || node == NULL){
        return -1;
    }

    if(!parent->is_directory || node->is_directory){
        return -1;
    }

    //finner posisjonen til node i parent sin entries, som senere kan brukes til å fjerne den fra entries 
    int pos = -1;

    for(int i = 0; i<parent->num_entries; i++){
        if((struct inode *)parent->entries[i] == node){
            pos = i;
            break;
        }
    }

    if(pos == -1){
        return -1; //Noden finnes ikke i parent!
    }

    struct Extent *ext = (struct Extent *) node->entries; 

    for(int i = 0; i<node->num_entries; i++){
        int blocknr = ext[i].blockno;
        int remaining = ext[i].extent;

        while(remaining>0){
            free_block(blocknr);
            blocknr++;
            remaining--;
        }
    }

    //flytter entriene i entries 
    for(int i = pos; i<parent->num_entries-1; i++){
        parent->entries[i] = parent->entries[i+1];
    }

    parent->num_entries--;

    free(node->name);
    free(node->entries);
    free(node);
    return 0;

}

int delete_dir( struct inode* parent, struct inode* node )
{
    if(node != NULL && !node->is_directory){
        return -1;
    }
    if(parent != NULL && !parent->is_directory){
        return -1;
    }

    if(find_inode_by_name(parent, node->name) == NULL){
        return -1; //node finnes ikke i entries til parent 
    }

    if(node->num_entries != 0){
        return -1; //directory er ikke tomt 
    }

    int pos = -1;

    for(int i = 0; i<parent->num_entries; i++){
        if((struct inode *)parent->entries[i] == node){
            pos = i;
            break;
        }
    }
    
    for(int i = pos; i<parent->num_entries-1; i++){
        parent->entries[i] = parent->entries[i+1];
    }

    parent->num_entries--;

    free(node->name);
    free(node->entries);
    free(node);

    return 0;

}

void save_inodes( const char* master_file_table, struct inode* root )
{
    FILE *file = fopen(master_file_table, "wb");
    if(file == NULL){
        perror("Kunne ikke åpne master file table");
        return;
    }

    write_inode(file, root);

    fclose(file);
}

//Hjelpefunksjon for å frigjøre ressurser fra heapen dersom noe feiler 
void free_helper(struct inode *inode){
    if(inode != NULL){
        free(inode->name);
        free(inode->entries);
        free(inode);
    }
}
//Hjelpefunksjon for å lese én og én inode 
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

    int array_size = 10;
    int p = 0; //posisjon

    //Nodene som skal leses lagres midlertidig i et array. Siden størrelsen er ukjent på forhånd lages et "dynamisk" array
    struct inode **inodes = malloc(array_size*sizeof(struct inode*));
    if(inodes == NULL){
        perror("Malloc feilet");
        return NULL;
    }

    while(1){
        //Bruker hjelpemetoden read_one_inode
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

//Hjelpefunksjon for skriving av én inode til fil
void write_inode(FILE *file, struct inode* node){
    if(node == NULL){
        return;
    }

    fwrite(&node->id, sizeof(uint32_t), 1, file);

    int len = strlen(node->name)+1;
    fwrite(&len, sizeof(uint32_t), 1, file);
    fwrite(node->name, sizeof(char), len, file);
    fwrite(&node->is_directory, sizeof(char), 1, file);
    fwrite(&node->is_readonly, sizeof(char), 1, file);

    if(!node->is_directory){
        fwrite(&node->filesize, sizeof(uint32_t), 1, file);
    }
    
    fwrite(&node->num_entries, sizeof(uint32_t), 1, file);
    
    if(node->is_directory){
        for(int i = 0; i<node->num_entries; i++){
            struct inode *child = (struct inode * )node->entries[i];
            fwrite(&child->id, sizeof(uintptr_t), 1, file);
        }

        for (int i = 0; i < node->num_entries; i++) {
            struct inode *child = (struct inode *) node->entries[i];
            write_inode(file, child);
        }

    }else{
        struct Extent *ext = (struct Extent *) node->entries;
        for(int i = 0; i<node->num_entries; i++){
            fwrite(&ext[i].blockno, sizeof(uint32_t), 1, file);
            fwrite(&ext[i].extent, sizeof(uint32_t), 1, file);
        }

        
    }

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

