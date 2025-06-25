#include "query.h"
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SelectQuery* qselect(SelectQuery* q, char* columns) {
  q->columns = calloc(RS_MAX_COLS, sizeof(char*));

  char columns_buf[1024];
  strncpy(columns_buf, columns, sizeof(columns_buf) - 1);
  columns_buf[sizeof(columns_buf) - 1] = '\0';

  char* rest_col = columns_buf;
  char* col_token;

  while ((col_token = strtok_r(rest_col, ",", &rest_col))) {
    q->columns[q->columns_count++] = strdup(col_token);
  }

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
      printf("| %s=%s ", rs->column_order[j], rs->data[i][j]);
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