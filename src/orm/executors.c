#include "executors.h"

#include "../data/store.h"

#include <stdlib.h>
#include <string.h>

int select_executor(SelectQuery* q, char*** results, int* rows, int* cols) {
  FILE* f = open_store_on_modes(q->table, "r");
};

SelectQuery* new_select_query() {
  SelectQuery* q = calloc(1, sizeof(SelectQuery));
  memset(q, 0, sizeof(SelectQuery));
  q->select  = qselect;
  q->where   = qwhere;
  q->from    = qfrom;
  q->execute = select_executor;
  return q;
}