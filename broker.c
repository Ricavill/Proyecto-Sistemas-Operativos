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
#define SBUFSIZE 100
void atender_cliente(int connfd);

void *thread(void *vargp);
sbuf_t sbuf;
struct timeval current_time;
void print_help(char *command)
{
	printf("Servidor ejemplo apliación eco.\n");
	printf("uso:\n %s <puerto>\n", command);
	printf(" %s -h\n", command);
	fprintf(stderr, "%s -l para leer log del dia de hoy\n", command);
	printf("Opciones:\n");
	printf(" -h\t\t\tAyuda, muestra este mensaje\n");
}

/**
 * Recoge hijos zombies...
 */
void recoger_hijos(int signal){
	while(waitpid(-1, 0, WNOHANG) >0)
		;

	return;
}
int maxTopicos=50;
char *matrizTopicos[2][50];
char *str;
char *str2;
void crearDocumento();
void crearDocumento(){
	str2=malloc(strlen(str)+4);
	strcat(str2,"log/");
	strcat(str2,str);
	
	
	if(access(str2,F_OK)!=0){
			
			FILE *f=fopen(str2,"w");
			fclose(f);
	}
	
		
}
void leerDocumento();
void leerDocumento(){

int pid=fork();
	if(pid==0){
	
	
		
		
		char *args[]={"/bin/cat",str2,NULL};
		
		if(access(str2,F_OK)!=0){
			
		}
		else{
			execvp(args[0],args);	
			printf("No existe log diario");
		}
		
		
		
		exit(0);
	}
	
	
	
}
void diaActual(int dia[3]);


void diaActual(int dia[3]){
int sizec=0;

	for(int i=0;i<3;i++){
		int length = snprintf(NULL,0,"%d",dia[i]);
		
		sizec=length+sizec;
		
		
	}
	//printf("%d\n",sizec);
	str=malloc(sizeof(char)*sizec);
	//char str[sizec+7];
	int n=0;
	for(int i=0;i<3;i++){
		int length = snprintf(NULL,0,"%d",dia[i]);
		
		char *str2=malloc(length+1);
		snprintf(str2,length+1,"%d",+dia[i]);
		
		for(int j=0;j<strlen(str2);j++){
			str[j+n]=str2[j];
		}
		n=n+length;
		//strcat(str,str2);
		free(str2);
		
	}
	
	strcat(str,"log.txt");
	//printf("%s\n",str);
	
	
	
} 
int main(int argc, char **argv)
{
	int log=0;
	
	time_t t=time(NULL);
	struct tm tm=*localtime(&t);
	int dia[3]={tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday};
	diaActual( dia);
	int opt;
	/*
	matrizTopicos[1][0]="hola";
	for(int i=0;i<maxTopicos;i++){
		for(int j=0;j<2;j++){
		if(matrizTopicos[j][i]){
			printf("%s",matrizTopicos[j][i]);
		}
	
	}
	}*/
	//Sockets
	int listenfd, connfd;
	unsigned int clientlen;
	//Direcciones y puertos
	struct sockaddr_in clientaddr;
	struct hostent *hp;
	char *haddrp, *port;

	while ((opt = getopt (argc, argv, "hl")) != -1){
		switch(opt)
		{
			case 'h':
				print_help(argv[0]);
				return 0;
			case 'l':
				log=1;
				break;
			default:
				fprintf(stderr, "uso: %s <puerto>\n", argv[0]);
				fprintf(stderr, "     %s -l para leer log del dia de hoy\n", argv[0]);
				fprintf(stderr, "     %s -h\n", argv[0]);
				return 1;
		}
	}
	crearDocumento();
	if(log){
		leerDocumento( );
	}

	
	
	
	
	//Registra funcion para recoger hijos zombies
	signal(SIGCHLD, recoger_hijos);
	int fd[2];
	pipe(fd);
	int pidoriginal=fork();

	
	//proceso donde se escucharan a los publishers
	if(pidoriginal==0){
	
		signal(SIGCHLD, recoger_hijos);
		 port="8081";

		//Valida el puerto
		int port_n = atoi(port);
		if(port_n <= 0 || port_n > USHRT_MAX){
			fprintf(stderr, "Puerto: %s invalido. Ingrese un número entre 1 y %d.\n", port, USHRT_MAX);
			return 1;
		}
		printf("fff");
		
		listenfd = open_listenfd(port);

		if(listenfd < 0)
			connection_error(listenfd);
		printf("server escuchando en puerto %s...\n", port);
		
		
		pthread_t tid;
		sbuf_init(&sbuf,11);

		for(int i=0; i<10;i++){
			pthread_create(&tid,NULL,thread,NULL);	
		}
		int connfd;
		clientlen = sizeof(clientaddr);
		while (1) {
			
			connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
					sbuf_insert(&sbuf,connfd);	


		//	pthread_create(&tid,NULL,thread,connfd_ptr);
		}
		close(connfd);
		exit(0);
	
	
		
	
	
	
		
	}
	//Parte de suscriber
	 port="8080";

	//Valida el puerto
	int port_n = atoi(port);
	if(port_n <= 0 || port_n > USHRT_MAX){
		fprintf(stderr, "Puerto: %s invalido. Ingrese un número entre 1 y %d.\n", port, USHRT_MAX);
		return 1;
	}
	
	
	
	//Abre un socket de escucha en port
	listenfd = open_listenfd(port);

	if(listenfd < 0)
		connection_error(listenfd);

	printf("server escuchando en puerto %s...\n", port);

	while (1) {
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);

		printf("hola");
		//El proceso hijo atiende al cliente
		if(fork() == 0){
			close(listenfd);

			/* Determine the domain name and IP address of the client */
			hp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
						sizeof(clientaddr.sin_addr.s_addr), AF_INET);
			haddrp = inet_ntoa(clientaddr.sin_addr);

			printf("server conectado a %s (%s)\n", hp->h_name, haddrp);
			atender_cliente(connfd);
			printf("server desconectando a %s (%s)\n", hp->h_name, haddrp);

			close(connfd);

			exit(0);
		}

		close(connfd);
	}
}
void *thread(void *vargp){
	

	//Entra a estado detach antes de atender al cliente
	pthread_detach(pthread_self());

	//Atiende al cliente
	while(1){
	int connfd=sbuf_remove(&sbuf);
		atender_cliente(connfd);
		
		close(connfd);
	}
	
		
	
}

void atender_cliente(int connfd)
{
	int n;
	char buf[MAXLINE] = {0};

	while(1){
		n = read(connfd, buf, MAXLINE);
		if(n <= 0)
			return;

		printf("Recibido: %s", buf);

		//Detecta "CHAO" y se desconecta del cliente
		if(strcmp(buf, "CHAO\n") == 0){
			write(connfd, "BYE\n", 4);
			return;
		}

		//Reenvía el mensaje al cliente
		n = 0;
		if(n <= 0)
			return;

		memset(buf, 0, MAXLINE); 
		return;//Encera el buffer
	}
}


