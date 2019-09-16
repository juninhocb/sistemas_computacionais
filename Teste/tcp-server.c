

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h> 

void die(char *s) {
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr; 
    struct sockaddr_in client_addr;
    unsigned int addrlen = sizeof(client_addr);
    int s = 0, connfd = 0;
    char recvBuff[512]; 
    char sendBuff[512];

    /* Cria o Socket: SOCK_STREAM = TCP */
    s = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(sendBuff, 0, sizeof(sendBuff));
    memset(recvBuff, 0,sizeof(recvBuff)); 

	/* Configura servidor para receber conexoes de qualquer endereço:
	 * INADDR_ANY e ouvir na porta 5000 */ 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

	/* Associa o socket a estrutura sockaddr_in */
    bind(s, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

	/* Inicia a escuta na porta */
    listen(s, 10); 

    while(1) {
		/* Aguarda a conexão */	
        connfd = accept(s, (struct sockaddr*)&client_addr, &addrlen); 
	strcpy( &sendBuff[0],"Bem vindo" );
	send(connfd, sendBuff, strlen(sendBuff)+1, 0);

		/* Imprime IP e porta do cliente. */
        printf("Received connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	printf("Waiting for data...");
        fflush(stdout);
        
	 if (recv(connfd, recvBuff, 512, 0) == -1){
           die("recv()");
        }
	
	printf("Received packet from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Data: %s\n" , recvBuff);
	
	
	if (send(connfd, recvBuff, strlen(recvBuff), 0) == -1){             
           die("send()");        
        }
	
	puts(recvBuff);
}
     close(s);
     return 0;
}
