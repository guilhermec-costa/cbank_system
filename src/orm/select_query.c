#define _POSIX_C_SOURCE 200809L

#include "select_query.h"

#include "../data/store.h"
#include "filtering.h"

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

SelectQuery* qselect_all_from(SelectQuery* q, const char* table) {
  q->from(q, table);
  if (strcmp(table, DB_USER_SECTION) == 0) {
    q->select(q, "id,name,cpf,email,password,created_at,updated_at,is_active");
  }
  if (strcmp(table, DB_ACCOUNT_SECTION) == 0) {
    q->select(q, "id,balance,user_id_fk,created_at,updated_at");
  }
  return q;
}

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

static void full_line(ResultSet* result, int line_idx, char* buf, size_t buf_size) {
  char** line   = result->data[line_idx];
  int    offset = 0;

  for (int i = 0; i < result->cols; i++) {
    offset += snprintf(buf + offset, buf_size - offset, "%s", result->column_order[i]);
    offset += snprintf(buf + offset, buf_size - offset, "%s", "=");
    offset += snprintf(buf + offset, buf_size - offset, "%s", line[i]);
    if (i < result->cols - 1)
      offset += snprintf(buf + offset, buf_size - offset, "%s", ";");
  };
}

char* get_line_from_resultset(ResultSet* rs, int line_idx) {
  if (!rs || line_idx >= rs->rows)
    return NULL;
  char** line = rs->data[line_idx];

  size_t line_len = 0;

  for (int i = 0; i < rs->cols; i++) {
    line_len += strlen(rs->column_order[i]) + 1; // '='
    line_len += strlen(line[i]);
    line_len += 1; // ';' '\0'
  }

  char* buf = malloc(line_len + 1);
  if (!buf)
    return NULL;

  int offset = 0;
  for (int i = 0; i < rs->cols; i++) {
    offset += snprintf(buf + offset, line_len - offset, "%s=%s", rs->column_order[i], line[i]);
    if (i < rs->cols - 1) {
      offset += snprintf(buf + offset, line_len - offset, ";");
    }
  }
  buf[offset + 1] = '\0';
  return buf;
}

static const char* get_row_col_value(ResultSet* rs, int row_idx, const char* column) {
  char** line = rs->data[row_idx];

  for (int i = 0; i < rs->cols; i++) {
    if (strcmp(rs->column_order[i], column) == 0) {
      return line[i];
    }
  }
}

ResultSet* make_result_set() {
  ResultSet* rs = malloc(sizeof(ResultSet));
  memset(rs, 0, sizeof(ResultSet));
  rs->cols              = 0;
  rs->rows              = 0;
  rs->free              = free_result_set;
  rs->print             = print_result_set;
  rs->full_line         = full_line;
  rs->get_row_col_value = get_row_col_value;

  return rs;
}

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

ResultSet* select_executor(SelectQuery* q) {
  ResultSet* rs = make_result_set();
  FILE*      f  = fopen(q->table, "r");

  fseek(f, 0, SEEK_SET);
  if (!f)
    return 0;

  char    line_buf[1024];
  char*** result_data = calloc(RS_MAX_ROWS, sizeof(char**));
  int     line_count  = 0;

  while (fgets(line_buf, sizeof(line_buf), f)) {
    line_buf[strcspn(line_buf, "\n")] = '\0';

    if (apply_select_where(q, line_buf)) {
      char** columns   = calloc(RS_MAX_COLS, sizeof(char*));
      int    col_count = 0;

      char* rest = line_buf;
      char* pair;

      while ((pair = strtok_r(rest, ";", &rest))) {
        char* sep_pos = strchr(pair, '=');
        if (!sep_pos)
          continue;

        *sep_pos                = '\0';
        const char* column_name = pair;
        const char* value       = sep_pos + 1;

        for (int i = 0; i < q->columns_count; i++) {
          if (strcmp(q->columns[i], column_name) == 0) {
            if (line_count == 0) {
              rs->column_order[col_count] = strdup(pair);
            }
            columns[col_count++] = strdup(value);
          }
        }
      }

      rs->cols                  = col_count;
      result_data[line_count++] = columns;
    }
  }

  fclose(f);
  rs->rows = line_count;
  rs->data = result_data;
  return rs;
};

static void destroy_select_query(SelectQuery* q) {
  if (!q)
    return;

  for (int i = 0; i < q->columns_count; i++) {
    free(q->columns[i]);
  }
  free(q);
};

SelectQuery* new_select_query() {
  SelectQuery* q = calloc(1, sizeof(SelectQuery));
  memset(q, 0, sizeof(SelectQuery));
  q->select     = qselect;
  q->select_all = qselect_all_from;
  q->where      = qwhere;
  q->from       = qfrom;
  q->execute    = select_executor;
  q->destroy    = destroy_select_query;
  return q;
}