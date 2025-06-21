#include "router.h"

#include "handlers.h"
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

void send_404_response(int fd, struct HttpResponse* res) {
  res->status_code = HTTP_NOT_FOUND;
  res->version     = "HTTP/1.1";
  res->status_text = get_status_text(HTTP_NOT_FOUND);
  add_res_header(res, get_header_field_name(HEADER_CONTENT_TYPE),
                 get_content_type_string(CONTENT_TYPE_HTML));

  get_path_template(res->body, sizeof(res->body), NOT_FOUND_ROUTE_PATH);
  send_http_response(fd, res);
};

void send_bad_request_response(int fd, struct HttpResponse* res) {
  res->status_code = HTTP_BAD_REQUEST;
  res->version     = "HTTP/1.1";
  res->status_text = get_status_text(HTTP_NOT_FOUND);
  add_res_header(res, get_header_field_name(HEADER_CONTENT_TYPE),
                 get_content_type_string(CONTENT_TYPE_PLAIN));
  send_http_response(fd, res);
}

struct Route routes[] = {{"GET", INDEX_ROUTE_PATH, handle_home},
                         {"GET", ACCOUNTS_ROUTE_PATH, handle_accounts},
                         {"GET", "/api/accounts", handle_accounts},
                         {"GET", LOGIN_ROUTE_PATH, handle_login}};

RouteHandler get_route_handler(struct HttpRequest* req, struct HttpResponse* res) {
  const int routers_count = sizeof(routes) / sizeof(routes[0]);
  for (int i = 0; i < routers_count; i++) {
    const struct Route route = routes[i];
    if ((strcasecmp(req->method, route.method) == 0) && (strcmp(req->path, route.path) == 0)) {
      return route.handler;
      break;
    }
  }
  return NULL;
}