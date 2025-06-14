#include "store.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Stores stores;

FILE* db_instance(const char* f) {
  FILE* store = fopen(f, "a+");
  return store;
}

void check_sucessfull_storage_creation(FILE* storage,
                                       const char* storage_name) {
  if (storage == NULL) {
    printf("Failed to initialize \"%s\"", storage_name);
    exit(-1);
  };
  return;
}

void setup_stores() {
  stores.user_store.store_name = "user.db";
  stores.user_store.storage = db_instance(DB_USER_SECTION);
  check_sucessfull_storage_creation(stores.user_store.storage,
                                    stores.user_store.store_name);

  stores.id_tracker_store.store_name = "id_traker.db";
  stores.id_tracker_store.storage = db_instance(DB_ID_TRACKER_SECTION);
  check_sucessfull_storage_creation(stores.id_tracker_store.storage,
                                    stores.id_tracker_store.store_name);
}

void mov_store_cursor(const char* store_name, int pos) {
  if (strcmp(store_name, stores.user_store.store_name) == 0) {
    fseek(stores.user_store.storage, 0, pos);
  }
}

FILE* get_storage(const char* store_name) {
  if (strcmp(store_name, DB_USER_SECTION) == 0)
    return stores.user_store.storage;
  return NULL;
}

void updt_next_identity(const char* store_name) {
  FILE* id_storage = get_storage(DB_ID_TRACKER_SECTION);
  if (id_storage == NULL) return;
  mov_store_cursor(DB_ID_TRACKER_SECTION, SEEK_END);
}

int get_next_identity(const char* store_name) {
  mov_store_cursor(DB_USER_SECTION, SEEK_END);
  return 2654;
}
