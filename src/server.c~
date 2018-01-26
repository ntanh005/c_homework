#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8888
#define DATA_LENGTH 1024

// data area
typedef struct _client {
    int status;
    int fd;
} client;
client* lstClient[100];
int numConnected = 0;
int numLogOn = 0;
//<-------
// prototype function
void prtStatus();
void servConnect(int fd);
void servDisConnect(int fd) ;
void servLogOn1(int fd);
void servLogOff(int fd);
void howToUse();
void servStatus(int fd);
//<---
// the thread function
void* connection_handler(void*);
void* serverHandle(void*);
//<---




int main(int argc, char* argv[])
{
    char message[DATA_LENGTH];
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, serverHandle, NULL) < 0) {
        perror("could not create thread");
        return 1;
    }

    howToUse();

    while (1) {
        memset(message, 0, DATA_LENGTH);
        scanf("%s", message);
        // exit
        if (strcmp(message, "exit") == 0) {            
            break;
        }
        // print all status
        else if (strcmp(message, "prtStatus") == 0) {
            
            prtStatus();
        }
        else {
            howToUse();
        }
    }
}
void* serverHandle(void* p)
{
    int socket_desc = 0;
    int client_sock = 0;
    int c = 0;

    struct sockaddr_in server;
    struct sockaddr_in client;

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
        return;
    }
    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind
    if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0) {
        // print the error message
        perror("bind failed. Error");
        return;
    }
    // Listen
    listen(socket_desc, 10);
    // Accept and incoming connection
    //puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    
    while ((client_sock = accept(socket_desc, (struct sockaddr*)&client,
                (socklen_t*)&c))) {
        servConnect(client_sock);
    }
    if (client_sock < 0) {
        perror("accept failed");
        return;
    }
    return;
}

/*
 * This will handle connection for each client
 * */
void* connection_handler(void* socket_desc)
{
    // Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size = 0;
    char client_message[DATA_LENGTH];
    memset(client_message, 0, DATA_LENGTH);
    while ((read_size = recv(sock, client_message, DATA_LENGTH, 0)) > 0) {
        // end of string marker
        client_message[read_size] = '\0';
        if (strcmp(client_message, "LOGON") == 0) {
            servLogOn1(sock);
        }
        else if (strcmp(client_message, "LOGOFF") == 0) {
            servLogOff(sock);
        }
        else if (strcmp(client_message, "STATUS_REQ") == 0) {
            servStatus(sock);
        }
        else {
            // do nothing
        }
        // clear the message buffer
        memset(client_message, 0, DATA_LENGTH);
    }

    if (read_size == 0) {
        servDisconnect(sock);
    }
    else if (read_size == -1) {
        servDisconnect(sock);
        perror("recv failed");
    }

    return 0;
}

void prtStatus()
{
    printf("Number of connected client: %d\n", numConnected);
    printf("Number of log on client: %d\n", numLogOn);
    printf("Number of log off client: %d\n", numConnected - numLogOn);
}

void servDisconnect(int fd)
{   
    int i = 0;
    struct sockaddr_in  client;
    int len = sizeof( struct sockaddr_in );
    
    for ( i = 0; i < 100; i++) {
        if (fd == lstClient[i]->fd) {
            numConnected--;
            if(lstClient[i]->status == 1){
               numLogOn--;
            }
            free(lstClient[i]);
            lstClient[i] = NULL;
            if(getpeername(fd, (struct sockaddr *)&client, &len) != -1){
               printf(">>>%s:%u disconnected\n", inet_ntoa(client.sin_addr), (unsigned)ntohs(client.sin_port));
            }
            break;
        }
    }
    return;
}

void servConnect(int fd)
{
    client* cl = NULL;
    int i = 0;
    int test = fd;
    struct sockaddr_in  client;
    int len = sizeof( struct sockaddr_in );

    if( (numConnected + 1) < 100){       
        for (i = 0; i < 100; i++) {
            if (lstClient[i] == NULL) {
                if(getpeername(fd, (struct sockaddr *)&client, &len) != -1){
                    printf(">>>%s:%u connected\n", inet_ntoa(client.sin_addr), (unsigned)ntohs(client.sin_port));    
                }
                cl = malloc(sizeof(client));
                cl->fd = fd;
                cl->status = 0;
                lstClient[i] = cl;
                numConnected++;
                pthread_t thread_id;
                if (pthread_create(&thread_id, NULL, connection_handler,(void*)&test)< 0) {
                    perror("could not create thread");
                    return;
                }
                break;
            }
        }
    }else{
        close(fd);
        printf("maximum of number of connection is 100\n");
    }
}

void servLogOn1(int fd)
{
    int i = 0;
    for (  i = 0; i < 100; i++) {
        if (fd == lstClient[i]->fd) {
            if (lstClient[i]->status == 0) {
                lstClient[i]->status = 1;
                numLogOn++;
            }
            break;
        }
    }
    char* msg = "OK";
    write(fd, msg, strlen(msg));
}

void servLogOff(int fd)
{
    int i;
    for ( i = 0; i < 100; i++) {
        if (fd == lstClient[i]->fd) {
            if (lstClient[i]->status == 1) {
                lstClient[i]->status = 0;
                numLogOn--;
            }
            break;
        }
    }
    char* msg = "OK";
    write(fd, msg, strlen(msg));
}

void servStatus(int fd)
{
    int status;
    int i;
    for (  i = 0; i < 100; i++) {
        if (fd == lstClient[i]->fd) {
            status = lstClient[i]->status;
            break;
        }
    }
    if (status == 1) {
        write(fd, "ON", strlen("ON"));
    }
    else {
        write(fd, "OFF", strlen("OFF"));
    }
}

void howToUse(){
    printf("\tprtStatus: Print client statuses\n");
    printf("\texit: Exit\n");
} 

