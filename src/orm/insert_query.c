#define _POSIX_C_SOURCE 200809L

#include "insert_query.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

InsertQuery* set_for_insert(InsertQuery* q, const char* column, const char* value) {
  if (!q || q->set_count >= MAX_INSERT_PAIRS)
    return q;

  q->insert_pair[q->set_count++] = (InsertPair){.col = strdup(column), .value = strdup(value)};

  return q;
};

static void free_insert_query(InsertQuery* q) {
  if (!q)
    return;

  for (int i = 0; i < q->set_count; i++) {
    free(q->insert_pair[i].col);
    free(q->insert_pair[i].value);
  }
  free(q->insert_pair);
  free(q);
};

int insert_query_executor(InsertQuery* q) {
  if (!q || !q->insert_pair || q->set_count == 0) {
    fprintf(stderr, "Invalid insert query");
    return 1;
  }

  FILE* f = fopen(q->table, "a+");
  if (!f) {
    printf("Failed to open file: %s\n", q->table);
    return 1;
  }
  fseek(f, 0, SEEK_END);

  for (int i = 0; i < q->set_count; i++) {
    fprintf(f, "%s=%s", q->insert_pair[i].col, q->insert_pair[i].value);
    if (i < q->set_count - 1) {
      fprintf(f, ";");
    }
  }
  fprintf(f, "\n");
  fflush(f);
  fclose(f);
  return 0;
}

InsertQuery* new_insert_query() {
  InsertQuery* q = calloc(1, sizeof(InsertQuery));
  q->insert_pair = calloc(MAX_INSERT_PAIRS, sizeof(InsertPair));
  q->set_count   = 0;
  q->set         = set_for_insert;
  q->destroy     = free_insert_query;
  return q;
}