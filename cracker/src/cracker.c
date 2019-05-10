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

// TOUTE LES CONSTANTES

int nmbfiles;
char **fichier;
bool fini;
bool fini2;

bool isconsonne;
int nthread;
int nmb;
int nbmarg;
bool verbose=false;
int test;

//ARRAYLIST BUFFER 1 (list1), qui va stocker les hashes



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

Liste *initialisation() // initialisation de l'array list
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


void insertion(Liste *liste, uint8_t *value) //methode permettant d'inserer un nouveau hash dans l'array list
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

void suppression(Liste *liste) //methode permettant de supprimer un hash dans l'array list
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

void delete2(Liste *liste1){ //permet de supprimer tout les éléments de l'array list sauf le dernier (celui qui a été initialisé par initialisation)
    while(liste1->premier->suivant != NULL){
        
        suppression(liste1);
    }
}

//ARRAYLIST BUFFER 2 (list2), buffer contenant les traductions des hash

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

Liste2 *initialisation2() // initialisation de l'array list
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


void insertion2(Liste2 *liste2, char* trad) //methode permettant d'inserer une nouvelle traduction dans l'array list
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

void suppression2(Liste2 *liste2) //methode permettant de supprimer une traduction dans l'array list
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

//permet de supprimer tout les éléments de l'array list sauf le dernier (celui qui a été initialisé par initialisation)
void delete(Liste2 *liste2){
    while(liste2->premier->suivant != NULL){
        free(liste2->premier->trad);
        suppression2(liste2);
    }
}

// fonction pour voyelle consonne
Liste *list1;
Liste2 *list2;
Liste2 *list3;

int voyelle(char chaine[16]) // cette fonction retourne le nombre de voyelle contenu de la chaine de caractere
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

int consonne(char chaine[16]) // cette fonction retourne le nombre de consonne contenu de la chaine de caractere
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

/*int copy(char *file_name, char *new_file_name) {
int fd = open(file_name, O_CREAT|O_RDWR);
if(fd==-1){
    return -1;
}
struct stat x;
int err = stat(file_name,&x);
if(err == -1){
    return -1;
}
chmod(new_file_name,x.st_mode);
int fdbis = open(new_file_name,O_CREAT|O_RDWR);
if(fdbis == -1){
    close(fd);
    return -1;
    
}


int taille = x.st_size;
char trans;
err = read(fd,(void *) &trans,taille);
if(err ==-1){
    return -1;
}
err=write(fdbis, (void *) &trans,err);
if(err==-1){
    return -1;
}

err = close(fd);
int err2 = close(fdbis);
if(err==-1||err2 ==-1){
    return -1;
}
return 0;
}
*/
// fonctin d'écriture sur console/fichier //cette fonction prend une array et un string et créer un fichier (str) et le rempli avec toute les traduction de l'array
void outfichier(Liste2 *list2, char *str){
    int fd = open(str,O_WRONLY|O_CREAT);
    if (fd==-1)
    {
        printf("ca ouvre pas\n");
    }

    while(list2->premier->suivant != NULL){
        char *a = list2->premier->trad;
        int ecrire = write(fd,a,strlen(a));

        if(ecrire == -1){
            printf("ça ecrit pas\n");
        }

        int ecrire2 = write(fd,"\n",sizeof(char));
            if(ecrire2 == -1){
                printf("dfjndjf\n");
            }
        
        suppression2(list2);
   
}
   int err = close(fd);
   if(err == -1){
    printf("hihi\n");
   }
    return; 
    
}



void outconsole(Liste2 *list2){  //print les traductions present dans la list2 
   
    while(list2->premier->suivant != NULL){
    
        
        printf("%s\n", list2->premier->trad);
        suppression2(list2);
   
}
}


 //Premier producteur

void * producer(void*arg){ //premier producteur qui va prendre les hash dans un fichier et les placé dans list1
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

void* consumer(void*arg){  // ce consommateur va prendre prendre les hashs de la list1, va les traduires et ls placé dans list 2
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
        test++;
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

void* consumer2(void*arg){  // on va regarder toute les traductions de la list 2 et placé celles qui ont le plus de voyelle ou de consonne dans list3

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
        nmb =acc;
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
nmb = 0; //nombre de consonne/voyelle
nmbfiles= 1; //nombre de fichier a traité
fichier = argv;
isconsonne = false;
nthread = 1;
char options;
bool isfichierout = false;
char* fichierout = NULL;
nbmarg = argc;
test = 0;

while((options=getopt(argc, argv,"t:co:v"))!= -1){
    switch(options){

        case 't' : 
        nthread = strtol(optarg,NULL,10);
        break;

        case 'c' :
        isconsonne = true;
        break;

        case 'o' : 
        isfichierout = true;
        fichierout = optarg;
        break;

        case 'v' :
        verbose = true; //permet d'enlever les print pour alléger la console
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

if(!isfichierout){
    outconsole(list3);
}
else{
   outfichier(list3, fichierout);
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
printf("%d",test);
if(verbose)
    printf("fin");
}
