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
#include <pthread.h>

void die(char *s){
    perror(s);
    exit(1);
}

struct thread_data{
    int s,n;
    char msg[512];
    //char recvBuff[512];
   
   
};

char* recvBuff1(int s, char *msg) {
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

 void * client(void* td){
   struct thread_data *d = (struct thread_data*)td;
 //  int t =  d-> n;
   int sk = d -> s;
   char *message = d-> msg;

   while(1){    

    char *mensagem;
    mensagem = recvBuff1(sk , message);
    printf("%s \n", mensagem);
    fflush(stdout);
     }
   return NULL;
}
 

int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr;
    int s = 0;
    char msg[512];
    char recvBuff[512];
    char sendBuff[512];
    char sendBuff1[512];
   //pthread_t thr;
    msg[0] = 100;
    strcpy(&msg[1], argv[3]);
    //struct thread_data *td;


    if(argc != 4)
    {
        printf("\n Usage: %s <ip of server> <port> <nickname>\n",argv[0]);
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
    serv_addr.sin_port = htons(atoi(argv[2]));

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
   
    send(s, msg, strlen(&msg[1])+2,0);  // Primeiro send
    recvBuff1(s, msg);  // Primeiro Recive aceitação do servidor
   
    printf("%d \n", msg[0]);


   // td = (struct thread_data *)malloc(sizeof(struct thread_data));
    //td->s = s;

   // pthread_create(&thr, NULL, client, (void *)td);
    //pthread_detach(thr);


    while (1){
        //menu
	int a = 0;
        printf("1- Enviar mesangem a todos os participantes \n");
        printf("2- Enviar mesangem privada \n");
        printf("3- Listar participantes \n");
        printf("4- Sair \n");
        printf("Selecione a opção: \n");
        scanf("%d", &a);
       //pegar o nome das pessoas que estão no chat
     
        switch(a){
           
            case 1:
                //zera os buffers
                memset(sendBuff, 0, sizeof(sendBuff));
                memset(recvBuff, 0,sizeof(recvBuff));
                memset(sendBuff1, 0, sizeof(sendBuff1));
 
       
                //envia 101 ao servidor
                sendBuff1[0] = 101;
               
                printf("Escreva a mensagem: ");
                scanf(" %[^\n]", &sendBuff1[1]);
                strcat(sendBuff1, '\0');
               
                send(s, sendBuff1, strlen(&sendBuff1[1])+2 , 0);

                recvBuff1(s, msg);

                printf("%s \n", msg); //checa se recebeu certo
                break;

            case 2:
                //zera os buffers
                memset(sendBuff, 0, sizeof(sendBuff));
                memset(recvBuff, 0,sizeof(recvBuff));
                memset(sendBuff1, 0, sizeof(sendBuff1));
   		//declaracao de variaveis
                int temp;  
                sendBuff1[0] = 102;


                printf("Nickname: ");   
                scanf(" %[^\n]", &sendBuff1[1]);
                temp = strlen(sendBuff1);
                strcat(sendBuff1, "|");
                printf("Message: ");
                scanf(" %[^\n]", &sendBuff1[temp]+1);
                strcat(sendBuff1, '\0');
       	        printf("Valor do sendbuff é: %s ", &sendBuff1[0]); // TA OK
                send(s, sendBuff1, strlen(&sendBuff1[1])+2 , 0);
                recvBuff1(s, msg);
                printf("%d \n", msg[0]); //checa se recebeu certo 
 
                break;  
 
            case 3:  //listar os usuarios
                //zera os buffers
                memset(recvBuff, 0,sizeof(recvBuff));
                memset(sendBuff1, 0, sizeof(sendBuff1));
 
                sendBuff1[0] = 104;
                sendBuff1[1] = '\0';
                send(s, sendBuff1, strlen(&sendBuff1[0])+2 , 0);
                recvBuff1(s, msg);
                printf("%s \n", msg);
           
                break;

            case 4:  //sair 
                memset(sendBuff, 0, 512);
                sendBuff[0] = 103;
                sendBuff[1] = '\0';
                send(s, sendBuff, strlen(&sendBuff[0])+2 , 0);
                exit(1);


   

                break;
           
        }
           
       
       
       
   
       
       
        }
  return 0;
   
}
