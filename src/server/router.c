#include "router.h"

#include "handlers.h"
#include "http_parser.h"
#include "http_utils.h"
#include "route_contants.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void add_res_header(struct HttpResponse* res, const char* key, const char* value) {
  if (res->header_count >= MAX_HEADERS)
    return;
  strncpy(res->headers[res->header_count].key, key, MAX_KEY_LEN - 1);
  strncpy(res->headers[res->header_count].value, value, MAX_VALUE_LEN - 1);
};

void send_http_response(int client_fd, const struct HttpResponse* res) {
  char header_buf[2048];
  int  offset = 0;

  // status line - VERSION STATUS_CODE STATUS_MESSAGE
  offset += snprintf(header_buf, sizeof(header_buf), "%s %d %s" CRLF, res->version,
                     res->status_code, res->status_text);

  // headers
  for (int i = 0; i < res->header_count; i++) {
    offset += snprintf(header_buf + offset, sizeof(header_buf) - offset, "%s : %s" CRLF,
                       res->headers[i].key, res->headers[i].value);
  }

  // empty line that separates headers from body
  offset += snprintf(header_buf + offset, sizeof(header_buf) - offset, CRLF);

  write(client_fd, header_buf, offset);

  if (res->body) {
    write(client_fd, res->body, sizeof(res->body));
  }
};

struct Route routes[] = {{"GET", INDEX_ROUTE_PATH, handle_home},
                         {"GET", ACCOUNTS_ROUTE_PATH, handle_accounts},
                         {"GET", "/api/accounts", handle_accounts},
                         {"GET", LOGIN_ROUTE_PATH, handle_login}};

void route_request(struct HttpRequest* req, struct HttpResponse* res) {
  const int routers_count = sizeof(routes) / sizeof(routes[0]);
  int       router_found  = 0;
  for (int i = 0; i < routers_count; i++) {
    const struct Route route = routes[i];
    if ((strcasecmp(req->method, route.method) == 0) && (strcmp(req->path, route.path) == 0)) {
      route.handler(req, res);
      router_found = 1;
      break;
    }
  }
  // if (!router_found) {
  //   const char* not_found =
  //       "HTTP/1.1 404 Not Found" CRLF "Content-Type: text/plain" CRLF CRLF "404 Not Found";
  //   write(client_fd, not_found, strlen(not_found));
  // }
}