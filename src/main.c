#include <stdio.h>
#include <string.h>

#include "auth.h"
#include "store.h"
#include "utils.h"

#define AQUAMARINE "\033[1;36m"
#define GOLD "\033[1;33m"
#define RESET_COLOR "\033[0m"

void show_menu() {
  colorize(AQUAMARINE);
  printf("----- Welcome to CBank System -----\n");
  colorize(GOLD);
  printf("1 - Login to my account\n");
  printf("2 - Register account\n");
  printf("3 - Quit system\n");
  colorize(AQUAMARINE);
  printf("-----------------------------------\n");
  colorize(RESET_COLOR);
}

int get_opt_input() {
  char action_opt[10];
  printf("\nEnter your choice -> ");
  fgets(action_opt, sizeof(action_opt), stdin);
  action_opt[strcspn(action_opt, "\n")] = '\0';
  return validate_choice(action_opt);
}

void app_loop() {
  bool running = true;
  show_menu();
  int selected_opt = 0;

  while (running) {
    selected_opt = get_opt_input();
    switch (selected_opt) {
      case INVALID_CHOICE: {
        printf("Invalid option! Try again\n");
        break;
      }
      case 1: {
        char tmp_id[NAME_MAX_CHAR_CONSTRAINT];
        char tmp_pwd[PWD_MAX_CHAR_CONSTRAINT];
        ask_terminated_input_str(tmp_id, sizeof(tmp_id), "Enter your ID -> ");
        ask_terminated_input_str(tmp_pwd, sizeof(tmp_pwd),
                                 "Enter your password -> ");

        AuthCredentials c;
        strcpy(c.id, tmp_id);
        strcpy(c.password, tmp_pwd);
        login(c);
        break;
      }
      case 2: {
        printf("Logging out");
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

int main() {
  setup_stores();
  app_loop();
  return 0;
};
