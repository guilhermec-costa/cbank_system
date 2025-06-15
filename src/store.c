#include "store.h"
#include "colorization.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

Stores stores;

FILE* db_instance(const char* f) {
  FILE* store = fopen(f, "a+");
  return store;
}

void check_sucessfull_storage_creation(FILE* storage, const char* storage_name) {
  if (storage == NULL) {
    printf("\n%s❌ Failed to initialize storage \"%s\"%s\n", COLOR_RED, storage_name, COLOR_RESET);
    printf("%s⚠️  Check if the file or directory exists and has correct permissions.%s\n\n", COLOR_YELLOW, COLOR_RESET);
    exit(1);
  };
  return;
}

void open_store(Store* store, const char* name) {
  store->store_name = name;
  store->storage = db_instance(name);
  check_sucessfull_storage_creation(store->storage, store->store_name);
};

void setup_stores() {
  if(mkdir("stores", 0777) == -1) {
    if(errno != EEXIST) {
      perror("Failed to create stores directory");
      exit(EXIT_FAILURE);
    }
  };

  open_store(&stores.id_tracker_store, DB_ID_TRACKER_SECTION);
  open_store(&stores.user_store, DB_USER_SECTION);
  open_store(&stores.account_store, DB_ACCOUNT_SECTION);
  open_store(&stores.transaction_store, DB_TRANSACTION_SECTION);

  initialize_id_tracker_if_needed(DB_USER_SECTION);
  initialize_id_tracker_if_needed(DB_TRANSACTION_SECTION);
  initialize_id_tracker_if_needed(DB_ACCOUNT_SECTION);
}

void terminate_stores() {
  fclose(stores.user_store.storage);
  fclose(stores.id_tracker_store.storage);
}

void mov_store_cursor(const char* store_name, int pos) {
  if (strcmp(store_name, stores.user_store.store_name) == 0) {
    fseek(stores.user_store.storage, 0, pos);
  }
  if (strcmp(store_name, stores.id_tracker_store.store_name) == 0) {
    fseek(stores.id_tracker_store.storage, 0, pos);
  }
}

FILE* get_storage(const char* store_name) {
  if (strcmp(store_name, DB_USER_SECTION) == 0) {
    mov_store_cursor(DB_USER_SECTION, SEEK_SET);
    return stores.user_store.storage;
  }

  if (strcmp(store_name, DB_ID_TRACKER_SECTION) == 0) {
    mov_store_cursor(DB_ID_TRACKER_SECTION, SEEK_SET);
    return stores.id_tracker_store.storage;
  }

  return NULL;
}

void updt_next_identity(const char* store_name) {
  FILE* id_storage = get_storage(DB_ID_TRACKER_SECTION);
  char  line_buf[256];

  const char* tmp_f_name = "id_tracker_tmp.db";
  FILE*       tmp_f      = fopen(tmp_f_name, "w");
  if (tmp_f == NULL) {
    printf("Failed to open temporary file for updating ID.\n");
    return;
  }

  const char* store_token = NULL;
  const char* curid_token = NULL;
  bool        updated     = false;

  while (fgets(line_buf, sizeof(line_buf), id_storage)) {
    store_token = strstr(line_buf, "store=");
    curid_token = strstr(line_buf, "cur_id=");
    if (!curid_token || !store_token) {
      fputs(line_buf, tmp_f);
      continue;
    }

    char _tmp_store_name[50];
    int  cur_id = 0;

    sscanf(store_token, "store=%49[^;];", _tmp_store_name);
    sscanf(curid_token, "cur_id=%d", &cur_id);

    if (strcmp(_tmp_store_name, store_name) == 0) {
      cur_id += 1;
      updated = true;
      fprintf(tmp_f, "store=%s;cur_id=%d;\n", _tmp_store_name, cur_id);
    } else {
      fprintf(tmp_f, "store=%s;cur_id=%d;\n", _tmp_store_name, cur_id);
    }
  }

  fclose(tmp_f);
  remove(DB_ID_TRACKER_SECTION);
  rename(tmp_f_name, DB_ID_TRACKER_SECTION);

  fclose(id_storage);
  // very IMPORTANT
  stores.id_tracker_store.storage = db_instance(DB_ID_TRACKER_SECTION);
  if (!updated) {
    printf("⚠️  Store '%s' not found in ID tracker.\n", store_name);
  }
}

int get_next_identity(const char* store_name) {
  FILE* id_storage = get_storage(DB_ID_TRACKER_SECTION);
  if (id_storage == NULL)
    return false;

  char line_buf[256];

  const char* store_token = NULL;
  const char* curid_token = NULL;
  while (fgets(line_buf, sizeof(line_buf), id_storage)) {
    store_token = strstr(line_buf, "store=");
    curid_token = strstr(line_buf, "cur_id=");
    if (!curid_token || !store_token)
      continue;

    char _tmp_store_name[50];
    int  cur_id = 0;
    sscanf(store_token, "store=%49[^;];", _tmp_store_name);
    if (strcmp(_tmp_store_name, store_name) == 0) {
      sscanf(curid_token, "cur_id=%d", &cur_id);
      return cur_id;
    }
  }

  return -1;
}
