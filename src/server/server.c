#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int server() {
  int server_fd, client_fd;
  char client_buf[3000] = {0};

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_sock_addr;
  int                addrlen = sizeof(server_sock_addr);

  server_sock_addr.sin_family      = AF_INET;
  server_sock_addr.sin_addr.s_addr = INADDR_ANY;
  server_sock_addr.sin_port        = htons(8080);

  if (bind(server_fd, (struct sockaddr*)&server_sock_addr,
           sizeof(server_sock_addr)) < 0) {
    perror("socket bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 10) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port 8080\n");

  while (1) {
    struct sockaddr_in client_addr;
    printf("ALo\n");

    if ((client_fd = accept(server_fd, (struct sockaddr*)&server_sock_addr,
                            (socklen_t*)&addrlen)) < 0) {
      perror("accept failed");
      continue;
    }

    printf("Client connected\n");

    memset(client_buf, 0, sizeof(client_buf));
    read(client_fd, client_buf, sizeof(client_buf) - 1);
    const char* response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html\r\n"
                           "Content-Length: 11\r\n"
                           "\r\n"
                           "Hello World";

    write(client_fd, response, strlen(response));
    printf("Response sent! closing connection\n");
    close(client_fd);
  }

  close(server_fd);
}