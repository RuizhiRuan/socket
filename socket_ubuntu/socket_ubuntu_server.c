#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_PORT 5000
#define recvBuf_Size 255
#define sendBuf_Size 255

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char recvBuf[recvBuf_Size] = {0};
    char sendBuf[255];  
    int iResult;

    // Creat socket
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //Configure opt
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);

    //Bind port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    //Listen port
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    while(1) {
            //Wait for connection
            printf("Wait for connection!\n");
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
            break;
             }
             else {
                char *client_ip = inet_ntoa(address.sin_addr);
                printf("Successfully connect with client:  %s\n", client_ip);
             }

            char *message = "Hello! Please wait for my message";
            send(new_socket, message, strlen(message), 0);

            while(1) {
                //Send message to client
                while(1) {
                    printf("\nSend message to client? [Y/N] :  ");
                    scanf("%s", sendBuf);

                    if('Y' == sendBuf[0]){
                            printf("\nInput Data:");
                            bzero(recvBuf, recvBuf_Size);
                             bzero(sendBuf, sendBuf_Size);                          
                            scanf("%s", sendBuf);         
                            iResult = send(new_socket, sendBuf, (int)strlen(sendBuf), 0);
                    }
                    else if ('N' == sendBuf[0]){
                            printf("\nWait for message from client!\n");
                            bzero(sendBuf, sendBuf_Size); 
                            sendBuf[0] = 'q';
                            iResult = send(new_socket, sendBuf, (int)strlen(sendBuf), 0);
                            break;
                        }
                }
                //Recieve message from clien
                while(1) {
                    bzero(recvBuf, recvBuf_Size);
                    iResult = recv(new_socket, recvBuf, recvBuf_Size,0);

                    if('q' == recvBuf[0]) {
                        printf("Received message from client: ready for receive!\n");
                        break;
                    }  
                    else {
                        bzero(sendBuf, sendBuf_Size); 
                    }
            printf("Received message from client: %s\n", recvBuf);               
                }
            //Judge whether to relaease connection 
            if((sendBuf[0] == 'q')&&(recvBuf[0] == 'q')) {
                bzero(sendBuf, sendBuf_Size);   
                 bzero(recvBuf, recvBuf_Size);
                break;}
            }
    }
    return 0;
}

