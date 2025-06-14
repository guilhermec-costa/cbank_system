#ifndef CBANK_STORE_H
#define CBANK_STORE_H

#include <stdio.h>
#include "models.h"

#define DB_USER_SECTION "user.db"

typedef struct _stores {
  struct UserStore {
    const char* store_name;
    FILE* storage;
  } user_store;
} Stores;

FILE* db_instance(const char* f);

void setup_stores();
void terminate_stores();
BankUser get_user_by_id(const char* id);
void reset_entity_attr_mem(void* ent, size_t ent_s);
void updt_next_identity(const char* store_name);

#define RESET_ENTITY(entity) reset_entity_attr_mem(&(entity), sizeof(entity))
#define UPDATE_ENTITY_NEXT_IDENTIY(store) updt_next_identity(store);

typedef enum storage_file_pos {
  START = SEEK_SET,
  END = SEEK_END
} StorageFilePos;

#endif /* CBANK_STORE_H */
