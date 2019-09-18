
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
 

int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr; 
    int s = 0, a=0; 
    char recvBuff[512];
    char sendBuff[512];
    char sendBuff1[512];
    //char recvBuff1[512];


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
        //menu
        printf("1- Enviar mesangem a todos os participantes \n");
        printf("2- Enviar mesangem privada \n");
        printf("3- Listar participantes \n");
        printf("4- Sair \n");
        scanf("%d", &a);
       
        
        
        switch(a){
            
            case 1: 
                //zera os buffers
                memset(sendBuff, 0, sizeof(sendBuff));
                memset(recvBuff, 0,sizeof(recvBuff));
                memset(sendBuff1, 0, 512); 
        
                //envia 101 ao servidor
                strcpy(&sendBuff[0], "101"); 
                send(s, sendBuff, strlen(sendBuff) , 0);
                
                sleep(10);
                
                //recebe a resposta do servidor
                recv(s, recvBuff, 512, 0);
             
                   
                int t;   
                if (strcmp(recvBuff, "105") == 0){
                printf("Enter message:  \n");
                scanf("%d", &t);
                fgets(sendBuff1, sizeof(sendBuff1), stdin);
                
                send(s, sendBuff1, strlen(sendBuff1) , 0);
                
                    
                }else {
                    printf("marco");
                
                }
                
                fflush(stdin);
                
                break;
            
            case 4:
                memset(sendBuff, 0, 512);
                sendBuff[0] = 104;
                if (send(s, sendBuff, strlen(sendBuff) , 0)==-1)
                { 
                    die("send()");
                    
                }
                
                 if (recv(s, recvBuff, 512, 0) == -1){
                    die("recv()");
                }
                
                break;
            
        }
            
        
        
        
    
        
        
        }
    close(s);
    return 0;
    
}
