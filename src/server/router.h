#ifndef CBANK_ROUTER_H
#define CBANK_ROUTER_H

#include "http_parser.h"

struct Route {
  const char* method;
  const char* path;
  void (*handler)(int client_fd, struct HttpRequest* req);
};

void route_request(int client_fd, struct HttpRequest* req);

#endif /* CBANK_ROUTER_H */