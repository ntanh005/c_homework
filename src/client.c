#include <stdio.h>       //printf
#include <string.h>      //strlen
#include <sys/socket.h>  //socket
#include <arpa/inet.h>   //inet_addr

#define PORT 8888
#define DATA_LENGH 1024
void howToUse();

int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in server;
  char message[DATA_LENGH];
  char server_reply[DATA_LENGH];
  int rt_size = 1;

  // Create socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    printf("Could not create socket");
    return 1;
  }
  
  // prepare server address
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);

  // Connect to remote server
  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connect failed. Error");
    return 1;
  }
  howToUse();

  // keep communicating with server
  while (rt_size > 0) {
    // clear the message buffer
    memset(server_reply, 0, DATA_LENGH);
    memset(message, 0, DATA_LENGH);

    printf("client> ");
    scanf("%s", message);

    if (strcmp(message, "logon") == 0) {
      char *logonMsg = "LOGON";
      // Send some data
      if (send(sock, logonMsg, strlen(logonMsg), 0) < 0) {
        puts("Send failed");
        break;
      }
      // Receive a reply from the server
      rt_size = recv(sock, server_reply, DATA_LENGH, 0);
      if (rt_size < 0) {
         puts("recv failed ");
         break;
      }else if (rt_size == 0) {
         puts("server is termminated ");
         break;
      }else{
         printf(">>>%s\n",server_reply);
      }
      
    } else if (strcmp(message, "logoff") == 0) {
      char *logoffMsg = "LOGOFF";
      // Send some data
      if (send(sock, logoffMsg, strlen(logoffMsg), 0) < 0) {
        puts("Send failed");
        break;
      }
      // Receive a reply from the server
      rt_size = recv(sock, server_reply, DATA_LENGH, 0);
      if (rt_size < 0) {
         puts("recv failed ");
         break;
      }else if (rt_size == 0) {
         puts("server is termminated ");
         break;
      }else{
         printf(">>>%s\n",server_reply);
      }
    } else if (strcmp(message, "status") == 0) {
      char *logoffMsg = "STATUS_REQ";
      // Send some data
      if (send(sock, logoffMsg, strlen(logoffMsg), 0) < 0) {
        puts("Send failed");
        break;
      }
      // Receive a reply from the server
      rt_size = recv(sock, server_reply, DATA_LENGH, 0);
      if (rt_size < 0) {
         puts("recv failed ");
         break;
      }else if (rt_size == 0) {
         puts("server is termminated ");
         break;
      }else{
         printf(">>>%s\n",server_reply);
      }
    } else if (strcmp(message, "exit") == 0) {
	break;
    } else {
      howToUse();
    }
  }

  close(sock);
  return 0;
}

void howToUse(){
   printf("\tlogon: send logon request\n");
   printf("\tlogoff: send logoff request\n");
   printf("\tstatus: send status request\n");
   printf("\texit: send logon request\n");
}
