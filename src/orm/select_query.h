#ifndef CBANK_QUERY_H
#define CBANK_QUERY_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct SelectQuery SelectQuery;
typedef struct ResultSet   ResultSet;

typedef enum { QUERY_SELECT, QUERY_INSERT, QUERY_UPDATE, QUERY_DELETE } QueryType;

#define CONDITION_COUNT 10

typedef struct {
  const char* column;
  const char* operator;
  const char* value;
} WhereCondition;

typedef SelectQuery* (*WhereFn)(SelectQuery* q, const char* column, const char* operator,
                                const char* value);
typedef SelectQuery* (*SelectFn)(SelectQuery* q, char* columns);
typedef SelectQuery* (*SelectAllFn)(SelectQuery* q, const char* table);
typedef SelectQuery* (*FromFn)(SelectQuery* q, const char* table);

typedef struct {
  int     columns;
  int     rows;
  char*** data; // data[row][column]: value
} SelectResult;

typedef ResultSet* (*SelectQueryExecutorFn)(SelectQuery*);

struct SelectQuery {
  const char* table;
  char**      columns;
  int         condition_count;
  int         columns_count;

  WhereCondition        conditions[CONDITION_COUNT];
  WhereFn               where;
  SelectFn              select;
  SelectAllFn           select_all;
  FromFn                from;
  SelectQueryExecutorFn execute;
  void (*destroy)(SelectQuery* q);
};

SelectQuery* qselect(SelectQuery* q, char* columns);
SelectQuery* qfrom(SelectQuery* q, const char* table);
SelectQuery* qwhere(SelectQuery* q, const char* column, const char* operator, const char* value);

#define RS_MAX_COLS 20
#define RS_MAX_ROWS 100

struct ResultSet {
  char*** data;
  int     rows;
  int     cols;
  char*   column_order[RS_MAX_COLS];

  void (*full_line)(ResultSet* rs, int line_idx, char* buf, size_t buf_size);
  void (*free)(ResultSet*);
  void (*print)(ResultSet*);
  const char* (*get_row_col_value)(ResultSet* rs, int row_idx, const char* column);
};

ResultSet*   make_result_set();
SelectQuery* new_select_query();
bool         apply_select_where(SelectQuery* q, const char* line_buf);

#endif /* CBANK_QUERY_H */