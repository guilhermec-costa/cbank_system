#include "store.h"

#include "../colorization.h"
#include "../orm/select_query.h"
#include "../server/logger.h"
#include "../utils.h"
#include "id_tracker_store.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

Stores stores;

FILE* db_instance(const char* f) {
  FILE* store = fopen(f, "a+");
  return store;
}

void check_sucessfull_storage_creation(FILE* storage, const char* storage_name) {
  if (storage == NULL) {
    printf("\n%s❌ Failed to initialize storage \"%s\"%s\n", COLOR_RED, storage_name, COLOR_RESET);
    printf("%s⚠️  Check if the file or directory exists and has correct permissions.%s\n\n",
           COLOR_YELLOW, COLOR_RESET);
    exit(1);
  };
  return;
}

void create_store(Store* store, const char* name) {
  store->store_name = name;
  store->storage    = db_instance(name);
  check_sucessfull_storage_creation(store->storage, store->store_name);
};

void setup_stores() {
  GLOBAL_LOGGER->log(GLOBAL_LOGGER, DEBUG, "Setting up data stores");
  if (mkdir("stores", 0777) == -1) {
    if (errno != EEXIST) {
      perror("Failed to create stores directory");
      exit(EXIT_FAILURE);
    }
  };

  create_store(&stores.id_tracker_store, DB_ID_TRACKER_SECTION);
  create_store(&stores.user_store, DB_USER_SECTION);
  create_store(&stores.account_store, DB_ACCOUNT_SECTION);
  create_store(&stores.transaction_store, DB_TRANSACTION_SECTION);

  initialize_id_tracker_if_needed(DB_USER_SECTION);
  initialize_id_tracker_if_needed(DB_TRANSACTION_SECTION);
  initialize_id_tracker_if_needed(DB_ACCOUNT_SECTION);
}

void terminate_stores() {
  fclose(stores.user_store.storage);
  fclose(stores.id_tracker_store.storage);
  fclose(stores.account_store.storage);
  fclose(stores.transaction_store.storage);
}

void mov_store_cursor(const char* store_name, int pos) {
  if (strcmp(store_name, stores.user_store.store_name) == 0)
    fseek(stores.user_store.storage, 0, pos);
  if (strcmp(store_name, stores.id_tracker_store.store_name) == 0)
    fseek(stores.id_tracker_store.storage, 0, pos);
  if (strcmp(store_name, stores.account_store.store_name) == 0)
    fseek(stores.account_store.storage, 0, pos);
  if (strcmp(store_name, stores.transaction_store.store_name) == 0)
    fseek(stores.transaction_store.storage, 0, pos);
}

FILE* get_storage_for_reading(const char* store_name) {
  if (strcmp(store_name, DB_USER_SECTION) == 0) {
    mov_store_cursor(DB_USER_SECTION, SEEK_SET);
    return stores.user_store.storage;
  }

  if (strcmp(store_name, DB_ID_TRACKER_SECTION) == 0) {
    mov_store_cursor(DB_ID_TRACKER_SECTION, SEEK_SET);
    return stores.id_tracker_store.storage;
  }

  if (strcmp(store_name, DB_ACCOUNT_SECTION) == 0) {
    mov_store_cursor(DB_ACCOUNT_SECTION, SEEK_SET);
    return stores.account_store.storage;
  }

  if (strcmp(store_name, DB_TRANSACTION_SECTION) == 0) {
    mov_store_cursor(DB_TRANSACTION_SECTION, SEEK_SET);
    return stores.transaction_store.storage;
  }

  return NULL;
}

void updt_next_identity(const char* store_name) {
  FILE* id_storage = get_storage_for_reading(DB_ID_TRACKER_SECTION);
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
    terminate_str_by_newline(line_buf);
    store_token = strstr(line_buf, "store=");
    curid_token = strstr(line_buf, "cur_id=");
    if (!curid_token || !store_token) {
      fputs(line_buf, tmp_f);
      continue;
    }

    char _tmp_store_name[50];
    int  cur_id = 0;

    sscanf(store_token, "store=%49[^;];", _tmp_store_name);
    sscanf(curid_token, "cur_id=%d[^;];", &cur_id);

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

bool get_next_identity(const char* store_name, char* out_buf, size_t buf_size) {
  SelectQuery* q = new_select_query();
  q->select(q, "cur_id")->from(q, DB_ID_TRACKER_SECTION)->where(q, "store", "=", store_name);
  ResultSet* result = q->execute(q);
  if (!result->data)
    return false;
  const char* id = result->get_row_col_value(result, 0, "cur_id");
  snprintf(out_buf, buf_size, "%s", id);
  result->free(result);
  q->destroy(q);
  return true;
}
