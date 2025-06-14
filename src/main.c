#include <stdio.h>

#include "auth.h"
#include "colorization.h"
#include "store.h"
#include "utils.h"

static int pre_login_loop_controller = 0;
static int post_login_loop_controller = 0;

void pre_login_menu() {
    printf("\n%s=========================================\n", COLOR_CYAN);
    printf("         💰 Welcome to CBank System\n");
    printf("=========================================%s\n\n", COLOR_RESET);

    printf("%s👉 Select an option:%s\n\n", COLOR_GREEN, COLOR_RESET);

    printf("%s1%s - 🔐 Login to my account\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s2%s - 📝 Register a new account\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s3%s - 🚪 Quit system\n", COLOR_YELLOW, COLOR_RESET);

    printf("\n%s-----------------------------------------%s\n", COLOR_CYAN, COLOR_RESET);
}

void post_login_menu() {
  colorize(COLOR_AQUAMARINE);
  printf("----- Welcome to CBank System -----\n");
  colorize(COLOR_YELLOW);
  printf("1 - See balance\n");
  colorize(COLOR_AQUAMARINE);
  printf("-----------------------------------\n");
  colorize(COLOR_RESET);
}

int get_opt_input() {
  char action_opt[10];
  printf("\n%s👉 Enter your choice:%s ", COLOR_GREEN, COLOR_RESET);
  fgets(action_opt, sizeof(action_opt), stdin);
  terminate_str_by_newline(action_opt);
  return validate_choice(action_opt);
}

void pre_login_loop() {
  bool running = true;
  pre_login_menu();
  int selected_opt = 0;
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
          printf("Failed to perform login");
          exit(-1);
        }
        pre_login_loop_controller = 0;
        post_login_loop_controller = 1;
        break;
      }
      case 2: {
        const CreateUserDTO user = register_user_form();
        create_user(user);
        break;
      }
      case 3: {
        printf("Thanks for using the system\n");
        running = false;
        break;
      }
    }
  }
};

void post_login_loop() {
  bool running = true;
  post_login_menu();
  int selected_opt = 0;

  while (running && post_login_loop_controller) {
    printf("After login loop");
    selected_opt = get_opt_input();
    switch (selected_opt) {
      case INVALID_CHOICE: {
        printf("Invalid option! Try again\n");
        break;
      }
    }
  }
}

int main() {
  setup_stores();
  pre_login_loop();
  post_login_loop();
  return 0;
}
