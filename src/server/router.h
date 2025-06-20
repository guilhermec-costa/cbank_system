#ifndef CBANK_ROUTER_H
#define CBANK_ROUTER_H

#include "http_parser.h"

struct Route {
  const char* method;
  const char* path;
  void (*handler)(struct HttpRequest* req, struct HttpResponse* res);
};

void route_request(struct HttpRequest* req, struct HttpResponse* res);
void add_res_header(struct HttpResponse* res, const char* key, const char* value);
void send_http_response(int client_fd, const struct HttpResponse* res);

#endif /* CBANK_ROUTER_H */