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
int verbose;
int contadorHash;
int indiceGlobal;
int buscarMayor();
int contadorHash=0;
int flagescritura=0;
int flagcarga=1;
int buscarIndice(char * buf);
char *m[4][100];
void *recargarMatriz(void *vargp);
void atender_cliente(int connfd,int* j);
sem_t mutex;
sem_t mmutex;
void *thread(void *vargp);
void *thread2(void *vargp);
char **separar_mensaje(char *buf);
void atender_cliente2(int connfd, int *j);
int revisarTopico(char ***m);
int revisarEspacioTopico(char ***m);
sbuf_t sbuf;
char **separar_topicos(char *buf);
struct fd_matriz{
	int fd[2];
	char ***matrizTopicos;
	sem_t sem;
	
};
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

char *str;
char *str2;
char *str5;
char *str6;
void crearDocumento(){
	str2=malloc(strlen(str)+4);
	strcat(str2,"log/");
	strcat(str2,str);
	char * str6=strdup(str2);
	int cont=strlen(str2);
	str6[(cont-1)-4]='p';
	str6[(cont-1)-6]='t';
	//printf("%s",str6);
	
	
	
	
	if(access(str2,F_OK)!=0){
			
			FILE *f=fopen(str2,"w");
			//FILE *f1=fopen(str3,"w");
			fclose(f);
			//fclose(f1);
	}
	if(access(str6,F_OK)!=0){
			
			FILE *f=fopen(str6,"w");
			
			fclose(f);
			
	}
	
		
}
void leerDocumento();
void leerDocumento(){

int pid=fork();
	if(pid==0){
		
		sleep(1);
		
	
		
		
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
	str5=malloc(sizeof(char)*sizec);
	//char str[sizec+7];
	int n=0;
	for(int i=0;i<3;i++){
		int length = snprintf(NULL,0,"%d",dia[i]);
		
		char *str2=malloc(length+1);
		snprintf(str2,length+1,"%d",+dia[i]);
		
		for(int j=0;j<strlen(str2);j++){
			str[j+n]=str2[j];
			str5[j+n]=str2[j];
		}
		
		
		
		
		n=n+length;
		//strcat(str,str2);
		free(str2);
		
	}
	
	strcat(str,"log.txt");
	strcat(str5,"top.txt");
	return;

	
	
	
	
}
struct stat st={0}; 
int main(int argc, char **argv)
{	
	if(stat("log",&st)==-1){
		mkdir("log",0700);
	}
	if(stat("topico",&st)==-1){
		mkdir("topico",0700);
	}
	if(access("topico/topico.txt",F_OK)!=0){
			
			FILE *f=fopen("topico/topico.txt","w");
			//FILE *f1=fopen(str3,"w");
			fclose(f);
			//fclose(f1);
		}
	int log=0;
	verbose=0;
	
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
	int listenfd;
	unsigned int clientlen;
	//Direcciones y puertos
	struct sockaddr_in clientaddr;
	
	char  *port;

	while ((opt = getopt (argc, argv, "hlv")) != -1){
		switch(opt)
		{
			case 'h':
				print_help(argv[0]);
				return 0;
			case 'v':
				verbose=1;
				break;
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
	
	
	
	
	

	
	
	
	
	//Registra funcion para recoger hijos zombies
	signal(SIGCHLD, recoger_hijos);
	/*
	struct fd_matriz objeto;
	objeto.matrizTopicos=malloc(sizeof(void)*2);
	objeto.matrizTopicos[0]=malloc(sizeof(void)*50);
	objeto.matrizTopicos[1]=malloc(sizeof(void)*50);
	
	
		for(int j=0;j<2;j++){
		
			for(int i=0;i<50;i++){
				objeto.matrizTopicos[j][i]=(char*)calloc(500,1);
				objeto.matrizTopicos[j][i]="";
			}
			
		}
		//objeto.matrizTopicos[0][0]="hola";*/
	
	
	sem_init(&mutex,0,1);
	sem_init(&mmutex,0,1);
	
	
	signal(SIGCHLD, recoger_hijos);
	int fd[2];

	if(pipe(fd)<0){
		printf("ERROR CON PIPE");
		exit(1);
	}
	if(fcntl(fd[0],F_SETFL,O_NONBLOCK)<0){
		printf("ERROR CON FCNTL");
		exit(2);
	}
	//proceso donde se escucharan a los publishers
	if(fork()==0){
	
	
	
		
		
		 port="8081";

		//Valida el puerto
		int port_n = atoi(port);
		if(port_n <= 0 || port_n > USHRT_MAX){
			fprintf(stderr, "Puerto: %s invalido. Ingrese un número entre 1 y %d.\n", port, USHRT_MAX);
			return 1;
		}
		
		
		listenfd = open_listenfd(port);

		if(listenfd < 0){
			connection_error(listenfd);
			}
		
			printf("\nserver escuchando en puerto %s...\n", port);
		
		
		
		
		pthread_t tid;
		sbuf_init(&sbuf,11);

		for(int i=0; i<10;i++){
			pthread_create(&tid,NULL,thread,fd);	
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
	else{
	
	

		if(log){
		leerDocumento( );
	}
	
	
	
	//sleep(10);
	
	

	
	
	

	
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

	
			printf("\nserver escuchando en puerto %s...\n", port);
	
	
	/*
	while (1) {
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);

		printf("hola");
		//El proceso hijo atiende al cliente
		if(fork() == 0){
			close(listenfd);*/

			/* Determine the domain name and IP address of the client */
			/*
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
	}*/
	
		pthread_t tid;
		sbuf_init(&sbuf,11);
	
			
		
		for(int i=0; i<10;i++){
			pthread_create(&tid,NULL,thread2,fd);	
		}
		pthread_create(&tid,NULL,recargarMatriz,fd);
		int connfd;
		clientlen = sizeof(clientaddr);
		while (1) {
			
			connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
					sbuf_insert(&sbuf,connfd);	


		//	pthread_create(&tid,NULL,thread,connfd_ptr);
		}
		close(connfd);
	}
	
	
}
void *thread(void *vargp){
	

	//Entra a estado detach antes de atender al cliente
	int  *j=(int *)vargp;
	
	pthread_detach(pthread_self());
	
	
	

	//Atiende al cliente
	while(1){
		
	int connfd=sbuf_remove(&sbuf);
		atender_cliente(connfd,j);
		
		close(connfd);
	}
	
	
		
	
}
void *thread2(void *vargp){
	

	//Entra a estado detach antes de atender al cliente
	int *j=(int *)vargp;

		
	
	
	pthread_detach(pthread_self());
	

	//Atiende al cliente
	while(1){
	int connfd=sbuf_remove(&sbuf);
		atender_cliente2(connfd,j);
		
		
		close(connfd);
	}
	
	
		
	
}
void *recargarMatriz(void *vargp){

	//m[3][100];
	//m[0]=calloc(sizeof(char**));
	//m[0]=calloc(sizeof(char**));
	//m[0]=calloc(sizeof(char**));
	pthread_detach(pthread_self());
	int *j=(int *)vargp;
	for(int i=0;i<100;i++){
		for(int j=0;j<4;j++){
			m[j][i]=calloc(500,sizeof(char));
			m[j][i]="";
		}
	
	}
	//m[0][0]="hola";
	
	
	
	FILE *fptr=fopen("topico/topico.txt","r+");
	int contadorval=0;
	char line[256];
	while(fgets(line,sizeof(line),fptr)){
			
			char **linea=separar_mensaje(line);
			
			//int length = atoi(linea[0]);
			char * topico=strdup(linea[2]);
			char * mensaje=strdup(linea[3]);
			//printf("h:%d\n",length);
			
			if(contadorval<100){
				m[0][contadorval]=strdup(linea[0]);
				m[1][contadorval]=strdup(linea[1]);
				m[2][contadorval]=strdup(linea[2]);
				m[3][contadorval]=strdup(linea[3]);
			
			}
			//printf("PRUEBA:%s %s",m[2][contadorval],m[3][contadorval]);
			//int l =snprintf(NULL,0,"%d",length);
			//char * valor=malloc(l+1);
			//snprintf(valor,l+1,"%d",length);
			//printf("%s %s %s",linea[0],linea[1],linea[2]);
			
			
			
			free(linea);
			free(topico);
			free(mensaje);
			contadorval++;
			//analizar la linea 
			// arreglar la linea
			// escribir sobre la linea
		}
		fclose(fptr);
		
	while(1){
		sem_wait(&mmutex);
		
		char f[2];
		
		int nread=read(j[0],&f,sizeof(f));
		
		 if(nread==2){
		 contadorHash++;
		 	
		 	FILE *fptr=fopen("topico/topico.txt","r+");
	int contadorval=0;
	char line[256];
	while(fgets(line,sizeof(line),fptr)){
			
			char **linea=separar_mensaje(line);
			
			//int length = atoi(linea[0]);
			char * topico=strdup(linea[1]);
			char * mensaje=strdup(linea[2]);
			//printf("h:%d\n",length);
			
			
			if(contadorval<100){
				m[0][contadorval]=strdup(linea[0]);
				m[1][contadorval]=strdup(linea[1]);
				m[2][contadorval]=strdup(linea[2]);
				m[3][contadorval]=strdup(linea[3]);
			
			}
			
			//int l =snprintf(NULL,0,"%d",length);
			//char * valor=malloc(l+1);
			//snprintf(valor,l+1,"%d",length);
			//printf("%s %s %s",linea[0],linea[1],linea[2]);
			
			
			
			free(linea);
			free(topico);
			free(mensaje);
			contadorval++;
			//analizar la linea 
			// arreglar la linea
			// escribir sobre la linea
		}
		fclose(fptr);
		
			
		}
		sem_post(&mmutex);
	}
		
	
	
}
void atender_cliente2(int connfd,int *j)
{

	/*
	char b[2];
	int t=read(j[0],b,sizeof(b));
	printf("JJJJ%s y %d\n",b,t);*/
	int n;
	char buf[MAXLINE] = {0};
	
	//int flag=0;
	n = read(connfd, buf, MAXLINE);
	
	FILE *flog=fopen(str2,"a+");
		if(flog==NULL){
			printf("ERROR LOG");
			exit(1);
		}
		fprintf(flog,"Suscriber connectado, connfd:%d\n",connfd);
		fclose(flog);
		if(verbose==1){
			printf("Suscriber connectado, connfd:%d\n",connfd);
		}
		
		if(n <= 0){
			return;
			}
	
	//int r=1;
	//close(j[1]);
	int contadorTopico=0;
	int contadorTopicoHash=0;
	int indice=-1;
	buf[strlen(buf)-1]='\0';
	while(1){
		if(strcmp(buf, "CHAO\n") == 0){
			write(connfd, "BYE\n", 4);
			FILE *flog=fopen(str2,"a+");
			if(flog==NULL){
				printf("ERROR LOG");
				exit(1);
			}
			fprintf(flog,"Mensaje 'CHAO' enviado a Suscriber");
			fprintf(flog,"Suscriber desconectado, connfd:%d\n",connfd);
			fclose(flog);
			return;
		}
		sem_wait(&mmutex);
		if(strcmp(buf,"#")==0){
				if(contadorTopicoHash<contadorHash){
					indice=buscarMayor();
					write(connfd, m[3][indice], strlen(m[3][indiceGlobal]));
					contadorTopicoHash=contadorHash;
						FILE *flog=fopen(str2,"a+");
						if(flog==NULL){
							printf("ERROR LOG");
							exit(1);
						}
						fprintf(flog,"Mensaje enviado a Suscriber:%s",m[3][indice]);
						fprintf(flog,"Suscriber desconectado, connfd:%d\n",connfd);
						fclose(flog);
				}
			
			
		}
		else{
			if(contadorTopico==0){
			
			indice=buscarIndice(buf);
			}
			if(indice!=-1){
				int length = atoi(m[0][indice]);
				if(contadorTopico<length){
					if(indice!=-1){
						write(connfd, m[3][indice], strlen(m[3][indice]));
					contadorTopico=length;
						
						FILE *flog=fopen(str2,"a+");
						if(flog==NULL){
							printf("ERROR LOG");
							exit(1);
						}
						fprintf(flog,"Mensaje enviado a Suscriber:%s",m[3][indice]);
						fprintf(flog,"Suscriber desconectado, connfd:%d\n",connfd);
						fclose(flog);
					}
					
				}
				
			}
		}
		
		
		sem_post(&mmutex);
		
		
		/*
		else if(nread==0){
		
		}
		else if(nread==1){
			printf("ENTRE CON PIPE YEY");
			if(r==1){
				printf("ENTRE CON PIPE YEY");
				r=0;
			}
		
		}*/
		
		//Detecta "CHAO" y se desconecta del cliente
		

		//Reenvía el mensaje al cliente
		

		//memset(buf, 0, MAXLINE); 
		//Encera el buffer
	}
}

void atender_cliente(int connfd,int * j)
{
	int n;
	char buf[MAXLINE] = {0};
	//int * j=(int *)vargp;
	
	//close(j[0]);
	
	
	
	while(1){
	
		n = read(connfd, buf, MAXLINE);
		FILE *flog=fopen(str2,"a+");
		if(flog==NULL){
			printf("ERROR LOG");
			exit(1);
		}
		fprintf(flog,"Publisher connectado, connfd:%d\n",connfd);
		if(verbose==1){
			printf("Publisher connectado, connfd:%d\n",connfd);
		}
		if(n <= 0)
			return;
		
		
		if(verbose){
		printf("Recibido: %s", buf);
		}
		//Detecta "CHAO" y se desconecta del cliente
		if(strcmp(buf, "CHAO\n") == 0){
			write(connfd, "BYE\n", 4);
			return;
		}

		//Reenvía el mensaje al cliente
		n = 0;
		
		char **c=separar_mensaje(buf);
		sem_wait(&mutex);
		//printf("PRUEBA %s\n",str2);
		
		fprintf(flog,"Topico %s ingresado con mensaje: %s",c[0],c[1]);
		if(verbose==1){
		printf("Topico %s ingresado con mensaje: %s",c[0],c[1]);
		}
		
		FILE *fptr=fopen("topico/topico.txt","r+");
		FILE *fptr2=fopen("topico/topicotemp.txt","w");
		char line[256];
		if(fptr==NULL){
			printf("ERROR");
			exit(1);
			
		}
		int encontro=0;
		//int r=1;
		
		int contador=0;
		while(fgets(line,sizeof(line),fptr)){
			
			char **linea=separar_mensaje(line);
			
			int length = atoi(linea[0]);
			//int indice = atoi(linea[1]);
			char * topico=strdup(linea[2]);
			char * mensaje=strdup(linea[3]);
			//printf("h:%d\n",length);
			
			if(strcmp(linea[2],c[0])==0 ){
				length=length+1;
				encontro=1;
				fprintf(fptr2,"%d %d %s %s",length,contador,topico,c[1]);
			}
			else{
				fprintf(fptr2,"%d %d %s %s",length,-1,topico,mensaje);
			}
			//int l =snprintf(NULL,0,"%d",length);
			//char * valor=malloc(l+1);
			//snprintf(valor,l+1,"%d",length);
			//printf("%s %s %s",linea[0],linea[1],linea[2]);
			
			
			
			free(linea);
			free(topico);
			free(mensaje);
			
			//analizar la linea 
			// arreglar la linea
			// escribir sobre la linea
			contador++;
		}
		if(encontro==0){
			fprintf(fptr2,"%d %d %s %s",1,contador,c[0],c[1]);
		} 
		
		
		fclose(fptr);
		fclose(fptr2);
		//printf("SALI WHILE");
		 fptr=fopen("topico/topico.txt","w");
		 fptr2=fopen("topico/topicotemp.txt","r");
		
		if(fptr2==NULL){
			printf("ERROR");
			exit(1);
		}
		while(fgets(line,sizeof(line),fptr2)){
			//printf("h:%s\n",line);
			char **linea=separar_mensaje(line);
			int length = atoi(linea[0]);
			int p = atoi(linea[1]);
			char * topico=strdup(linea[2]);
			char * mensaje=strdup(linea[3]);
			//printf("%s %s %s",linea[0],linea[1],linea[2]);
			fprintf(fptr,"%d %d %s %s",length,p,topico,mensaje);
			free(linea);
			free(topico);
			free(mensaje);
			//analizar la linea 
			// arreglar la linea
			// escribir sobre la linea
		} 
		
		
		fclose(fptr);
		fclose(fptr2);
		
		char k[2]={'1','\n'};
		write(j[1],&k,sizeof(k));
	
		
		
		sem_post(&mutex);
		
		fprintf(flog,"Publisher desconectado, connfd:%d\n",connfd);
		if(verbose==1){
		printf("Publisher desconectado, connfd:%d\n",connfd);
		}
		fclose(flog);
		
		
		if(n <= 0)
			return;

		memset(buf, 0, MAXLINE); 
		
		return;//Encera el buffer
	}
}

char **separar_mensaje(char *buf){
	char ** topmen;
		topmen=malloc((4+1)*sizeof(char*));
	
		
		for(int  j=0;j<4;j++){
      		topmen[j] =malloc(500);
		}
		
		separar_procesos(buf," ",topmen);
		
		
		return topmen;
}


char **separar_topicos(char *buf){
	char ** topmen;
		topmen=malloc((2+1)*sizeof(char*));
	
		
		for(int  j=0;j<2;j++){
      		topmen[j] =malloc(500);
		}
		
		separar_procesos(buf," ",topmen);
		
		
		return topmen;
}

int revisarTopico(char ***m){
	
		
		/*
			for(int i=0;i<50;i++){
			
				if(strcmp(m[0][i],topico)){
					return i;
				}
			}*/
			
		
		return -1;
}
int revisarEspacioTopico(char ***m){
	
		
			for(int i=0;i<50;i++){
				if(strcmp(m[0][i],"")==0){
					return i;
				}
			}
			
		
		return -1;
}
int buscarIndice(char * buf){
	
	
	
	
	for(int i=0;i<100;i++){
		
		if(strcmp(m[2][i],buf)==0){
			
			return i;
		}
	}
	return -1;
}
int buscarMayor(){
	
	
	
	
	for(int i=0;i<100;i++){
		
		if(strcmp(m[1][i],"-1")!=0){
			
			return i;
		}
	}
	return -1;
}



