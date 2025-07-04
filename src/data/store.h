#ifndef CBANK_STORE_H
#define CBANK_STORE_H

#include "models.h"

#include <stdio.h>

#define DB_USER_SECTION "./stores/user_db"
#define DB_ID_TRACKER_SECTION "./stores/id_tracker_db"
#define DB_ACCOUNT_SECTION "./stores/account_db"
#define DB_TRANSACTION_SECTION "./stores/transaction_db"

typedef struct Store {
  const char* store_name;
  FILE*       storage;
} Store;

typedef struct _stores {
  Store user_store;
  Store id_tracker_store;
  Store account_store;
  Store transaction_store;
} Stores;

extern Stores stores;

FILE* db_instance(const char* f);

void setup_stores();
void terminate_stores();
void reset_entity_attr_mem(void* ent, size_t ent_s);
void updt_next_identity(const char* store_name);
void mov_store_cursor(const char* store_name, int pos);
bool get_next_identity(const char* store_name, char* out_buf, size_t buf_size);

FILE* get_storage_for_reading(const char* store_name);

#define RESET_ENTITY(entity) reset_entity_attr_mem(&(entity), sizeof(entity))

#define NEXT_USER_IDENTITY get_next_identity(DB_USER_SECTION);
#define NEXT_ACCOUNT_IDENTITY get_next_identity(DB_ACCOUNT_SECTION);

// COLUMNS NAME DEFINITIONS
#endif /* CBANK_STORE_H */
