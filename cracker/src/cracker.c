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
sem_t empty;
sem_t full;
pthread_mutex_init(&mutex, NULL);
sem_init (&empty,0,N-1);
sem_init(&full,0,0);

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

typedef struct Element1 Element1;
struct Element1
{
    Element1 *suivant;
    char trad[16];
};


typedef struct Liste1 Liste1;
struct Liste1
{
    Element1 *premier;
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

//ARRAYMIST BUFFER 3 (List avec X nombre de voyelle ou consonne)

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

void consumer(void){
	while(true){
	sem_wait(&full);
	pthread_mutex_lock(&mutex);



	pthread_mutex_unlock(&mutex);
	sem_post(&empty);
	}
}
 

int main (int argc, char *argv[]){

uint8_t *hashe = malloc(sizeof(char)*32);


int fd, rd;
fd = open("test.bin",O_RDONLY);
if(fd < 0){
    return -1;
}


}