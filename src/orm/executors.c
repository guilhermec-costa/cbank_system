#include "query.h"
#define _POSIX_C_SOURCE 200809L

#include "../data/store.h"
#include "executors.h"

#include <stdlib.h>
#include <string.h>

bool compare(const char* left, const char* op, const char* right) {
  if (strcmp(op, "=") == 0) {
    return strcmp(left, right) == 0;
  } else if (strcmp(op, "!=") == 0) {
    return strcmp(left, right) != 0;
  } else if (strcmp(op, ">") == 0) {
    return atof(left) > atof(right);
  } else if (strcmp(op, "<") == 0) {
    return atof(left) < atof(right);
  } else if (strcmp(op, ">=") == 0) {
    return atof(left) >= atof(right);
  } else if (strcmp(op, "<=") == 0) {
    return atof(left) <= atof(right);
  } else {
    return false;
  }
};

bool apply_select_where(SelectQuery* q, const char* line_buf) {
  if (q->condition_count == 0)
    return true;

  char line_cp[1024];
  strncpy(line_cp, line_buf, sizeof(line_cp) - 1);
  line_cp[sizeof(line_cp) - 1] = '\0';

  char* rest = line_cp;
  char* token;

  while ((token = strtok_r(rest, ";", &rest))) {
    char* _eq_pos = strchr(token, '=');
    if (!_eq_pos)
      continue;
    *_eq_pos          = '\0';
    const char* key   = token;
    const char* value = _eq_pos + 1;

    for (int i = 0; i < q->condition_count; i++) {
      WhereCondition clause = q->conditions[i];
      if (strcmp(clause.column, key) == 0) {
        if (!compare(value, clause.operator, clause.value)) {
          return false;
        }
      }
    }
  }

  return true;
};

int select_executor(SelectQuery* q, char**** results, int* rows, int* cols) {
  FILE* f = open_store_on_modes(q->table, "r");
  if (!f)
    return 0;

  int max_rows = 100;

  char   line_buf[1024];
  char*** result_data = calloc(max_rows, sizeof(char**));
  int    line_count  = 0;

  while (fgets(line_buf, sizeof(line_buf), f)) {
    line_buf[strcspn(line_buf, "\n")] = '\0';

    if (apply_select_where(q, line_buf)) {
      result_data[line_count++] = strdup(line_buf);
    }
  }

  fclose(f);

  *results = result_data;
  *rows = line_count;
  return line_count;
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