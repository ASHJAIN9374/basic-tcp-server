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
    int n;
    char buffer[256];
    while((opt=getopt(argc,argv,"s:p:"))!=-1)
    {
        switch(opt){
            case 's':
            ip_address=optarg;
            break;
            case 'p':
            port_number=atoi(optarg);
            break;
            default:
            fprintf(stderr,"Usage %s -s <IP_address> -p <port_number>\n",argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    if(ip_address==NULL||port_number==-1){
        fprintf(stderr,"BOTH IP_address and port_number are required");
        exit(EXIT_FAILURE);
    }
    int sock=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server_addr;
    memset(&server_addr,'\0',sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port_number);
    inet_pton(AF_INET,ip_address,&server_addr.sin_addr);
    socklen_t addrlen=sizeof(server_addr);
    if(connect(sock,(struct sockaddr*)&server_addr,addrlen)<0)
    {
        perror("Connection failed");
        exit(1);
    }
    printf("Server's IP Address is : %s\n",ip_address);
    printf("Server's port number is : %d\n",port_number);
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    getsockname(sock, (struct sockaddr *) &client_addr, &client_len);
    printf("Client port: %d\n", ntohs(client_addr.sin_port));
    int user_input;
    printf("Enter an integer: ");
    scanf("%d", &user_input);

    bzero(buffer, 256);
    sprintf(buffer, "%d", user_input);
    n = write(sock, buffer, strlen(buffer));
    if (n < 0) perror("ERROR writing to socket");

    bzero(buffer, 256);
    n = read(sock, buffer, 255);
    if (n < 0) perror("ERROR reading from socket");

    int received_sum;
    memcpy(&received_sum, buffer, sizeof(int));
    printf("Received sum from server: %d\n", received_sum);
    close(sock);
    return 0;
}