#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")
#define Port 5000
#define Server_Ip_Address "192.168.221.131"
#define recvBuf_Size 255
#define sendBuf_Size 255

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char recvBuf[recvBuf_Size];
    char sendBuf[sendBuf_Size];
    char keybordBuf[10];  
    int iResult;

    // Inatialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    while(1) {
        printf("Would you like to connect with server? [Y/N]: ");
        scanf("%s", &keybordBuf);
         if('Y' == keybordBuf[0]){

            // Creat socket
            clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (clientSocket == INVALID_SOCKET) {
                printf("socket failed: %ld\n", WSAGetLastError());
                WSACleanup();
                return 1;
            }

            // Configure
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(Port);
            serverAddr.sin_addr.s_addr = inet_addr(Server_Ip_Address);

            // Connect server
            iResult = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            if (iResult == SOCKET_ERROR) {
                printf("connect failed: %d\n", WSAGetLastError());
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
            iResult = recv(clientSocket, recvBuf, recvBuf_Size, 0);
            printf("Received message from server: %s\n", recvBuf);

            while(1) {
                //Recieve message from server
                while(1) {
                    memset(recvBuf, 0, recvBuf_Size);
                    iResult = recv(clientSocket, recvBuf, recvBuf_Size,0);  

                    if('q' == recvBuf[0]) {
                    
                        printf("Received message from server: ready for receive!\n");
                        break;
                    } 
                    else {
                     memset(sendBuf, 0, sendBuf_Size);   
                    }
                    printf("\nReceived message from server: %s\n", recvBuf);
                }
                //Send message to server
                while(1) {
                    printf("Send message to server? [Y/N] :  ");
                    scanf("%s", &sendBuf);

                    if('Y' == sendBuf[0]) {
                    printf("Input Data: ");
                    memset(recvBuf, 0, recvBuf_Size);
                    memset(sendBuf, 0, sendBuf_Size);
                    scanf("%s", &sendBuf);
                    iResult = send(clientSocket, sendBuf, (int)strlen(sendBuf), 0);

                    }
                    else if('N' == sendBuf[0]) {
                    printf("\nWait for message from server!\n");
                    memset(sendBuf, 0, sendBuf_Size);
                    sendBuf[0] = 'q';
                    iResult = send(clientSocket, sendBuf, (int)strlen(sendBuf), 0);
                    break;           
                    }
                }
                //Judge whether release connection
                if((sendBuf[0] == 'q')&&(recvBuf[0] == 'q')) {
                    memset(recvBuf, 0, recvBuf_Size);   
                    memset(sendBuf, 0, sendBuf_Size);             
                    break;}
            }
            //Close socket
            closesocket(clientSocket);

            }                    
        }
            //Release Winsock
            WSACleanup();
    return 0;      
}



