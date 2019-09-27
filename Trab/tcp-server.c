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

// Var global
char armazenaNick[512];


void* mostrarNicks(void* arm){
    printf(" \n Participantes: %s \n ", (char*)arm);
    return 0;
}

//void* retiraNicks(void* arm void* nick){
    //percorrer o char e retirar o nick passsado como parametro
   // return 0;
    //}

//função para receber os caracteres do cliente
char* recebe(int s, char *msg) {
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

//


//estrutura dos dados do cliente
struct client_data{
    int sk;
    char *nick;
    struct sockaddr_in *client_addr;
    
};



//thread que vai ser excluisava para cada cliente
void * client_handle(void* cd){
    struct client_data *client = (struct client_data *)cd;
    char sendBuff[512];
    char recvBuff[512];
    char recvBuff1[512];
   
   
    //zera os buffers
    memset(sendBuff, 0, sizeof(sendBuff));
    memset(recvBuff, 0,sizeof(recvBuff));
    memset(recvBuff1, 0,sizeof(recvBuff1));
   
    //quando o cliente se conectar ao servidor, imprime as frases abaixo
    printf("Waiting for data...");
    fflush(stdout);
   
    //recebe o  dado 1 do cliente
    recebe(client->sk, recvBuff);
   
   
    switch (recvBuff[0]){
       
       case 101:
            memset(sendBuff, 0, sizeof(sendBuff));
            sendBuff[0] = 109;
            sendBuff[1] = '\0';
            printf("\n Dado Enviado por %s: ", client->nick); 
            puts(&recvBuff[1]);
            send(client->sk, sendBuff, strlen(sendBuff)+2, 0); 
            //envia a mensagem para a fila para TODOS Nick: Mensagem
            break;
            
       case 102:
            memset(recvBuff1, 0,sizeof(recvBuff1));
            memset(sendBuff, 0, sizeof(sendBuff));       
            recebe(client->sk, recvBuff1);
            // criar if para ver se existe esse nick no vetor de Char 
            // 105 sucesso, 106 não encontrado
            
            // sendBuff[0] = 105;
            // send(client-> sk, sendBuff, strlen(sendBuff)+2, 0);
            // envia para fila para o NICK
            
            //fputs(recvBuff1, stdout);   
            break;
       case 104: //listar
            memset(sendBuff, 0, sizeof(sendBuff));
            sendBuff[0] = 107;
            strcat(sendBuff, armazenaNick );
            strcat(sendBuff, "Sucesso");
            strcat(sendBuff, '\0');
            send(client->sk, sendBuff, strlen(sendBuff)+2, 0); 
            break;
            
       case 103: //fechar 
            //opcao(FILA,104, 0);
            printf("\n Clinte Desconectado! \n");
            //retiraNick 
            close(client->sk);
            break;
    }
   


            //acho que vou ter que fazer um while 1 aqui... 
            //free(client->client_addr);
            //free(client);

    return NULL;
}

int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr;
    unsigned int addrlen;
    int s = 0;
    struct client_data *cd;
    char msg[512];
    char msgB[512];
    char *pegaNick;
    pthread_t thr;
    s = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));

    memset(msgB, 0, sizeof(msgB));
    memset(armazenaNick, 0,sizeof(armazenaNick));
    

 

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

    //cliente conecta ao servidor
    cd->sk = accept(s, (struct sockaddr*)cd->client_addr, (socklen_t*)&addrlen);
   
    //condicional para verificar se o cliente se conectou ao servidor
    if (cd->sk > 0) {
        // pega o nick do cliente usando a função recvBuff que será enviada pelo Cliente
        pegaNick = recebe(cd->sk, msg);  //recebe o 100 + Nick
        //criar um if aqui para verificar se já existe um usuario com esse nick 
        printf("Cliente conectado: %s \n", &pegaNick[1]);
        printf("  105 \n");
        msgB[0] = 105;
        msgB[1] = '\0';
        strcat(armazenaNick, &pegaNick[1]);
        strcat(armazenaNick, "|");
        mostrarNicks(&armazenaNick); //Mostra os participantes do Chat
        cd->nick = *(char*)pegaNick; //?
        send(cd->sk, msgB, strlen(msgB)+2, 0);  
        
    }
   
    
    pthread_create(&thr, NULL, client_handle, (void *)cd);
    pthread_detach(thr);


     }
}
