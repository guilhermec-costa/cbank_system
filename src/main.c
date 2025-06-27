#include "data/store.h"
#include "server/logger.h"
#include "server/server.h"

enum APP_MODE { CLI, SERVER };

int main() {
  GLOBAL_LOGGER->log(GLOBAL_LOGGER, DEBUG, "Setting up data stores");
  setup_stores();

  struct ServerConfig server_cfg;
  server_cfg.domain      = AF_INET; // IPv4
  server_cfg.port        = PORT;
  server_cfg.socket_type = SOCK_STREAM; // connection reliable
  server_cfg.protocol    = 0;           // default to TCP
  server_cfg.queue_s     = 10;
  server_cfg.in_address  = INADDR_ANY; // accepts from any host

  const struct Server server = make_server(server_cfg, start);
  server.start(&server);
  terminate_stores();
  return 0;
}
