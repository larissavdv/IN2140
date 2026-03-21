***Lesing av master-file-table (MFT)***

MFT inneholder en sekvens av bytes som representerer records, der hver record beskriver en fil eller en mappe i filsystemet. 
MFT er selve representasjonen av hvordan filsystemet ligger lagret på disk, og brukes til å lese filsystemet inn i minnet.

I denne oppgaven har en record strukturen til en inode. 

En inode inneholder informasjon (metadata) om en fil eller en mappe, og informasjon om hvor filense data eventuelt ligger lagret på disk. 

Alle inoder inneholder følgende informasjon når de er lagret på disk:
ID
name_length  
name
is_directory
is_readonly
filesize
num_entries
entries 

Det er altså denne informasjone som MFT inneholder, og informasjonen ligger sekvensielt etter hverandre i MFT. 
Det er dermed nøyaktig i den rekkefølgen informasjonen må leses inn. 

Først må MFT filen åpnes for lesing. 
Deretter leses inoden én etter én frem til man når EOF (end of file)

For hver inode som leses fra disk, må det opprettes en ny struct inode i minnet. Det må settes av minne til denne på heapen (med malloc eller calloc), slik at den fortsetter å eksistere i minnet etter at funksjonen returnerer. 
Deretter leses hvert "felt" fra filen og lagres i structen.
Denne prosessen, der man leser informasjon fra MFT til faktiske strukturer i minnet, kalles "deserialisering".

Etter at ID er lest inn, må man lese inn navnet. Navnet er lagret på disk som lengden på navnet + selve navnet. 
Grunnen til at dette gjøres er at pekere ikke kan lagres på disk, og dataene må derfor serialiseres. 
Navnet kan derfor ikke leses direkte inn i en peker. Det må gjøres i to steg: først leses lengden på navnet, slik at man kan allokere riktig mengde minne til navnet.
Deretter leses selve navnet inni dette minnet, navnepekeren nå peker på.

Så leses is_directory og is_readonly, og deretter filesize. Dersom inode er en mapper, så er filesize alltid 0. 
Hvis det er en fil, så settes filesizen til det som ligger i MFT. 

num_entries er det neste som leses inn. For mapper er dette antall filer og mapper som den inneholder, mens for filer så er det antall extents som filen består av. 

Dataen som entries inneholder avhenger av om inoden er en mappe eller en fil.

For mapper:
I MFT tilsvarer entries IDene til de mappene og filene som som mappen inneholder. I minnet inneholder entries pekere til selve inodene med disse IDene.
Igjen, ettersom pekere ikke kan lagres på disk, så må dataen serialiseres, og det derfor det kun er IDene som ligger lagret i entries på disk, og ikke selve pekerne.
Det er derfor slik at når vi leser inn IDene fra MFT, så må senere i prosessen erstatte disse med pekere til de faktiske inodene.
Ettersom vi ikke har opprettet alle inoder ennå når vi leser fra MFT, så kan dette ikke gjøres før alle inoder er lest inn. 

For filer: 
Her inneholder entries informasjon om hvor filens data ligger på disk. I MFT består hver entry av et blockno og en extent. I minnet inneholder entries pekere til strukturer "struct Extends". 
Igjen så er dette en serialisering av data for å kunne lagre det på disk. 
For hver entry (blockno + extent) som leses inn fra MFT opprettes en ny struct Extend (det allokeres minne til structen med malloc).
Blockno og extent lagres i denne structen, og en peker til denne lagres i entries. 

Forklaring av extents:
En extent er et sammenhengde område på disk. 
Blockno representerer startblokken, og extent hvor mange sammenhengde blokker extenten består av.
For eksempel:

blockno =  15
extent = 3

Dette betyr det at filen ligger på blokkene 15, 16 og 17. 
Dersom filen har flere entries (= pekere til ulike Extents), betyr det at filen er "brutt opp" i flere deler på disk. 

entries = {Extent{15, 3}, Extent{21,2}}
Så ligger filen på blokkene 15,16,17 og 21,22. 

Antall extends som trengs for en fil avhenger av filstørrelse og antall ledige (sammenhengende) blokker på disk.
Større filer trenger flere blokker, og dersom det ikke er nok sammenhengende blokker ledig, må filen "fordeles" over flere extents.

Entries er deklarert som uintptr_t*. På grunn av dette kan vi bruk det samme feltet til både pekere til inoder (for mapper), pekere til Extents (for filer)


Hver inode som leses fra MFT lagres i et dynamisk array. Når vi har lest inn alle inodene, har vi som nevnt ikke koblet de sammen ennå. 
Alle mapper sin entries inneholder kun IDene til sine barne-inoder, men ikke pekere til selve barne-inodene.
Derfor må vi gå over hver inode i arrayet og erstatte IDen i entries med en peker til den faktiske inoden med denne IDen. 

Nå er hele filsystemet koblet sammen, og man har et tre av inoder i minnet. Vi kan da returnere root-noden og få tilgang til alle mapper og filer fra denne. 


***Avvik fra prekoden***

Jeg har lagt til 3 hjelpemetoder utover metodene gitt i prekoden:

1. struct inode* read_one_inode(FILE *file);

Denne er laget for å forenkle load_inodes() noe. 
Istedenfor å gjøre alt i load_inodes, så utfører read_one_inode() deserialiserering av én inode av gangen, returnerer en peker til denne. 
load_inodes kaller read_one_inode, og plasserer den returnerte pekeren i et dynamisk array som senere brukes til koblingen mellom inoder. Dette gjentas frem til vi når EOF. 

2. void free_helper(struct inode *inode);
Denne brukes for å frigjøre allokert minne på heapen dersom noe feiler underveis i lesing eller allokering av minne. 


3. void write_inode(FILE *file, struct inode* node);

Dette er en rekursiv hjelpemetode som brukes i save_inodes. Den serialiserer inodene fra minnet til MFT. 