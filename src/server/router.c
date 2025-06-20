#include "router.h"

#include "route_contants.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void handle_home(int client_fd, struct HttpRequest* req) {
  char template_content[8192] = {0};
  get_route_html(template_content, sizeof(template_content), req->path);
  if (strlen(template_content) == 0) {
    const char* not_found =
        "HTTP/1.1 404 Not Found" CRLF "Content-Type: text/plain" CRLF CRLF "404 Not Found";
    write(client_fd, not_found, strlen(not_found));
  } else {
    const char* header = "HTTP/1.1 200 OK" CRLF "Content-Type: text/html" CRLF CRLF;
    write(client_fd, header, strlen(header));
    write(client_fd, template_content, strlen(template_content));
  }
}

void handle_accounts(int client_fd, struct HttpRequest* req) {
  char template_content[8192] = {0};
  get_route_html(template_content, sizeof(template_content), req->path);
  if (strlen(template_content) == 0) {
    const char* not_found =
        "HTTP/1.1 404 Not Found" CRLF "Content-Type: text/plain" CRLF CRLF "404 Not Found";
    write(client_fd, not_found, strlen(not_found));
  } else {
    const char* header = "HTTP/1.1 200 OK" CRLF "Content-Type: text/html" CRLF CRLF;
    write(client_fd, header, strlen(header));
    write(client_fd, template_content, strlen(template_content));
  }
}

void handle_login(int client_fd, struct HttpRequest* req) {
  char template_content[8192] = {0};
  get_route_html(template_content, sizeof(template_content), req->path);
  if (strlen(template_content) == 0) {
    const char* not_found =
        "HTTP/1.1 404 Not Found" CRLF "Content-Type: text/plain" CRLF CRLF "404 Not Found";
    write(client_fd, not_found, strlen(not_found));
  } else {
    const char* header = "HTTP/1.1 200 OK" CRLF "Content-Type: text/html" CRLF CRLF;
    write(client_fd, header, strlen(header));
    write(client_fd, template_content, strlen(template_content));
  };
};

struct Route routes[] = {{"GET", INDEX_ROUTE_PATH, handle_home},
                         {"GET", ACCOUNTS_ROUTE_PATH, handle_accounts},
                         {"GET", "/api/accounts", handle_accounts},
                         {"GET", LOGIN_ROUTE_PATH, handle_login}};

void route_request(int client_fd, struct HttpRequest* req) {
  printf("%s", req->body);
  const int routers_count = sizeof(routes) / sizeof(routes[0]);
  int       router_found  = 0;
  for (int i = 0; i < routers_count; i++) {
    const struct Route route = routes[i];
    if ((strcmp(req->method, route.method) == 0) && (strcmp(req->path, route.path) == 0)) {
      route.handler(client_fd, req);
      router_found = 1;
    }
  }

  if (!router_found) {
    const char* not_found =
        "HTTP/1.1 404 Not Found" CRLF "Content-Type: text/plain" CRLF CRLF "404 Not Found";
    write(client_fd, not_found, strlen(not_found));
  }
}