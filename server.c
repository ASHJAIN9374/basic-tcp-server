#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
int main(int argc,char *argv[]){
    int opt;
    char *ip_address=NULL;
    int port_number=-1;
    char buffer[256];
    int x;
    while((opt=getopt(argc,argv,"i:p:c:"))!=-1)
    {
        switch(opt){
            case 'i':
            ip_address=optarg;
            break;
            case 'p':
            port_number=atoi(optarg);
            case 'c':
            x=atoi(optarg);
            break;
            default:
            fprintf(stderr,"Usage %s -i <IP_address> -p <port_number>\n",argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    if(ip_address==NULL||port_number==-1){
        fprintf(stderr,"BOTH IP_address and port_number are required");
        exit(EXIT_FAILURE);
    }
    int server_sock=socket(AF_INET,SOCK_STREAM,0);
    int client_sock;
    struct sockaddr_in server_addr,client_addr;
    memset(&server_addr,'\0',sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port_number);
    inet_pton(AF_INET,ip_address,&server_addr.sin_addr);
    if(bind(server_sock,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        perror("BIND ERROR");
        exit(1);
    }
    printf("IP Address is : %s\n",ip_address);
    printf("port number is : %d\n",port_number);
    listen(server_sock,10);
    socklen_t addrlen=sizeof(client_addr);
    while(1)
    {
        printf("waiting to accept\n");
        client_sock=accept(server_sock,(struct sockaddr*)&client_addr,&addrlen);
        printf("Connected to client at %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        if(fork()==0){
        bzero(buffer, 256);
        int n = read(client_sock, buffer, 255);
        if (n < 0) perror("ERROR reading from socket");

        int received_int = atoi(buffer);
        printf("Received number from client: %d\n", received_int);

        int sum_result = received_int + x;
        n = write(client_sock, &sum_result, sizeof(int));
        if (n < 0) perror("ERROR writing to socket");
        }
    }
    close(client_sock);
    close(server_sock);
    return 0;
}