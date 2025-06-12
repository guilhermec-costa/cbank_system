#include "store.h"
#include "models.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Stores stores;

FILE* db_instance(const char* f) {
  FILE* store = fopen(f, "a+");
  return store;
}

void setup_stores() {
  stores.user_store.store_name = "user.db";
  stores.user_store.storage = db_instance(DB_USER_SECTION);

  if (stores.user_store.storage == NULL) {
    printf("Failed to initialize user store");
    exit(-1);
  }
}

void reset_store_cursor_pos(const char* store_name) {
  if(strcmp(store_name, stores.user_store.store_name)) {
    fseek(stores.user_store.storage, 0, SEEK_SET);
  } 
  return;
}

BankUser get_user_by_id(const char* id) {
  char line_buf[256];
  BankUser user;
  RESET_ENTITY(user);
  reset_store_cursor_pos(stores.user_store.store_name);

  char* id_token = NULL;
  char* pwd_token = NULL;
  while (fgets(line_buf, sizeof(line_buf), stores.user_store.storage)) {
    id_token = strstr(line_buf, "id=");
    pwd_token = strstr(line_buf, "pwd=");

    if(!id_token || !pwd_token) {
      continue;
    }
    
    char _tmp_id[REGISTRATION_ID_MAX_CHAR_CONSTRAINT];
    sscanf(id_token, "id=%34[^;];", _tmp_id); 

    if(strcmp(_tmp_id, id) == 0) {
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
