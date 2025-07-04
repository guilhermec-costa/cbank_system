#include "router.h"

#include "../api/middlewares/middlewares.h"
#include "http_parser.h"
#include "http_utils.h"
#include "logger.h"
#include "route_contants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void add_res_header(struct HttpResponse* res, const char* key, const char* value) {
  if (res->header_count >= MAX_HEADERS)
    return;
  strncpy(res->headers[res->header_count].key, key, MAX_KEY_LEN - 1);
  strncpy(res->headers[res->header_count].value, value, MAX_VALUE_LEN - 1);
  res->header_count++;
};

void send_http_response(int client_fd, const struct HttpResponse* res) {
  char header_buf[2048];
  int  offset = 0;

  // status line - VERSION STATUS_CODE STATUS_MESSAGE
  offset += snprintf(header_buf, sizeof(header_buf), "%s %d %s" CRLF, res->version,
                     res->status_code, res->status_text);

  // headers
  for (int i = 0; i < res->header_count; i++) {
    offset += snprintf(header_buf + offset, sizeof(header_buf) - offset, "%s: %s" CRLF,
                       res->headers[i].key, res->headers[i].value);
  }

  // empty line that separates headers from body
  offset += snprintf(header_buf + offset, sizeof(header_buf) - offset, CRLF);

  write(client_fd, header_buf, offset);
  if (res->status_code == HTTP_REDIRECT) {
    return;
  }

  if (strlen(res->body) > 0) {
    write(client_fd, res->body, strlen(res->body));
  }
};

void make_res_first_line(struct HttpResponse* res, int status_code) {
  res->status_code = status_code;
  res->version     = "HTTP/1.1";
  res->status_text = get_status_text(status_code);
};

void send_404_response(int fd, struct HttpResponse* res) {
  make_res_first_line(res, HTTP_NOT_FOUND);
  add_res_header(res, get_header_field_name(HEADER_CONTENT_TYPE),
                 get_content_type_string(CONTENT_TYPE_HTML));

  get_route_render_template(res->body, sizeof(res->body), NOT_FOUND_ROUTE_PATH);
  send_http_response(fd, res);
};

void add_content_type(struct HttpResponse* res, ContentType type) {
  const char* content_type_str = get_header_field_name(HEADER_CONTENT_TYPE);
  const char* html_type        = get_content_type_string(type);
  add_res_header(res, content_type_str, html_type);
};

void add_content_len(struct HttpResponse* res, size_t len) {
  char        body_res_buf[32];
  const char* content_length_str = get_header_field_name(HEADER_CONTENT_LENGTH);
  snprintf(body_res_buf, sizeof(body_res_buf), "%zu", len);
  add_res_header(res, content_length_str, body_res_buf);
}

void add_body(struct HttpResponse* res, const char* body) {
  strncpy(res->body, body, sizeof(res->body) - 1);
  res->body[sizeof(res->body) - 1] = '\0';
}

void add_res_token_cookie(struct HttpResponse* res, const char* token) {
  char buf[1024] = {0};
  snprintf(buf, sizeof(buf), "token=%s; Path=/; HttpOnly; SameSite=Strict; Max-Age=3600", token);
  add_res_header(res, "Set-Cookie", buf);
}

void send_not_allowed_response(int fd, struct HttpResponse* res) {
  make_res_first_line(res, HTTP_METHOD_NOT_ALLOWED);
  add_res_header(res, get_header_field_name(HEADER_CONTENT_TYPE),
                 get_content_type_string(CONTENT_TYPE_PLAIN));
  send_http_response(fd, res);
}

void send_bad_request_response(int fd, struct HttpResponse* res) {
  make_res_first_line(res, HTTP_BAD_REQUEST);
  add_res_header(res, get_header_field_name(HEADER_CONTENT_TYPE),
                 get_content_type_string(CONTENT_TYPE_PLAIN));
  send_http_response(fd, res);
}

void init_route_registry(RouteRegistry* registry) {
  registry->routes   = NULL;
  registry->capacity = 0;
  registry->count    = 0;
};

void free_route_registry(RouteRegistry* registry) {
  free(registry->routes);
  registry->routes   = NULL;
  registry->capacity = 0;
  registry->count    = 0;
};

int register_route(RouteRegistry* registry, Route route) {
  int method_count = 0;
  for (int i = 0; i < MAX_METHODS; i++) {
    if (route.allowed_methods[i])
      method_count++;
    else
      break;
  }
  route.method_count = method_count;

  int middleware_count = 0;
  for (int i = 0; i < MAX_MIDDLEWARES; i++) {
    if (route.middlewares[i])
      middleware_count++;
    else
      break;
  }
  route.middleware_count = middleware_count;

  if (registry->count >= registry->capacity) {
    size_t new_capacity = registry->capacity == 0 ? 8 : registry->capacity * 2;
    Route* new_routes   = realloc(registry->routes, new_capacity * sizeof(Route));
    if (!new_routes) {
      return -1;
    }

    registry->routes   = new_routes;
    registry->capacity = new_capacity;
  }
  registry->routes[registry->count++] = route;
  GLOBAL_LOGGER->log(GLOBAL_LOGGER, DEBUG,
                     "Registered route: path='%s', methods=%d, middlewares=%d", route.path,
                     route.method_count, route.middleware_count);
  return 0;
};

struct RouteValidationResponse get_route(RouteRegistry* registry, struct HttpRequest* req,
                                         struct HttpResponse* res) {
  for (size_t i = 0; i < registry->count; i++) {
    const Route route = registry->routes[i];
    if (strcmp(req->path, route.path) == 0) {
      for (int i = 0; i < route.method_count; i++) {
        if (strcasecmp(req->method, route.allowed_methods[i]) == 0) {
          return (struct RouteValidationResponse){
              .route = route, .error_flag = NO_ERROR_FLAG, .valid = true};
        }
      }
      return (struct RouteValidationResponse){
          .route = NULL, .error_flag = METHOD_NOT_ALLOWED_FLAG, .valid = false};
    }
  }
  return (struct RouteValidationResponse){
      .route = NULL, .error_flag = NOT_FOUND_FLAG, .valid = false};
}

void redirect(int client_fd, struct HttpResponse* res, const char* location) {
  make_res_first_line(res, HTTP_REDIRECT);

  const char* location_header_str = get_header_field_name(HEADER_LOCATION);
  add_res_header(res, location_header_str, location);

  send_http_response(client_fd, res);
}