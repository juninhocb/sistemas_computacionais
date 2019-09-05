/*
    Simple UDP client
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h> 
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
 
#define IPSZ 32
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to send data
 
void die(char *s){
    perror(s);
    exit(1);
}
 
int main(void){
    struct sockaddr_in si_other;  
    unsigned int s, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];
    char ip_dest[IPSZ];
 
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        die("socket");
    }
//familia de protocolos, , protocolo udp
//socket retorna inteiro 
    memset((char *) &si_other, 0, sizeof(si_other));  //seta tudo pra 0
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
     
    while(1)    {
    printf("Enter Destiny IP : ");
     fgets(ip_dest, sizeof(ip_dest), stdin);

        printf("Enter message : ");
        fgets(message, sizeof(message), stdin);
         
     /* set the destiny ip. */
     if (inet_aton(ip_dest , &si_other.sin_addr) == 0) 
     {  //pega o endereco digitado e converte
         fprintf(stderr, "inet_aton() failed\n");
        exit(1);
     }

        /* send the message */
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        { 
            die("sendto()");
        }
         
        /* receive a reply and print it
        clear the buffer by filling null, it might have previously received data */
        memset(buf, 0, BUFLEN);
        /*try to receive some data, this is a blocking call*/
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1){
            die("recvfrom()");
        }
         
        puts(buf);
    }
 
    close(s);
    return 0;
}