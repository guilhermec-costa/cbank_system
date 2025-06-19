#ifndef CBANK_SERVER_H
#define CBANK_SERVER_H

#define BUFFER_SIZE 16000

#include <netinet/in.h>

#define PORT 8080

struct ServerConfig {
  int      domain;
  int      port;
  int      protocol;
  int      queue_s;
  int      socket_type;
  uint32_t in_address;
};

struct Server {
  struct ServerConfig cfg;
  int                 socket_fd;
  struct sockaddr_in  sock_address;
  void (*start)(const struct Server* server);
};

struct Server make_server(struct ServerConfig config, void (*start)(const struct Server* server));
void          start(const struct Server* server);

#endif /* CBANK_SERVER_H */