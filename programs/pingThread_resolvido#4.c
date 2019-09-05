#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define IPFOR "150.162.218.%i"
#define BUFFERSZ	10
#define STRLEN		64
#define NCON		5

static char buffer[BUFFERSZ][STRLEN];
static int in, out;
static volatile int count;

static char format[] = "ping -c 1 -w 1 %s";
static char command[128];
	
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
sem_t prod;
sem_t cons;	


int ping(char *ip)
{
	char str[1024];
	FILE *fpipe;
	
	sprintf(command, format, ip);

	/* executa comando redirecionando a saida para o pipe. */
	if ( (fpipe = popen(command, "r"))<0){
		perror("popen");
	}
	
	/* Ignora primeira linha */
	/* PING 10.32.175.200 (10.32.175.200) 56(84) bytes of data. */
	fgets(str, 1023, fpipe);

	/* Verifica se teve sucesso no ping com a segunda linha */
	/* 64 bytes from 10.32.175.200: icmp_req=1 ttl=64 time=0.023 ms */
	fgets(str, 1023, fpipe);
	if(strcmp(str, "\n"))
		return 1;
	else
		return 0;

	fclose(fpipe);
}


void getFromBuf(char *str){
	sem_wait(&cons);
	pthread_mutex_lock(&m);	
	
	strncpy(str, buffer[out], STRLEN);
	out = (out + 1) % BUFFERSZ;
	count--;
	
	pthread_mutex_unlock(&m);
	sem_post(&prod);
}

void setToBuf(char *str){
	sem_wait(&prod);
	pthread_mutex_lock(&m);	
	
	strncpy(buffer[in], str, STRLEN);
	in = (in + 1) % BUFFERSZ;
	count++;
	pthread_mutex_unlock(&m);
	sem_post(&cons);
}

/* Thread Produtora */
void * producer(){
	char str[STRLEN];
	int i = 0;
	
	for(i=1; i<255; i++){
		sprintf(str, IPFOR, i);
		setToBuf(str);
	}
}

/* Thread Consumidora */
void * consumer(void * d){
	int id = *(int*)d;
	char str[STRLEN];
	int i = 0;
	
	while(1){
		getFromBuf(str);
		printf("Thread %i -> %s %s\n", id, str, ping(str)? "sucesso": "falha");
		fflush(stdout);
	}
}


int main(int argc, char **argv){
	pthread_t prod, *con;
	int *ids;
	int i, numthr;
	
	sem_init(&prod, 0, BUFFERSZ);
	sem_init(&cons, 0, 0);

	in = 0;
	out = 0;
	count = 0;
	memset(buffer, 0, sizeof(BUFFERSZ));
	
	if (argc < 2){
		printf("Uso: %s <insira o nmr de threads>\n", argv[0]);
		exit(0);
	}
	
	/*Cria array com os números de identificação das threads. */
	numthr = atoi(argv[1]);
	ids = (int *)malloc(numthr * sizeof(int));
	for(i=0; i<numthr; i++){
		ids[i] = i;
	}
	
	
	/* Cria uma thread produtora */
	if(pthread_create(&prod, NULL, producer, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

    /* Cria diversas threads consumidoras */
    con = (pthread_t *)malloc(numthr * sizeof(pthread_t));
	for(i=0; i<numthr; i++){
		if(pthread_create(&con[i], NULL, consumer, (void*)&ids[i])) {
			fprintf(stderr, "Error creating thread\n");
			return 2;
		}
	}

	/* Aguarda o término da thread produtora */
	if(pthread_join(prod, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 3;
	}

	/* Aguarda o esvaziamento do buffer. */
	while (count > 0){
		sleep(1);
	}

	return 0;
}