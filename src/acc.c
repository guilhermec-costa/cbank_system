#include <stdio.h>

#include "acc.h"
#include "models.h"

extern BankUser logged_user;

int check_user_balance() {
  printf("Logged user email: %s", logged_user.email);
  return 1;
}