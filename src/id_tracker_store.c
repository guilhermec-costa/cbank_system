#include <string.h>

#include "store.h"

bool id_tracker_has_store(const char* store_name) {
  FILE* id_storage = get_storage(DB_ID_TRACKER_SECTION);
  if (id_storage == NULL) return false;

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
