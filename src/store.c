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

void initialize_id_tracker_if_needed(const char* store_name) {
  if (!id_tracker_has_store(store_name)) {
    FILE* id_storage = db_instance(DB_ID_TRACKER_SECTION);
    if (id_storage == NULL) {
      printf("Failed to open ID tracker to initialize.\n");
      return;
    }

    fprintf(id_storage, "store=%s;cur_id=0;\n", store_name);
    fflush(id_storage);
    printf("✅ Initialized ID tracker for '%s'\n", store_name);
  }
}
void setup_stores() {
  stores.user_store.store_name = DB_USER_SECTION;
  stores.user_store.storage = db_instance(DB_USER_SECTION);
  check_sucessfull_storage_creation(stores.user_store.storage,
                                    stores.user_store.store_name);

  stores.id_tracker_store.store_name = DB_ID_TRACKER_SECTION;
  stores.id_tracker_store.storage = db_instance(DB_ID_TRACKER_SECTION);
  check_sucessfull_storage_creation(stores.id_tracker_store.storage,
                                    stores.id_tracker_store.store_name);

  initialize_id_tracker_if_needed(DB_USER_SECTION);
}

void mov_store_cursor(const char* store_name, int pos) {
  if (strcmp(store_name, stores.user_store.store_name) == 0) {
    fseek(stores.user_store.storage, 0, pos);
  }
}

FILE* get_storage(const char* store_name) {
  if (strcmp(store_name, DB_USER_SECTION) == 0)
    return stores.user_store.storage;

  if (strcmp(store_name, DB_ID_TRACKER_SECTION) == 0)
    return stores.id_tracker_store.storage;

  return NULL;
}

void updt_next_identity(const char* store_name) {
  FILE* id_storage = get_storage(DB_ID_TRACKER_SECTION);
  if (id_storage == NULL) return;
  char line_buf[256];

  const char* tmp_f_name = "id_tracker_tmp.db";
  FILE* tmp_f = fopen(tmp_f_name, "w");
  if (tmp_f == NULL) {
    printf("Failed to open temporary file for updating ID.\n");
    fclose(tmp_f);
    return;
  }

  const char* store_token = NULL;
  const char* curid_token = NULL;
  bool updated = false;

  while (fgets(line_buf, sizeof(line_buf), id_storage)) {
    store_token = strstr(line_buf, "store=");
    curid_token = strstr(line_buf, "cur_id=");
    if (!curid_token || !store_token) continue;

    char _tmp_store_name[50];
    int cur_id = 0;

    sscanf(store_token, "store=%49[^;];", _tmp_store_name);
    sscanf(curid_token, "cur_id=%d", &cur_id);

    if (strcmp(_tmp_store_name, store_name) == 0) {
      cur_id += 1;
      updated = true;
      fprintf(tmp_f, "store=%s;cur_id=%d;\n", _tmp_store_name, cur_id);
      continue;
    }

    fputs(line_buf, tmp_f);
  }

  fflush(tmp_f);
  fclose(tmp_f);

  fclose(id_storage);

  remove(DB_ID_TRACKER_SECTION);
  rename(tmp_f_name, DB_ID_TRACKER_SECTION);
  if (!updated) {
    printf("⚠️  Store '%s' not found in ID tracker.\n", store_name);
  }
}

int get_next_identity(const char* store_name) {
  mov_store_cursor(DB_USER_SECTION, SEEK_END);
  return 2654;
}
