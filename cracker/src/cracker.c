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

// TOUTE LES CONSTANTES

#define N 4 // SLOT DU PREMIER BUFFER
#define THREADS 4 //nombre de THREADS souhaite


//INITIALISATION DES TABLEAU VARIABLE



int main (int argc, char *argv[]){


uint8_t hash[31]; // TABLEAU CONTENANT 32*BYTES
uint8_t buffer[N-1][31]; // BUFFER CONTENANT N * 32*BYTES
int err; // variable d'erreur lors de la création de thread

//INITIALISATION DE LA LECTURE DU FICHIER

int fd, rd;
fd = open(test.bin,O_RDONLY);
if(fd < 0){
    return -1;
}

//INITIALISATION DES THREADS

pthread_mutex_t mutex;
sem_t empty;
sem_t full;
pthread_mutex_init(&mutex, NULL);
sem_init (&empty,0,N-1);
sem_init(&full,0,0);

//PREMIERE FONCTION PRODUCTEUR(REMPLIR DE BUFFER DE HASH)

void producer(void){
	while(true){
	sem_wait(&empty);
	pthread_mutex_lock(&mutex);



	if(rd!=0){
    
    rd = read(fd,hash,sizeof(sizeof(char)*32));
    if(rd < 0){
       // return -1;
    }
    buffer[0][0] = hash;
	}
	if(rd == 0){
		int cl=close(fd);
		if (cl < 0){
    	//return -1;
    	}
	// ECRIRE ICI QUAND LE FICHIER EST VIDE
	}
	
	pthread_mutex_unlock(&mutex);
	sem_post(&full);
	}
}

//PREMIERE FONCTION CONSOMMATEUR(DEHASHER ET METTRE DANS UN AUTRE FICHIER)
void consumer(void){
	while(true){
	sem_wait(&full);
	pthread_mutex_lock(&mutex);


	pthread_mutex_unlock(&mutex);
	sem_post(&empty);
	}
} 


}