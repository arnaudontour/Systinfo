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
#include "sha256.h"
#include "reverse.h"

// TOUTE LES CONSTANTES

#define N 4 // SLOT DU PREMIER BUFFER
#define THREADS 4 //nombre de THREADS souhaite

//INITIALISATION DES MUTEX ET SEMAPHORE

pthread_mutex_t mutex;
pthread_mutex_t mutex2;
sem_t empty;
sem_t full;
sem_t empty2;
sem_t full2;
pthread_mutex_init(&mutex2,NULL);
pthread_mutex_init(&mutex, NULL);
sem_init (&empty,0,N);
sem_init(&full,0,0);
sem_init (&empty2,0,N);
sem_init(&full2,0,0);

//ARRAYLIST BUFFER 1 (32bytes)

typedef struct Element Element;
struct Element
{
    Element *suivant;
    uint8_t *hash[32];
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
    Element *nouveau = malloc(sizeof(*nouveau));
    if (liste == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }
    nouveau->hash = *value;

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
        free(aSupprimer);
    }
}

//ARRAYLIST BUFFER 2 (liste avec les traductions)

typedef struct Element2 Element2;
struct Element2
{
    Element2 *suivant;
    char trad[16];
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

    if (liste == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }

    element2->trad = NULL;
    element->suivant = NULL;
    liste2->premier = element;

    return liste2;
}


void insertion2(Liste2 *liste2, char[16] trad)
{
    /* Création du nouvel élément */
    Element2 *nouveau = malloc(sizeof(*nouveau));
    if (liste2 == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
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
        suppression2(list2);
    }
}


 //Premier producteur

void* producer(void*arg){
	while(true){
	sem_wait(&empty);
	pthread_mutex_lock(&mutex);


	uint8_t *hashe = malloc(sizeof(char)*32);
	if(rd!=0){
    
    rd = read(fd,hashe,sizeof(sizeof(char)*32));
    if(rd < 0){
       // return -1;
    }
    insertion(buffer1,hashe);
    free(hashe);
	}
	if(rd == 0){
		int cl=close(fd);
		if (cl < 0){
    	//return -1;
    	}
	}
	
	pthread_mutex_unlock(&mutex);
	sem_post(&full);
	}
}

// premier consommateur

void* consumer(void*arg){
	while(true){
	sem_wait(&full);
	pthread_mutex_lock(&mutex);
    char *x[16]; 
    reversehash(list1->premier->hash, x, sizeof(char*16));
    sem_wait(&empty2)
    pthread_mutex_lock(&mutex2);
    insertion2(liste2, x);
    suppression2(Liste2 *liste2);
    pthread_mutex_unlock(&mutex2);
    sem_post(&full);
	pthread_mutex_unlock(&mutex);
	sem_post(&empty);
	}
}

void* consumer2(void*){  // ICI ON VA METTRE QU'UN THREAD DU COUP BALLEC
sem_wait(&full);
pthread_mutex_lock(&mutex2);
char trans = list2->premier->trad;
pthread_mutex_unlock(&mutex2);
sem_post(&empty2);

int acc = 0;
if(consonne){
acc = consonne(trans);
    if(acc == nmb){
        insertion3(list3, trans);
    }
    if(acc > nmb){
        delete(list3);
        insertion3(list3,trans);
    }
    else{}
}
else{ 
   acc = voyelle(trans);
    if(acc == nmb){
        insertion3(list3, trans);
    }
    if(acc > nmb){
        delete(list3);
        insertion3(list3,trans);
    }
    else{}

}
}

int main (int argc, char *argv[]){

int nmb = 0;
int fd, rd;
fd = open("test.bin",O_RDONLY);
if(fd < 0){
    return -1;
}


}