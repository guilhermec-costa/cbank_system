#include "router.h"

#include "../api/http_handlers/handlers.h"
#include "http_parser.h"
#include "http_utils.h"
#include "route_contants.h"
#include "templates_constants.h"

#include <stdio.h>
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

  get_path_template(res->body, sizeof(res->body), NOT_FOUND_ROUTE_PATH);
  send_http_response(fd, res);
};

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

struct Route routes[] = {{{"GET"}, INDEX_ROUTE_PATH, handle_home, 1},
                         {{"GET"}, ACCOUNTS_ROUTE_PATH, handle_accounts, 1},
                         {{"GET", "POST"}, "/api/accounts", handle_accounts, 2},
                         {{"GET", "POST"}, LOGIN_ROUTE_PATH, handle_login, 2}};

static struct RouteHandlerResponse make_handler_response(const RouteHandler*    handler,
                                                         enum HandlerErrorFlags f) {
  struct RouteHandlerResponse res = {
    .handler = handler ? *handler : NULL,
    f
  };
  return res;
};

struct RouteHandlerResponse get_route_handler(struct HttpRequest* req, struct HttpResponse* res) {
  const int routers_count = sizeof(routes) / sizeof(routes[0]);
  for (int i = 0; i < routers_count; i++) {
    const struct Route route = routes[i];
    if (strcmp(req->path, route.path) == 0) {
      for (int i = 0; i < route.method_count; i++) {
        if (strcasecmp(req->method, route.allowed_methods[i]) == 0) {
          return make_handler_response(&route.handler, NO_ERROR_FLAG);
        }
      }
      return make_handler_response(NULL, METHOD_NOT_ALLOWED_FLAG);
    }
  }
  return make_handler_response(NULL, NOT_FOUND_FLAG);
}