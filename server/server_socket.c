#include "server_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void* server_socket(void *arg) {
  int port = *((int*)arg); 
  int server_fd, new_socket;
  ssize_t valread;
  struct sockaddr_in address;
  int opt = 1;
  socklen_t addrlen = sizeof(address);
  char buffer [1024] = {0};

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt SO_REUSEADDR");
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

