#ifndef CBANK_ROUTER_H
#define CBANK_ROUTER_H

#include "http_parser.h"

typedef void (*RouteHandler)(int fd, struct HttpRequest* req, struct HttpResponse* res);

#define MAX_METHODS 4

struct Route {
  const char*  allowed_methods[MAX_METHODS];
  const char*  path;
  RouteHandler handler;
  int          method_count;
};

enum HandlerErrorFlags { NO_ERROR_FLAG, NOT_FOUND_FLAG, METHOD_NOT_ALLOWED_FLAG };

struct RouteHandlerResponse {
  const RouteHandler     handler;
  enum HandlerErrorFlags error_flag;
};

struct RouteHandlerResponse get_route_handler(struct HttpRequest* req, struct HttpResponse* res);
void add_res_header(struct HttpResponse* res, const char* key, const char* value);
void send_http_response(int client_fd, const struct HttpResponse* res);
void send_404_response(int fd, struct HttpResponse* res);
void send_bad_request_response(int fd, struct HttpResponse* res);
void send_not_allowed_response(int fd, struct HttpResponse* res);
void make_res_first_line(struct HttpResponse* res, int status_code);

#endif /* CBANK_ROUTER_H */