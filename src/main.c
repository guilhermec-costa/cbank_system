#include <stdio.h>
#include <stdbool.h>

struct ClientCredentials {
  char name[35];
  char password[20];
};

int validate_opt_input(int* action_opt) {
  int tmp_opt = 0;
  const numerical = scanf("Type option: %d", &tmp_opt);
  if(!numerical) return 0;
  if(tmp_opt < 1 || tmp_opt > 2) return 0;

  return 1;
}

void app_loop() {
  bool running = true;
  int action_opt = 0;
  while(running) {
    printf("----- Welcome to CBank System -----\n");
    printf("1 - Login to my account\n");
    printf("2 - Quit system\n");
    printf("-----------------------------------\n");
    validate_opt_input(&action_opt);
  }
};

int main() {
  app_loop();
  return 0;
};