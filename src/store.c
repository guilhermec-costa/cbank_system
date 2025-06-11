#include "store.h"

#include <stdlib.h>
#include <string.h>

Stores stores;

FILE* db_instance(const char* f) {
  FILE* store = fopen(f, "a+");
  return store;
}

void setup_stores() {
  stores.user_store = db_instance(DB_USER_SECTION);
  if (stores.user_store == NULL) {
    printf("Failed to initialize user store");
    exit(-1);
  }
}

void get_user_by_id(const char* id) {
  char query_container[50];
  while (fscanf(stores.user_store, "%s", query_container) == 1) {
    if (strstr(query_container, id) != 0) {
      printf("Query container: %s", query_container);
    }
  }
}
