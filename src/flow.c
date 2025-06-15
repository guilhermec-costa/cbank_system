#include "flow.h"

#include "auth.h"
#include "colorization.h"
#include "menu.h"
#include "store.h"
#include "utils.h"

#include <stdbool.h>

bool pre_login_loop() {
  bool running = true;
  pre_login_menu();
  int selected_opt          = 0;
  pre_login_loop_controller = 1;

  while (running && pre_login_loop_controller) {
    selected_opt = get_opt_input();
    switch (selected_opt) {
      case INVALID_CHOICE: {
        printf("Invalid option! Try again\n");
        break;
      }
      case 1: {
        bool logged = trigger_login_process();
        if (!logged) {
          printf("\n%s=========================================\n", COLOR_RED);
          printf("       🚫 Maximum login attempts reached\n");
          printf("=========================================%s\n\n", COLOR_RESET);
          printf("%s⚠️  For security reasons, the application has been terminated.%s\n",
                 COLOR_YELLOW, COLOR_RESET);
          printf("%s👉 Please restart the application to try again.%s\n\n", COLOR_CYAN,
                 COLOR_RESET);
          return false;
        }
        pre_login_loop_controller  = 0;
        post_login_loop_controller = 1;
        return true;
      }
      case 2: {
        const CreateUserDTO user = register_user_form();
        create_user(user);
        break;
      }
      case 3: {
        printf("Thanks for using the system\n");
        return false;
      }
    }
  }
}

void post_login_loop() {
  bool running = true;
  post_login_menu();
  int selected_opt = 0;

  while (running && post_login_loop_controller) {
    selected_opt = get_opt_input();
    switch (selected_opt) {
      case INVALID_CHOICE: {
        printf("Invalid option! Try again\n");
        break;
      }
    }
  }
}
