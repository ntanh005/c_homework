#include <stdio.h>
#include <string.h>  //strlen
#include <stdlib.h>  //strlen
#include <sys/socket.h>
#include <arpa/inet.h>  //inet_addr
#include <unistd.h>     //write
#include <pthread.h>    //for threading , link with lpthread

#define PORT 8888
#define DATA_LENGTH 1024

// data area
int numConnected = 0;
int numLogOn = 0;
int numLogOff = 0;
//<-------

// the thread function

void *connection_handler(void *);
void* serverHandle(void* );
void prtStatus();

int main(int argc, char *argv[]) {
   char message[DATA_LENGTH];
   pthread_t thread_id;
   if( pthread_create(&thread_id, NULL, serverHandle, null) < 0 ){
      perror("could not create thread");
      return 1;
   }

   while(true){
      memset(message, 0, DATA_LENGTH);
      printf("server> ");
      scanf("%s", message);
      if(strcmp(message, "exit"){
	 // exit
         break;        
      }else if(strcmp(message, "prtStatus"){
         // print all status
         printStatus();
      }else{
	  printf("not support cmd: %s", message);
      }
   }
}
void* serverHandle(void* ){
  int socket_desc = 0;
  int client_sock = 0;
  int c = 0;

  struct sockaddr_in server;
  struct sockaddr_in client;

  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) {
    printf("Could not create socket");
    return 1;
  }
  // Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

  // Bind
  if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
    // print the error message
    perror("bind failed. Error");
    return 1;
  }
  // Listen
  listen(socket_desc, 10);
  // Accept and incoming connection
  puts("Waiting for incoming connections...");
  c = sizeof(struct sockaddr_in);
  pthread_t thread_id;
  while ((client_sock = accept(socket_desc, (struct sockaddr *)&client,
                               (socklen_t *)&c))) {
    printf("New connection , socket fd is %d , ip is : %s , port : %d \n" ,
             new_socket , inet_ntoa(client.sin_addr) , ntohs(client.sin_port));  
    ++numConnected;
    if (pthread_create(&thread_id, NULL, connection_handler,
                       (void *)&client_sock) < 0) {
      perror("could not create thread");
      return 1;
    }
    // Now join the thread , so that we dont terminate before the thread
    // pthread_join( thread_id , NULL);
    puts("Handler assigned");
  }
  if (client_sock < 0) {
    perror("accept failed");
    return 1;
  }
  return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc) {
  // Get the socket descriptor
  int sock = *(int *)socket_desc;
  int read_size = 0;
  char client_message[DATA_LENGTH];
  memset(client_message, 0, DATA_LENGTH);
  while ((read_size = recv(sock, client_message, DATA_LENGTH, 0)) > 0) {
    // end of string marker
    client_message[read_size] = '\0';
    // Send the message back to client
    // write(sock , client_message , strlen(client_message));
    //puts(client_message);
    if( strcpm(client_message, "LOGON") == 0){
        // servLogon
        
    }else if( strcpm(client_message, "LOGOFF") == 0){
        // servLogOff
    }else if( strcpm(client_message, "STATUS_REQ") == 0){
        // serv status request

    }else{
        // do nothing
    }
    
    // clear the message buffer
    memset(client_message, 0, DATA_LENGTH);
  }

  if (read_size == 0) {
    puts("Client disconnected");
    fflush(stdout);
  }

  else if (read_size == -1) {
    perror("recv failed");
  }

  return 0;
}

void prtStatus(){
    printf("Number of connected client: %d", numConnected);
    printf("Number of log on client: %d", numLogOn);
    printf("Number of log off client: %d", numLogOff);
}
