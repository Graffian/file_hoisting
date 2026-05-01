#include <stdio.h>
#include <winsock2.h>


int main(){
    char buffer[100];
    char response[] =
    "HTTP/1.1 200 OK\r\n"
    "Access-Control-Allow-Origin: *\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
    "Hello";
    FILE *fp = fopen("../storage/test.txt" , "wb");
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2) , &wsa);
    SOCKET client_socket , server_socket;
    struct sockaddr_in client , server;
    server_socket = socket(AF_INET , SOCK_STREAM , 0);
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    bind(server_socket , (struct sockaddr *)&server , sizeof(server));
    printf("STARTING SERVER........\n");
    listen(server_socket , 3);
    printf("SERVER LISTENING TO PORT 8080");
    int c = sizeof(struct sockaddr_in);
    while (1){
        client_socket = accept(server_socket , (struct sockaddr *)&client , &c);
        int bytes = recv(client_socket , buffer , sizeof(buffer)-1 , 0);
        buffer[bytes] = '\0';
        printf("Client said : %s\n" , buffer);
        fwrite(buffer , 1 , sizeof(buffer) , fp);
        fclose(fp);
        send(client_socket , response , sizeof(response) , 0);
        closesocket(client_socket);

    }
    closesocket(server_socket);
    WSACleanup();

    return 0;
}