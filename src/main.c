#include "flow.h"
#include "store.h"

int main() {
  setup_stores();
  int stop_flag = 0;
  while (true && !stop_flag) {
    const bool loop_success = pre_login_loop(&stop_flag);
    if (loop_success)
      post_login_loop();
  }

  terminate_stores();
  return 0;
}
