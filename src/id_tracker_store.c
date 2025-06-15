#include "store.h"

#include <string.h>

bool id_tracker_has_store(const char* store_name) {
  FILE* id_storage = get_storage(DB_ID_TRACKER_SECTION);
  if (id_storage == NULL)
    return false;

  char line_buf[256];

  const char* store_token = NULL;
  while (fgets(line_buf, sizeof(line_buf), id_storage)) {
    store_token = strstr(line_buf, "store=");
    if (store_token) {
      char _tmp_store_name[50];
      sscanf(store_token, "store=%49[^;];", _tmp_store_name);
      if (strcmp(_tmp_store_name, store_name) == 0) {
        return true;
      }
    }
  }

  return false;
}

void initialize_id_tracker_if_needed(const char* store_name) {
  if (!id_tracker_has_store(store_name)) {
    FILE* id_storage = get_storage(DB_ID_TRACKER_SECTION);
    if (id_storage == NULL) {
      printf("Failed to open ID tracker to initialize.\n");
      return;
    }

    fprintf(id_storage, "store=%s;cur_id=0;\n", store_name);
    fflush(id_storage);
    printf("✅ Initialized ID tracker for '%s'\n", store_name);
  }
}
