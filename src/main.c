#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "auth.h"
#include "utils.h"

void menu() {
  printf("\033[1;36m");
  printf("----- Welcome to CBank System -----\n");
  printf("\033[1;33m");
  printf("1 - Login to my account\n");
  printf("2 - Register account\n");
  printf("3 - Quit system\n");
  printf("\033[1;36m");
  printf("-----------------------------------\n");
  printf("\033[0m");
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
  menu();
  int opt = 0;

  while (running) {
    opt = get_opt_input();
    switch (opt) {
      case INVALID_CHOICE: {
        printf("Invalid option! Try again\n");
        break;
      }
      case 1: {
        char tmp_id[NAME_MAX_CHAR_CONSTRAINT];
        printf("Enter your ID -> ");
        fgets(tmp_id, sizeof(tmp_id), stdin);
        tmp_id[strcspn(tmp_id, "\n")] = '\0';
        validate_user_id(tmp_id);

        char tmp_pwd[PWD_MAX_CHAR_CONSTRAINT];
        printf("Enter your password -> ");
        fgets(tmp_pwd, sizeof(tmp_pwd), stdin);
        tmp_pwd[strcspn(tmp_pwd, "\n")] = '\0';

        struct AuthCredentials c = {};
        strcpy(c.id, tmp_id);
        strcpy(c.password, tmp_pwd);
        login(c);
        break;
      }
      case 2: {
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
  app_loop();
  return 0;
};
