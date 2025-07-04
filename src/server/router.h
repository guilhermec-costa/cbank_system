#ifndef CBANK_ROUTER_H
#define CBANK_ROUTER_H

#include "http_parser.h"

#include <stdbool.h>

typedef void (*RouteHandler)(int fd, struct HttpRequest* req, struct HttpResponse* res);
typedef void (*Middleware)(struct HttpRequest*, struct HttpResponse*);

#define MAX_METHODS 4
#define MAX_MIDDLEWARES 10

typedef struct {
  const char*  path;
  RouteHandler handler;
  const char*  allowed_methods[MAX_METHODS];
  Middleware   middlewares[MAX_MIDDLEWARES];
  int          method_count;
  int          middleware_count;
} Route;

enum HandlerErrorFlags { NO_ERROR_FLAG, NOT_FOUND_FLAG, METHOD_NOT_ALLOWED_FLAG };

struct RouteValidationResponse {
  const Route            route;
  enum HandlerErrorFlags error_flag;
  bool                   valid;
};

typedef struct {
  Route* routes;
  size_t count;
  size_t capacity;
} RouteRegistry;

struct RouteValidationResponse get_route(RouteRegistry* registry, struct HttpRequest* req,
                                         struct HttpResponse* res);
void                           init_route_registry(RouteRegistry* registry);
void                           free_route_registry(RouteRegistry* registry);
int                            register_route(RouteRegistry* registry, Route route);
void add_res_header(struct HttpResponse* res, const char* key, const char* value);
void add_res_token_cookie(struct HttpResponse* res, const char* token);
void add_content_type(struct HttpResponse* res, ContentType type);
void add_content_len(struct HttpResponse* res, size_t len);
void add_body(struct HttpResponse* res, const char* body);
void send_http_response(int client_fd, const struct HttpResponse* res);
void send_404_response(int fd, struct HttpResponse* res);
void send_bad_request_response(int fd, struct HttpResponse* res);
void send_not_allowed_response(int fd, struct HttpResponse* res);
void make_res_first_line(struct HttpResponse* res, int status_code);
void redirect(int client_fd, struct HttpResponse* res, const char* location);

#endif /* CBANK_ROUTER_H */