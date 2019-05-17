/*
 ============================================================================
 Name        : EquipoGuardiola.c
 Author      : faromero
 Version     :
 Copyright   : Your copyright notice
 Description : Semaphores example
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <pthread.h>
#include <semaphore.h>

void jugador_izquierda();
void jugador_centro();
void jugador_derecha();
void delantero();

void crear_hilos(pthread_t* hilo_j_izq, pthread_t* hilo_j_cen,
		pthread_t* hilo_j_der, pthread_t* hilo_del);

void joinear_hilos(pthread_t* hilo_j_izq, pthread_t* hilo_j_cen,
		pthread_t* hilo_j_der, pthread_t* hilo_del);

char* PELOTA;

pthread_mutex_t M_PELOTA;
sem_t CENTRO;
sem_t IZQ;
sem_t DER;
sem_t PASES;
sem_t JUGADA;

pthread_t hilo_j_izq, hilo_j_der, hilo_j_cen, hilo_del;

int main(void) {
	PELOTA=string_new();
	pthread_mutex_init(&M_PELOTA, NULL);
	sem_init(&CENTRO,0,1);
	sem_init(&IZQ,0,0);
	sem_init(&DER,0,0);
	sem_init(&PASES,0,0);
	sem_init(&JUGADA,0,30);
	printf("Arranca la jugada: \n");
	crear_hilos(&hilo_j_izq, &hilo_j_cen, &hilo_j_der, &hilo_del);
	joinear_hilos(&hilo_j_izq, &hilo_j_cen, &hilo_j_der, &hilo_del);
	printf("Terminó la jugada...\n");
	free(PELOTA);
	pthread_mutex_destroy(&M_PELOTA);
	sem_destroy(&CENTRO);
	sem_destroy(&IZQ);
	sem_destroy(&DER);
	sem_destroy(&PASES);
	sem_destroy(&JUGADA);
	return EXIT_SUCCESS;
}

void crear_hilos(pthread_t* hilo_j_izq, pthread_t* hilo_j_cen,
		pthread_t* hilo_j_der, pthread_t* hilo_del) {
	pthread_create(hilo_j_izq, NULL, (void*)jugador_izquierda, NULL);
	pthread_create(hilo_j_cen, NULL, (void*)jugador_centro, NULL);
	pthread_create(hilo_j_der, NULL, (void*)jugador_derecha, NULL);
	pthread_create(hilo_del, NULL, (void*)delantero, NULL);
}

void joinear_hilos(pthread_t* hilo_j_izq, pthread_t* hilo_j_cen,
		pthread_t* hilo_j_der, pthread_t* hilo_del) {
	pthread_join(*hilo_j_izq, NULL);
	pthread_join(*hilo_j_cen, NULL);
	pthread_join(*hilo_j_der, NULL);
	pthread_join(*hilo_del, NULL);
}

void pasarPelotaAlCentro() {
	printf("La pelota pasa al centro\n");
}

void jugador_izquierda(){
	while(1){
		sem_wait(&IZQ);
		pasarPelotaAlCentro();
		pthread_mutex_lock(&M_PELOTA);
		PELOTA="izq";
		pthread_mutex_unlock(&M_PELOTA);
		sem_post(&CENTRO);
		sem_post(&PASES);
	}
}

void jugador_centro(){
	while(1){
		sem_wait(&JUGADA);
		sem_wait(&CENTRO);
		pthread_mutex_lock(&M_PELOTA);
		if(string_equals_ignore_case(PELOTA,"der")){
			printf("La pelota pasa a la izquierda\n");
			sem_post(&IZQ);
		} else {
			printf("La pelota pasa a la derecha\n");
			sem_post(&DER);
		}
		pthread_mutex_unlock(&M_PELOTA);
		sem_post(&PASES);
	}
}

void jugador_derecha(){
	while(1){
		sem_wait(&DER);
		pasarPelotaAlCentro();
		pthread_mutex_lock(&M_PELOTA);
		PELOTA="der";
		pthread_mutex_unlock(&M_PELOTA);
		sem_post(&CENTRO);
		sem_post(&PASES);
	}
}

void delantero(){
	while(1){
		for(int pases=0;pases<30;pases++){
			sem_wait(&PASES);
		}
		printf("El delantero busca la pelota\n");
		printf("¡¡El delantero patea al arco!!\n");
		sem_init(&JUGADA,0,30);
	}
}
