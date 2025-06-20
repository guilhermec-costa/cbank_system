#ifndef CBANK_ROUTER_H
#define CBANK_ROUTER_H

#include "http_parser.h"

typedef void (*RouteHandler)(int fd, struct HttpRequest* req, struct HttpResponse* res);

struct Route {
  const char*  method;
  const char*  path;
  RouteHandler handler;
};

RouteHandler get_route_handler(struct HttpRequest* req, struct HttpResponse* res);
void         add_res_header(struct HttpResponse* res, const char* key, const char* value);
void         send_http_response(int client_fd, const struct HttpResponse* res);
void         send_404_response(int fd, struct HttpResponse* res);

#endif /* CBANK_ROUTER_H */