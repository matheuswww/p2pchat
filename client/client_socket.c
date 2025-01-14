#include "client_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
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
