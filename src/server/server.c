#include "server.h"
#include "http_parser.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void set_nonblocking(int fd) {}

void start(const struct Server* server) {
  char client_buf[16000] = {0};
  int  addrlen           = sizeof(server->sock_address);
  while (1) {
    int client_fd;

    if ((client_fd = accept(server->socket_fd, (struct sockaddr*)&server->sock_address,
                            (socklen_t*)&addrlen)) < 0) {
      perror("accept failed");
      continue;
    }

    printf("Client connected\n");

    memset(client_buf, 0, sizeof(client_buf));
    ssize_t bytes_read = read(client_fd, client_buf, sizeof(client_buf) - 1);
    if(bytes_read > 0) {
      client_buf[bytes_read] = '\0';
    }

    struct HttpRequest req;
    printf("%s", client_buf);
    parse_request_line(client_buf, &req);

    const char* response = "HTTP/1.1 200 OK" CRLF
                           "Content-Type: text/html; charset=UTF-8" CRLF CRLF
                           "<!DOCTYPE html>" CRLF
                           "<html>" CRLF
                           "<head>" CRLF
                           "<title>HttpServer</title>" CRLF
                           "</head>" CRLF
                           "<body>" CRLF
                           "<h1>My http server testing again</h1>" CRLF
                           "</body>" CRLF
                           "</html>" CRLF;


    write(client_fd, response, strlen(response));
    printf("Response sent! closing connection\n");
    close(client_fd);
  }

  close(server->socket_fd);
}

struct Server make_server(struct ServerConfig cfg, void (*start)(const struct Server*)) {
  struct Server server;
  server.cfg = cfg;

  server.sock_address.sin_family      = cfg.domain;
  server.sock_address.sin_port        = htons(cfg.port);
  server.sock_address.sin_addr.s_addr = htonl(cfg.in_address);

  if ((server.socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  int sockopt = 1;
  // prevents socket already binded
  if(setsockopt(server.socket_fd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) < 0) {
    perror("setsocketopt failed");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in server_sock_addr;

  server_sock_addr.sin_family      = AF_INET;
  server_sock_addr.sin_addr.s_addr = INADDR_ANY;
  server_sock_addr.sin_port        = htons(8080);

  if (bind(server.socket_fd, (struct sockaddr*)&server_sock_addr, sizeof(server_sock_addr)) < 0) {
    perror("socket bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server.socket_fd, server.cfg.queue_s) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port 8080\n");

  server.start = start;

  return server;
};