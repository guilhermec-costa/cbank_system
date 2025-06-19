#ifndef CBANK_SERVER_H
#define CBANK_SERVER_H

#define BUFFER_SIZE 16000

#include <netinet/in.h>

#define PORT 8080

struct ServerConfig {
  int domain;
  int port;
  int service;
  int protocol;
  int queue_s;
};

struct Server {
  struct ServerConfig cfg;
  int                 socket_fd;
  struct sockaddr_in  sock_address;
  void (*launch)(struct Server* server);
};

struct Server make_server(int domain, int port, int service, int protocol, int backlog,
                          void (*launch)(struct Server* server));
void          launch(struct Server* server);

#endif /* CBANK_SERVER_H */