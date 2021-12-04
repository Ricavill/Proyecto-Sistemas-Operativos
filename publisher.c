#include <getopt.h>
#include <limits.h>

#include "common.h"

void print_help(char *command)
{
	printf("Cliente ejemplo aplicación eco.\n");
	printf("uso:\n %s <hostname> <puerto> <topico><valor>\n", command);
	printf(" %s -h\n", command);
	printf("Opciones:\n");
	printf(" -h\t\t\tAyuda, muestra este mensaje\n");
}

int main(int argc, char **argv)
{
	int opt;

	//Socket
	int clientfd;
	//Direcciones y puertos
	char *hostname, *port,*topico,*valor;

	//Lectura desde consola
	char *linea_consola;
	char read_buffer[MAXLINE] = {0};
	size_t max = MAXLINE;
	ssize_t n, l = 0;

	while ((opt = getopt (argc, argv, "htv")) != -1){
		switch(opt)
		{
			case 'h':
				print_help(argv[0]);
				return 0;
			default:
				fprintf(stderr, "uso: %s <hostname> <puerto> <topico> <valor>\n", argv[0]);
				fprintf(stderr, "     %s -h\n", argv[0]);
				return 1;
		}
	}

	if(argc != 5){
		fprintf(stderr, "uso: %s <hostname> <puerto> <topico> <valor>\n", argv[0]);
		fprintf(stderr, "     %s -h\n", argv[0]);
		return 1;
	}else{
		hostname = argv[1];
		port = argv[2];
		
		topico = argv[3];
		valor = argv[4];
		//printf("%s\n",topico);
		//printf("%s\n",valor);
		if(topico==NULL){
			printf("No se ingreso topico");
			return 0;
		}
		
		if(valor==NULL){
			printf("No se ingreso valor");
			return 0;
		}
	}

	//Valida el puerto
	int port_n = atoi(port);
	if(port_n <= 0 || port_n > USHRT_MAX){
		fprintf(stderr, "Puerto: %s invalido. Ingrese un número entre 1 y %d.\n", port, USHRT_MAX);
		return 1;
	}

	//Se conecta al servidor retornando un socket conectado
	clientfd = open_clientfd(hostname, port);

	if(clientfd < 0)
		connection_error(clientfd);

	printf("Conectado exitosamente a %s en el puerto %s.\n", hostname, port);

	linea_consola = (char *) calloc(1, MAXLINE);
	int st=strlen(topico)+strlen(valor);
	char r[st+2];
	snprintf(r,st+2,"%s %s",topico,valor);
	
	printf("> ");
	l = getline(&linea_consola, &max, stdin);
	int iproc=contar_procesos(linea_consola," ");
	printf("%d\n",iproc);
	if(iproc!=2 ){
		printf("No ingreso bien el topico junto con el mensaje\n");
		return 0;
	} //lee desde consola
	while(l > 0){
		n = write(clientfd, linea_consola, l);  //Envia al servidor
		if(n<=0){
			printf("Error");
		}
			

		n = read(clientfd, read_buffer, MAXLINE); //Lee respuesta del servidor
		if(n<=0)
			break;

		printf("Recibido: %s", read_buffer);
		memset(read_buffer,0,MAXLINE); //Encerar el buffer

		//Volver a leer desde consola
		printf("> ");
		l=-1;
		//l = getline(&linea_consola, &max, stdin);
	}


	printf("Desconectando...\n");
	free(linea_consola);
	close(clientfd);

	return 0;
}
