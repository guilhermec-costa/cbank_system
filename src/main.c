#include "data/store.h"
#include "secret/manager.h"
#include "server/logger.h"
#include "server/router.h"
#include "server/routes.h"
#include "server/server.h"

int main() {
  GLOBAL_LOGGER->log(GLOBAL_LOGGER, DEBUG, "Setting up data stores");
  setup_stores();
  load_env(".env");

  struct ServerConfig server_cfg;
  server_cfg.domain      = AF_INET; // IPv4
  server_cfg.port        = PORT;
  server_cfg.socket_type = SOCK_STREAM; // connection reliable
  server_cfg.protocol    = 0;           // default to TCP
  server_cfg.queue_s     = 10;
  server_cfg.in_address  = INADDR_ANY; // accepts from any host

  RouteRegistry route_registry;
  init_route_registry(&route_registry);

  size_t route_count = sizeof(routes) / sizeof(routes[0]);
  for (size_t i = 0; i < route_count; i++) {
    register_route(&route_registry, routes[i]);
  }

  struct Server server = make_server(server_cfg, start, route_registry);
  server.start(&server);

  terminate_stores();
  free_route_registry(&route_registry);
  free_secrets();
  return 0;
}
