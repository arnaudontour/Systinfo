// TOUT LES INCLUDES

#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include "sha256.c"
#include "reverse.c"


// TOUTE LES CONSTANTES

#define true 1 

//CREATION DES MUTEX ET SEMAPHOREs

pthread_mutex_t mutex;
pthread_mutex_t mutex2;
sem_t empty;
sem_t full;
sem_t empty2;
sem_t full2;

int nmbfiles;
char **fichier;
bool fini;
bool fini2;

bool isconsonne;
int nthread;
int nmb;
int nbmarg;
//ARRAYLIST BUFFER 1 (32bytes)

bool verbose=false;

typedef struct Element Element;
struct Element
{
    Element *suivant;
    uint8_t *hash;
};


typedef struct Liste Liste;
struct Liste
{
    Element *premier;
};

Liste *initialisation()
{
    Liste *liste = malloc(sizeof(*liste));
    Element *element = malloc(sizeof(*element));

    if (liste == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }

    element->hash = NULL;
    element->suivant = NULL;
    liste->premier = element;

    return liste;
}


void insertion(Liste *liste, uint8_t *value)
{
    /* Création du nouvel élément */
    Element *nouveau = malloc(sizeof(struct Element));
    nouveau->hash = (uint8_t *)malloc(32);
    if (liste == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }
    memcpy(nouveau->hash,value,32);

    /* Insertion de l'élément au début de la liste */
    nouveau->suivant = liste->premier;
    liste->premier = nouveau;
}

void suppression(Liste *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (liste->premier != NULL)
    {
        Element *aSupprimer = liste->premier;
        liste->premier = liste->premier->suivant;
        free(aSupprimer->hash);
        free(aSupprimer);
    }
}

//ARRAYLIST BUFFER 2 (liste avec les traductions)

typedef struct Element2 Element2;
struct Element2
{
    Element2 *suivant;
    char *trad;
};


typedef struct Liste2 Liste2;
struct Liste2
{
    Element2 *premier;
};

Liste2 *initialisation2()
{
    Liste2 *liste2 = malloc(sizeof(*liste2));
    Element2 *element2 = malloc(sizeof(*element2));

    if (liste2 == NULL || element2 == NULL)
    {
        exit(EXIT_FAILURE);
    }

    element2->trad = NULL;
    element2->suivant = NULL;
    liste2->premier = element2;

    return liste2;
}


void insertion2(Liste2 *liste2, char* trad)
{
    /* Création du nouvel élément */
    Element2 *nouveau = malloc(sizeof(struct Element2));
    if (liste2 == NULL || nouveau == NULL)
    {
        if(verbose)
            printf("coucou\n");
    }
    nouveau->trad = trad;

    /* Insertion de l'élément au début de la liste */
    nouveau->suivant = liste2->premier;
    liste2->premier = nouveau;
}

void suppression2(Liste2 *liste2)
{
    if (liste2 == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (liste2->premier != NULL)
    {
        Element2 *aSupprimer = liste2->premier;
        liste2->premier = liste2->premier->suivant;
        
        free(aSupprimer);
    }
}

//DELETE POUR LE BUFFER 3 (List avec X nombre de voyelle ou consonne)
void delete(Liste2 *liste2){
    while(liste2->premier->suivant != NULL){
        free(liste2->premier->trad);
        suppression2(liste2);
    }
}
void delete2(Liste *liste1){
    while(liste1->premier->suivant != NULL){
        
        suppression(liste1);
    }
}
// fonction pour voyelle consonne
Liste *list1;
Liste2 *list2;
Liste2 *list3;

int voyelle(char chaine[16])
{
   int count_v = 0;
   int i;
   for(i=0; chaine[i]!='\0';i++)
   {
      if (chaine[i]=='a' || chaine[i]=='e' || chaine[i]=='i' || chaine[i]=='o' || chaine[i]=='u' || chaine[i]=='y')
          count_v+=1;
      
   }
   return count_v;
}

int consonne(char chaine[16])
{
   int count_c = 0;
   int i;
   for(i=0; chaine[i]!='\0';i++)
   {
      if (chaine[i]!='a' && chaine[i]!='e' && chaine[i]!='i' && chaine[i]!='o' && chaine[i]!='u' && chaine[i]!='y')
          count_c+=1;
      
   }
   return count_c;
}
// fonctin d'écriture sur console/fichier
/*void outfichier (Liste2 *list2, char *str){
    
    
    
    if (list2 == NULL)
    {
        exit(EXIT_FAILURE);
    }
    
    int fd = open(str, O_APPEND);
    Element2 *premier = list2->premier;
    
    while (premier)
    {
        fputs(list2->premier->trad, fd)
        suppression2(list2)
    }
    printf("NULL\n");
    
}
*/



void outconsole(Liste2 *list2){
   
    while(list2->premier->suivant != NULL){
    
        
        printf("%s\n", list2->premier->trad);
        suppression2(list2);
   
}
}


 //Premier producteur

void * producer(void*arg){
	int i;
    if(verbose)
        printf("debut producteur\n");
    for(i=nmbfiles;i < nbmarg ; i++){
        int fd, rd;
    fd = open(fichier[i],O_RDONLY);
    if(fd < 0){
    if(verbose)
        printf("je n'ai pas reussi a ouvrir fichier\n");
    exit(1);

    }

        rd = 1;
        while(rd!=0){
	   

        sem_wait(&empty);
	   pthread_mutex_lock(&mutex);

	   uint8_t *hashe = malloc(sizeof(char*)*32);
	   
    
        rd = read(fd,hashe,32);
        if(rd < 0){
       // return -1;
        }
        if(rd == 0){
            pthread_mutex_unlock(&mutex);
            free(hashe);
            break;
        }
        insertion(list1,hashe);
        free(hashe);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
	   }
	   
	int cl=close(fd);
        if (cl < 0){
        //return -1;
        }
	
	
	}
    fini = true;
     if(verbose)
        printf("fin producteur\n");
     pthread_exit(NULL);
    return NULL;
}


// premier consommateur

void* consumer(void*arg){
 if(verbose)
    printf("debut consumer\n");
while(!fini || list1->premier->suivant!=NULL){
      while(list1->premier->suivant != NULL){
    pthread_mutex_lock(&mutex); 
    if (list1->premier->suivant == NULL) 
    {
    pthread_mutex_unlock(&mutex);
    continue;
    }
	   void * x = malloc(32); 
       sem_wait(&full);
	   //pthread_mutex_lock(&mutex);
        uint8_t g[32];
        memcpy(&g,list1->premier->hash,32);
        suppression(list1);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        int ret=reversehash((void *) &g, (void *) x, 6);
        if(verbose)
            printf("%d",ret);
        sem_wait(&empty2);
        pthread_mutex_lock(&mutex2);
        insertion2(list2, (void *) x);
        pthread_mutex_unlock(&mutex2);
        sem_post(&full2);
	
	}
    
    
}

 if(verbose)
    printf("fin consumer\n");
 pthread_exit(NULL);
return NULL;
}

void* consumer2(void*arg){  // ICI ON VA METTRE QU'UN THREAD DU COUP BALLEC

 if(verbose)
    printf("debut consumer2\n");

while(!fini2|| list2->premier->suivant != NULL){
   
        pthread_mutex_lock(&mutex2); 
    if (list2->premier->suivant == NULL) 
    {
        pthread_mutex_unlock(&mutex2);
        continue;
    }
    if(verbose)
        printf("rentre dans while\n");
sem_wait(&full2);
if(verbose)
    printf("rentre dans semaphore\n");
//pthread_mutex_lock(&mutex2);
char *trans = list2->premier->trad;
suppression2(list2);
pthread_mutex_unlock(&mutex2);
sem_post(&empty2);
if(verbose)
    printf("sors de la semaphore\n");
int acc = 0;
if(isconsonne){
acc = consonne(trans);
    if(acc == nmb){
        insertion2(list3, trans);
    }
    else if(acc > nmb){
        delete(list3);
        insertion2(list3,trans);
    }
    else{
        free(trans);
    }
}
else{ 
   acc = voyelle(trans);
    if(acc == nmb){
        insertion2(list3, trans);
    }
    else if(acc > nmb){
        delete(list3);
        insertion2(list3,trans);
        nmb=acc;
    }
    else{
        free(trans);
    }

}
}

 if(verbose)
    printf("fin consommateur2  \n");
 pthread_exit(NULL);

return NULL;
}

int main (int argc, char *argv[]){

// variable utile;
int err;
fini = false;
nmb = 1;
nmbfiles= 1;
fichier = argv;
isconsonne = false;
nthread = 1;
char options;
//bool isfichierout =false;
char* fichierout = NULL;
nbmarg = argc;

while((options=getopt(argc, argv,"t:co:v"))!= -1){
    switch(options){

        case 't' : 
        nthread = strtol(optarg,NULL,10);
        break;

        case 'c' :
        isconsonne = true;
        break;

        case 'o' : 
        //isfichierout = true;
        fichierout = optarg;
        break;

        case 'v' :
        verbose = true;
        break;
    }
}
nmbfiles = optind;
if(verbose)
    printf("finigetopt\n");
if(verbose)
    printf("nthread :  %d \n", nthread);

//INITIALISATION DES MUTEX ET SEMAPHORE
if(verbose)
    printf("initialisation mutex semaphore \n");
pthread_mutex_init(&mutex2,NULL);
pthread_mutex_init(&mutex, NULL);
sem_init (&empty,0,20);
sem_init(&full,0,0);
sem_init (&empty2,0,20);
sem_init(&full2,0,0);



// initialisation des threads
if(verbose)
    printf("creation1 des pthread \n");
pthread_t thread1;
pthread_t thread2[nthread];
pthread_t thread3;

//initialisation des lists
if(verbose)
    printf("initialisation des listes \n");
list1 = initialisation();
list2 = initialisation2();
list3 = initialisation2();

//creation des threads
if(verbose)
    printf("creation des thread producer\n");
err=pthread_create(&thread1,NULL,producer,NULL);
if(err != 0){
    
}
if(verbose)
    printf("fin creation trhead producer\n");
int ab;
if(verbose)
    printf("debut trhead consumer\n");

for(ab = 0;ab<nthread;ab++){
    err=pthread_create(&(thread2[ab]),NULL,consumer,NULL);
if(err != 0){
    
}
}
if(verbose)
    printf("fin creation trhead consumer\n");
if(verbose)
    printf("creation trhead consumer2\n");
//sleep(10);
sleep(2);
err=pthread_create(&thread3,NULL,consumer2,NULL);
if(err != 0){
    
}
if(verbose)
    printf("fin creation trhead consumer2\n");
//joingage des threads
if(verbose)
    printf("creation pthread_join 1\n");
err=pthread_join(thread1,NULL);
if(verbose)
    printf("fin creation pthread_join \n");
if(verbose)
    printf("creation pthread_join 2\n");

for(ab = 0;ab<nthread;ab++){
    err = pthread_join(thread2[ab],NULL);
    if(err!=0){
        
    }
}

fini2=true;
if(verbose)
    printf("fin creation pthread_join 2\n");
if(verbose)
    printf("fin creation pthread_join 3\n");
err=pthread_join(thread3,NULL);
if(err!=0){
    printf("erreur join3\n");
}
if(verbose)
    printf("fin creation pthread_join3\n");
if(verbose)
    printf("ecriture console/fichier \n");

if(!fichierout){
    outconsole(list3);
}

else{
    // a écrire
}


    sem_destroy(&full);
    sem_destroy(&full2);
    sem_destroy(&empty);
    sem_destroy(&empty2);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex2);
    delete2(list1);
    delete(list2);
    delete(list3);
    free(list1->premier);
    free(list2->premier);
    free(list3->premier->trad);
    free(list3->premier);

    free(list1);
    free(list2);
    free(list3);

if(verbose)
    printf("fin");
}
