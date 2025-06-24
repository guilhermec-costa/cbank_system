#include "query.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SelectQuery* qselect(SelectQuery* q, const char* columns) {
  strncpy(q->columns, columns, sizeof(q->columns) - 1);
  q->columns[sizeof(q->columns) - 1] = '\0';
  return q;
};

SelectQuery* qfrom(SelectQuery* q, const char* table) {
  q->table = table;
  return q;
}

SelectQuery* qwhere(SelectQuery* q, const char* column, const char* operator, const char* value) {
  if (q->condition_count >= CONDITION_COUNT)
    return q;
  q->conditions[q->condition_count++] = (WhereCondition){
      .column   = column,
      .operator = operator,
      .value    = value,
  };
  return q;
}

static void free_result_set(ResultSet* rs) {
  for (int i = 0; i < rs->rows; i++) {
    for (int j = 0; j < rs->cols; j++) {
      free(rs->data[i][j]);
    }
    free(rs->data[i]);
  }
  free(rs->data);
};

static void print_result_set(ResultSet* rs) {
  for (int i = 0; i < rs->rows; i++) {
    for (int j = 0; j < rs->cols; j++) {
      printf("%s\t", rs->data[i][j]);
    }
    printf("\n");
  }
};

ResultSet* make_result_set() {
  ResultSet* rs = malloc(sizeof(ResultSet));
  memset(rs, 0, sizeof(ResultSet));
  rs->cols  = 0;
  rs->rows  = 0;
  rs->free  = free_result_set;
  rs->print = print_result_set;

  return rs;
}