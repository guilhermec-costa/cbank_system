#include "cli/flow.h"
#include "data/store.h"
#include "server/logger.h"
#include "server/server.h"

enum APP_MODE { CLI, SERVER };

struct Logger logger;

int main() {
  logger       = get_logger();
  logger.level = DEBUG;

  logger.log(&logger, DEBUG, "Initializing stores");
  setup_stores();

  struct ServerConfig server_cfg;
  server_cfg.domain          = AF_INET; // IPv4
  server_cfg.port            = PORT;
  server_cfg.socket_type     = SOCK_STREAM; // connection reliable
  server_cfg.protocol        = 0;           // default to TCP
  server_cfg.queue_s         = 10;
  server_cfg.in_address      = INADDR_ANY; // accepts from any host
  const struct Server server = make_server(server_cfg, start);

  int app_mode = SERVER;
  if (app_mode == SERVER) {
    server.start(&server);
  } else {
    int stop_flag = 0;
    while (true && !stop_flag) {
      const bool loop_success = pre_login_loop(&stop_flag);
      if (loop_success)
        post_login_loop();
    }
  }

  terminate_stores();
  return 0;
}
