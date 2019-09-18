

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h> 
#include <pthread.h>

char* recvBuff(int s, char *msg) {
int pos = 0;
while (1) {
 recv(s, &msg[pos], 1, 0);
 if (!msg[pos]){
  break;
 }
 pos++;
}
return msg;
}

void die(char *s) {
    perror(s);
    exit(1);
}

struct client_data{
    int sk;
    struct sockaddr_in *client_addr;
    
};

void * client_handle(void* cd){
    struct client_data *client = (struct client_data *)cd;
    char sendBuff[512];
    char recvBuff[512]; 
    char recvBuff1[512];
    
    
    //zera os buffers
    memset(sendBuff, 0, sizeof(sendBuff)); 
    memset(recvBuff, 0,sizeof(recvBuff));
    memset(recvBuff1, 0,sizeof(recvBuff1));

    //strcpy( &sendBuff[0],"Bem vindo" );
    //send(client->sk, sendBuff, strlen(sendBuff)+1, 0);
    
    //quando o cliente se conectar ao servidor, imprime as frases abaixo
    printf("Received connection from %s:%d\n", inet_ntoa(client->client_addr->sin_addr), ntohs(client->client_addr->sin_port));
    printf("Waiting for data...");
    fflush(stdout);
    
    //recebe o 1 dado do cliente
    recv(client->sk, recvBuff, sizeof(recvBuff), 0);
    
    printf(" \n Received packet from %s:%d\n", inet_ntoa(client->client_addr->sin_addr), ntohs(client->client_addr->sin_port));
 
   
    
    if(strcmp(recvBuff , "101") == 0){
	
	 printf("105");
	 strcpy(&sendBuff[0], "105");
	 send(client->sk, sendBuff, strlen(sendBuff), 0);
    
    }else  { 
      
       printf("Comando não encontrado");
    }	
	
    recv(client->sk, recvBuff1, 512, 0);
    puts(recvBuff1);
    
    close(client->sk);
    
    free(client->client_addr);
    free(client);

    return NULL;
}

int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr; 
    unsigned int addrlen;
    int s = 0;
    struct client_data *cd;
    pthread_t thr;

    s = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(s, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

	/* Inicia a escuta na porta */
    listen(s, 10); 

    while(1) {
    
    cd = (struct client_data *)malloc(sizeof(struct client_data));
    cd->client_addr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    addrlen = sizeof(struct sockaddr_in);

    cd->sk = accept(s, (struct sockaddr*)cd->client_addr, (socklen_t*)&addrlen); 

    pthread_create(&thr, NULL, client_handle, (void *)cd);
    pthread_detach(thr);
	
	
     }
}
