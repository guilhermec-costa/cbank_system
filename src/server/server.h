#ifndef CBANK_SERVER_H
#define CBANK_SERVER_H

#include "router.h"
#define CLIENT_BUFFER_SIZE 16000

#include "http_parser.h"

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
  RouteRegistry       route_registry;
  struct ServerConfig cfg;
  int                 socket_fd;
  struct sockaddr_in  sock_address;
  void (*start)(const struct Server* server);
};

struct Server make_server(struct ServerConfig config, void (*start)(const struct Server* server),
                          RouteRegistry*      registry);
void          start(const struct Server* server);
void          end_client_conn(int client_fd, struct HttpRequest* req, struct HttpResponse* res);

#endif /* CBANK_SERVER_H */