#include "server.h"

#include "http_parser.h"
#include "logger.h"
#include "route_contants.h"
#include "router.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 8080

void set_nonblocking(int fd) {}

void start(const struct Server* server) {
  GLOBAL_LOGGER->log(GLOBAL_LOGGER, INFO, "Starting server");
  char client_buf[CLIENT_BUFFER_SIZE] = {0};
  while (1) {
    int                client_fd;
    struct sockaddr_in client_addr;
    int                addrlen = sizeof(client_addr);

    if ((client_fd =
             accept(server->socket_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen)) < 0) {
      perror("accept failed");
      continue;
    }

    GLOBAL_LOGGER->log(GLOBAL_LOGGER, DEBUG, "Client connected");

    memset(client_buf, 0, sizeof(client_buf));
    ssize_t bytes_read = read(client_fd, client_buf, sizeof(client_buf) - 1);

    if (bytes_read <= 0) {
      GLOBAL_LOGGER->log(GLOBAL_LOGGER, DEBUG, "Client closed connection");
      close(client_fd);
      continue;
    }

    client_buf[bytes_read] = '\0';

    struct HttpRequest  req = {0};
    struct HttpResponse res = {0};

    if (parse_req_line(client_buf, &req) == 0) {
      const char* headers_start = strstr(client_buf, CRLF) + 2;
      const char* body_start    = parse_req_headers(headers_start, &req);
      parse_req_body(body_start, &req);
      const struct RouteValidationResponse _route = get_route(&req, &res);
      if (!_route.valid) {
        if (_route.error_flag == NOT_FOUND_FLAG) {
          GLOBAL_LOGGER->log(GLOBAL_LOGGER, ERROR, "Not found");
          send_404_response(client_fd, &res);
        }
        if (_route.error_flag == METHOD_NOT_ALLOWED_FLAG) {
          GLOBAL_LOGGER->log(GLOBAL_LOGGER, DEBUG, "Method not allowed");
          send_not_allowed_response(client_fd, &res);
        }
        end_client_conn(client_fd, &req, &res);
        GLOBAL_LOGGER->log(GLOBAL_LOGGER, DEBUG, "Client connection closed");
        continue;
      }

      bool any_mw_fail = false;
      for (int i = 0; i < _route.route.middleware_count; i++) {
        _route.route.middlewares[i](&req, &res);
        if (res.status_code == HTTP_UNAUTHORIZED) {
          redirect(client_fd, &res, LOGIN_ROUTE_PATH);
          end_client_conn(client_fd, &req, &res);
          break;
        }
        if (res.status_code >= 400) {
          any_mw_fail = true;
          send_http_response(client_fd, &res);
          GLOBAL_LOGGER->log(GLOBAL_LOGGER, INFO,
                             "Middleware responded with bad status code! closing connection");
          end_client_conn(client_fd, &req, &res);
          break;
        }
      }
      if (!any_mw_fail) {
        _route.route.handler(client_fd, &req, &res);
      }
    } else {
      GLOBAL_LOGGER->log(GLOBAL_LOGGER, ERROR, "Failed to parse request line.");
      send_bad_request_response(client_fd, &res);
    };

    GLOBAL_LOGGER->log(GLOBAL_LOGGER, DEBUG, "Response sent! closing connection");
    end_client_conn(client_fd, &req, &res);
  }

  close(server->socket_fd);
}

struct Server make_server(struct ServerConfig cfg, void (*start)(const struct Server*)) {
  struct Server server;
  server.cfg = cfg;

  server.sock_address.sin_family      = cfg.domain;
  server.sock_address.sin_port        = htons(cfg.port);
  server.sock_address.sin_addr.s_addr = htonl(cfg.in_address);

  if ((server.socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  int sockopt = 1;
  // prevents socket already binded
  if (setsockopt(server.socket_fd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) < 0) {
    perror("setsocketopt failed");
    exit(EXIT_FAILURE);
  }

  if (bind(server.socket_fd, (struct sockaddr*)&server.sock_address, sizeof(server.sock_address)) <
      0) {
    perror("socket bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server.socket_fd, server.cfg.queue_s) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  GLOBAL_LOGGER->log(GLOBAL_LOGGER, INFO, "Server listening on port %d", SERVER_PORT);

  server.start = start;

  return server;
};

void end_client_conn(int client_fd, struct HttpRequest* req, struct HttpResponse* res) {
  free_query_params_list(&req->body_query_params_list);
  free_query_params_list(&req->url_query_params_list);
  close(client_fd);
}