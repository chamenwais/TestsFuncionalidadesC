/*
 ============================================================================
 Name        : TestAlarm.c
 Author      : faromero
 Version     :
 Copyright   : Your copyright notice
 Description : TestAlarm in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void configurar_signal_alarma(void);
void capturar_signal_alarma(int);

int main(void) {
	configurar_signal_alarma();
	puts("Arranco el test de la funcion 'alarm'");
	alarm(3);
	puts("En 3 segundos te voy a decir algo");
	sleep(3);
	alarm(4);
	puts("Ahora en 4 segundos te voy a decir algo mas");
	sleep(4);
	alarm(5);
	puts("Por ultimo, bancame 5'' que te necesito algo importante");
	sleep(5);

	return EXIT_SUCCESS;
}

void configurar_signal_alarma(void){
	struct sigaction signal_struct;
	signal_struct.sa_handler = capturar_signal_alarma;
	signal_struct.sa_flags = 0;

	sigemptyset(&signal_struct.sa_mask);

	sigaddset(&signal_struct.sa_mask,SIGALRM);
	if (sigaction(SIGALRM, &signal_struct, NULL) < 0) {
		puts(" SIGACTION error ");
	}
}

void capturar_signal_alarma(int signo){
	if(signo == SIGALRM){
		puts("¡ALGO!");
	}
}
