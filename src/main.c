#include "flow.h"
#include "store.h"

int main() {
  setup_stores();
  const bool loop_success = pre_login_loop();
  if (loop_success)
    post_login_loop();

  terminate_stores();
  return 0;
}
