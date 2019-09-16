

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

void die(char *s){
    perror(s);
    exit(1);
}
 

int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr; 
    int s = 0;
    char recvBuff[512];
    char sendBuff[512];

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    memset(recvBuff, 0,sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));
    
    /*Cria o Socket */
    if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

	/* Configura o IP de destino e porta na estrutura sockaddr_in */
    memset(&serv_addr, 0, sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

	/* Conecta ao servidor. */
    if( connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 
	

    while (1){
        
        
        printf("Enter message : ");
        fgets(sendBuff, sizeof(sendBuff), stdin);
        
          if (send(s, sendBuff, strlen(sendBuff) , 0)==-1)
        { 
            die("send()");
        }
        
        memset(recvBuff, 0, 512);
        
        if (recv(s, recvBuff, 512, 0) == -1){
            die("recv()");
        }
         
        puts(recvBuff);
        
        
        }
    close(s);
    return 0;
    
}
