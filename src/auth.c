#include "auth.h"

#include <stdio.h>

#include "store.h"

extern Stores stores;

void login(AuthCredentials c) {
  printf("Id: %s", c.id);
  printf("Password: %s", c.password);
  fprintf(stores.user_store, "Id: %s\n", c.id);
}
