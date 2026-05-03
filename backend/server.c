#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>


char *find_bytes(char *haystack , int hlen , char *needle , int nlen){
    for (int i=0 ; i<=(hlen-nlen) ; i++){
        if (memcmp(haystack+i , needle , nlen) == 0){
            return haystack+i;
        }
    }

    return NULL;

}


int main(){
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2) , &wsa);
    char buffer[4096];
    SOCKET client_socket , server_socket;
    struct sockaddr_in client,server;
    FILE *fp = fopen("../storage/test.jpg" , "wb");

    server_socket = socket(AF_INET , SOCK_STREAM , 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    bind(server_socket , (struct sockaddr *)&server , sizeof(server));
    listen(server_socket , 3);
    printf("SERVER LISTENING AT PORT 8080... \n");
    int c = sizeof(struct sockaddr_in);
    while(1){
        client_socket = accept(server_socket , (struct sockaddr *)&client , &c);
        if (client_socket){
            printf("CLIENT CONNECTED.....\n");
            int bytes = recv(client_socket , buffer , sizeof(buffer)-1 , 0);
            buffer[bytes] = '\0';
            char *body_start = strstr(buffer , "\r\n\r\n");
            body_start +=4;
            if (body_start){
                char *pointer_to_content_len = strstr(buffer , "Content-Length");
                pointer_to_content_len+=15;
                int content_length = atoi(pointer_to_content_len);
                printf("Content length is : %d\n" , content_length);
                char *body = malloc(content_length+1);
                if (body){
                    int total = 0;
                    int already_read_body = bytes - (int)(body_start - buffer);
                    if(already_read_body > 0){
                        memcpy(body , body_start , already_read_body);
                        total = already_read_body;
                    }

                    while(total < content_length){
                        int actual_body_recieved = recv(client_socket , body+total , content_length-total , 0);
                        if (actual_body_recieved<=0){
                            printf("SOMETHING IS WRONG BODY iS NOT RECIEVED");
                            break;
                        }
                        total+=actual_body_recieved;
                    }
                    body[total] = '\0';
                    char *image_start = find_bytes(body , total , "\r\n\r\n" , 4);
                    if (!image_start){
                        free(body);
                        closesocket(client_socket);
                    }
                    image_start+=4;

                    char boundary[256];
                    char *boundary_pos = strstr(buffer , "boundary=");
                    if(!boundary_pos){
                        free(body);
                        closesocket(client_socket);
                    }
                    boundary_pos+=9;
                    sscanf(boundary_pos , "%255s" , boundary);
                    char end_marker[256];
                    snprintf(end_marker , sizeof(end_marker) , "\r\n--%s--" , boundary);
                    int remaining = total - (int)(image_start - body);
                    char *image_end = find_bytes(image_start , remaining , end_marker , strlen(end_marker));
                    int image_size = (int)(image_end - image_start);
                    FILE *fp = fopen("../storage/test.jpg" , "wb");
                    fwrite(image_start , 1 , image_size , fp);
                    fclose(fp);
                    printf("%d bytes written\n" , image_size);
                    printf("BODY RECIEVED IS : %d" , sizeof(body));
                    free(body);
                }
            }
            continue;
        }
        closesocket(client_socket);
    }
    printf("HEADERS are : %s" , buffer);
    closesocket(server_socket);
    WSACleanup();


    return 0;
}