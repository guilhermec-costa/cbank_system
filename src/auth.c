#include "auth.h"

#include <stdio.h>

void login(struct AuthCredentials c) {
  printf("Id: %s", c.id);
  printf("Password: %s", c.password);
}
