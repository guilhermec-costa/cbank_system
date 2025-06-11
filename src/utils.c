#include "utils.h"

#include <stdlib.h>

int validate_user_id(char* id) {}
int validate_input_pwd(char* pwd) {}

int validate_choice(const char* choice) {
  int choice_to_i = atoi(choice);
  if (choice_to_i == 0 || choice_to_i < MIN_CHOICE || choice_to_i > MAX_CHOICE)
    return INVALID_CHOICE;
  return choice_to_i;
}
