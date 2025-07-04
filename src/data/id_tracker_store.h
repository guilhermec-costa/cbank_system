#ifndef CBANK_ID_TRACKER_STORE_H
#define CBANK_ID_TRACKER_STORE_H

#include <stdbool.h>

bool id_tracker_has_store(const char* store_name);
void initialize_id_tracker_if_needed(const char* store_name);

#endif /* CBANK_ID_TRACKER_STORE_H */