#include "flow.h"

#include "acc.h"
#include "auth.h"
#include "colorization.h"
#include "menu.h"
#include "store.h"
#include "utils.h"

#include <stdbool.h>

bool pre_login_loop(int stop_flag) {
  bool running = true;
  pre_login_menu();
  int selected_opt          = 0;
  pre_login_loop_controller = 1;

  while (running && pre_login_loop_controller) {
    selected_opt = get_opt_input(PRE_LOGIN_MIN_CHOICE, PRE_LOGIN_MAX_CHOICE);
    switch (selected_opt) {
      case INVALID_CHOICE: {
        printf("\n❌ Invalid option! Please try again.\n\n");
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
        return true;
      }
      case 2: {
        const CreateUserDTO user = register_user_form();
        create_user(user);
        break;
      }
      case 3: {
        printf("\n👋 Thank you for using CBank System. See you soon!\n\n");
        stop_flag = 1;
        return false;
      }
    }
  }

  return false;
}

void post_login_loop(int stop_flag) {
  bool running = true;
  post_login_menu();
  int selected_opt = 0;

  while (running) {
    selected_opt = get_opt_input(POST_LOGIN_MIN_CHOICE, POST_LOGIN_MAX_CHOICE);
    switch (selected_opt) {
      case INVALID_CHOICE: {
        printf("\n❌ Invalid option! Please try again.\n\n");
        break;
      }
      case 1: {
        double balance = check_user_balance();
        printf("\n💰 Current Balance: R$ %.2lf\n\n", balance);
        break;
      }

      case 2: {
        char value[20];
        ask_null_terminated_input_str(value, sizeof(value), "Value to deposit: ");
        double numeric_v = atof(value);
        if (numeric_v <= 0) {
          printf("\n❌ Invalid deposit amount. Must be greater than zero.\n\n");
          break;
        }
        int result = make_deposit(numeric_v);
        if (result == 0) {
          printf("\n✅ Deposit of R$ %.2lf successful!\n\n", numeric_v);
        } else if (result == 1) {
          printf("\n⚠️  Deposit failed. Account not found.\n\n");
        } else {
          printf("\n❌ Deposit failed due to an internal error.\n\n");
        }
        break;
      }

      case 7: {
        RESET_ENTITY(logged_user);
        stop_flag = 1;
        return;
      }
    }
  }
}
