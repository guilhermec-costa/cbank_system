#include "store.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "models.h"

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

void mov_store_cursor(const char* store_name, StorageFilePos pos) {
  if (strcmp(store_name, stores.user_store.store_name) == 0) {
    fseek(stores.user_store.storage, 0, pos);
  }
}

BankUser get_user_by_account_id(const char* id) {}

BankUser get_user_by_id(const char* id) {
  char line_buf[256];
  BankUser user;
  RESET_ENTITY(user);
  mov_store_cursor(stores.user_store.store_name, START);

  char* id_token = NULL;
  char* pwd_token = NULL;
  while (fgets(line_buf, sizeof(line_buf), stores.user_store.storage)) {
    id_token = strstr(line_buf, "id=");
    pwd_token = strstr(line_buf, "pwd=");

    if (!id_token || !pwd_token) {
      continue;
    }

    char _tmp_id[ACC_ID_MAX_CHAR_CONSTRAINT];
    sscanf(id_token, "id=%9[^;];", _tmp_id);

    if (strcmp(_tmp_id, id) == 0) {
      strcpy(user.id, _tmp_id);

      char _tmp_pwd[PWD_MAX_CHAR_CONSTRAINT];
      sscanf(pwd_token, "pwd=%34[^;];", _tmp_pwd);
      strcpy(user.password, _tmp_pwd);

      return user;
    }
  }

  strcpy(user.id, "");
  return user;
}

bool email_already_registered(const char* email) {
  char line_buf[256];
  mov_store_cursor(stores.user_store.store_name, START);

  char* email_token = NULL;
  while (fgets(line_buf, sizeof(line_buf), stores.user_store.storage)) {
    email_token = strstr(line_buf, "email=");
    if (!email_token) continue;

    char _tmp_email[REGISTRATION_EMAIL_MAX_CHAR_CONSTRAINT];
    sscanf(email_token, "email=%49[^;];", _tmp_email);

    if (strcmp(_tmp_email, email) == 0) {
      return true;
    }
  }
  return false;
}

FILE* get_storage(const char* store_name) {
  if (strcmp(store_name, DB_USER_SECTION) == 0)
    return stores.user_store.storage;

  return NULL;
}

void updt_next_identity(const char* store_name) {
  FILE* entity_storage = get_storage(store_name);
  if (entity_storage == NULL) return;
  mov_store_cursor(store_name, END);
}

int get_next_identity(const char* store_name) {
  mov_store_cursor(DB_USER_SECTION, END);
  return 2654;
}
