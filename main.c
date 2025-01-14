#include "./server/server_socket.h"
#include "./client/client_socket.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int client_port = atoi(argv[1]); 
  int server_port = atoi(argv[2]); 

  pthread_t server_tid;
  if (pthread_create(&server_tid, NULL, server_socket, &server_port) != 0) {
    perror("Failed to create server thread");
    return 1;
  }

  char str[100];
  printf("Type exit to exit\n");
  while (1) {
    scanf("%s", str);
    if (strcmp(str, "exit") == 0) {
      break;
    }
    client_socket(str, client_port);
  }
  return 0;
}