#include <getopt.h>
#include <limits.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include "common.h"
#include <sys/resource.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
int c;


void print_help(char *command)
{
	printf("Cliente ejemplo aplicación eco.\n");
	printf("uso:\n %s <hostname>\n", command);
	printf(" %s -h\n", command);
	printf("Opciones:\n");
	printf(" -h\t\t\tAyuda, muestra este mensaje\n");
}
void recoger_hijos(int signal){
	char buf[MAXLINE] = {0};
	buf[0]='C';
	buf[1]='H';
	buf[2]='A';
	buf[3]='O';
	buf[3]='\n';
	
	write(c, buf, strlen(buf)+1);
	printf("\n");
	exit(0);
	return;
}

int main(int argc, char **argv)
{
	int opt;

	//Socket
	int clientfd;
	//Direcciones y puertos
	char *hostname, *port;

	//Lectura desde consola
	char *linea_consola;
	char read_buffer[MAXLINE] = {0};
	size_t max = MAXLINE;
	ssize_t n, l = 0;

	while ((opt = getopt (argc, argv, "h")) != -1){
		switch(opt)
		{
			case 'h':
				print_help(argv[0]);
				return 0;
			default:
				fprintf(stderr, "uso: %s <hostname> \n", argv[0]);
				fprintf(stderr, "     %s -h\n", argv[0]);
				return 1;
		}
	}

	if(argc != 2){
		fprintf(stderr, "uso: %s <hostname> \n", argv[0]);
		fprintf(stderr, "     %s -h\n", argv[0]);
		return 1;
	}else{
		hostname = argv[1];
		port = "8080";
	}

	//Valida el puerto
	int port_n = atoi(port);
	if(port_n <= 0 || port_n > USHRT_MAX){
		fprintf(stderr, "Puerto: %s invalido. Ingrese un número entre 1 y %d.\n", port, USHRT_MAX);
		return 1;
	}

	//Se conecta al servidor retornando un socket conectado
	clientfd = open_clientfd(hostname, port);
	c=clientfd;

	if(clientfd < 0)
		connection_error(clientfd);

	printf("Conectado exitosamente a %s en el puerto %s.\n", hostname, port);
	ssignal(SIGINT, recoger_hijos);
	linea_consola = (char *) calloc(1, MAXLINE);
	printf("Ingrese texto para enviar al servidor, escriba CHAO para terminar...\n");
	printf("> ");
	l = getline(&linea_consola, &max, stdin);
	n = write(clientfd, linea_consola, l); //lee desde consola
	while(l > 0){
		 //Envia al servidor
		if(n<=0)
			break;

		n = read(clientfd, read_buffer, MAXLINE); //Lee respuesta del servidor
		if(n<=0)
			break;

		printf("Recibido: %s", read_buffer);
		memset(read_buffer,0,MAXLINE); //Encerar el buffer

		//Volver a leer desde consola
		printf("> ");
		
	}


	printf("Desconectando...\n");
	free(linea_consola);
	close(clientfd);

	return 0;
}
