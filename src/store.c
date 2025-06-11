#include "store.h"

Stores stores;

FILE* db_instance(const char* f) {
  FILE* store = fopen(f, "a+");
  return store;
}

void setup_stores() {
  stores.user_store = db_instance(DB_USER_SECTION);
  if (stores.user_store == NULL) {
    printf("Failed to initialize user store");
  }
}
