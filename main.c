#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <asm-generic/socket.h>
#include <pthread.h>


void* server_socket(void *arg) {
  int port = *((int*)arg); 
  int server_fd, new_socket;
  ssize_t valread;
  struct sockaddr_in address;
  int opt = 1;
  socklen_t addrlen = sizeof(address);
  char buffer [1024] = {0};
  char* hello = "Hello from server";

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt SO_REUSEADDR");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
    perror("setsockopt SO_REUSEPORT");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  
  while(1) {
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
      perror("accpet failed");
      exit(EXIT_FAILURE);
    }

    valread = read(new_socket, buffer, sizeof(buffer) - 1);
    if(valread < 0) {
      perror("read falied");
      exit(EXIT_FAILURE);
    }

    buffer[valread] = '\0';
    printf("socket: %s\n", buffer);

    close(new_socket);
  }

  close(server_fd);

  return 0;
}

int client_socket(char* msg, int port) {
  int status, valread, client_fd;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};

  if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, "192.168.0.106", &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }
  
  if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))) {
    printf("\nConnection Failed \n");
    return -1;
  }

  send(client_fd, msg, strlen(msg), 0);

  valread = read(client_fd, buffer, 1024 - 1);

  printf("%s", buffer);

  close(client_fd);
  return 0;
}

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
    printf("you:");
    scanf("%s", str);
    if (strcmp(str, "exit") == 0) {
      break;
    }
    client_socket(str, client_port);
  }
  return 0;
}