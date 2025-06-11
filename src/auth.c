#include <stdio.h>
#include "auth.h"

void login(struct AuthCredentials c) {
  printf("%s", c.name);
}
