#ifndef CBANK_QUERY_H
#define CBANK_QUERY_H

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
typedef SelectQuery* (*SelectFn)(SelectQuery* q, const char* columns);
typedef SelectQuery* (*FromFn)(SelectQuery* q, const char* table);

typedef struct {
  int     columns;
  int     rows;
  char*** data; // data[row][column]: value
} SelectResult;

typedef ResultSet* (*SelectQueryExecutorFn)(SelectQuery*);

struct SelectQuery {
  const char* table;
  char        columns[256];
  int         condition_count;

  WhereCondition        conditions[CONDITION_COUNT];
  WhereFn               where;
  SelectFn              select;
  FromFn                from;
  SelectQueryExecutorFn execute;
};

SelectQuery* qselect(SelectQuery* q, const char* columns);
SelectQuery* qfrom(SelectQuery* q, const char* table);
SelectQuery* qwhere(SelectQuery* q, const char* column, const char* operator, const char* value);

#define RS_MAX_COLS 20
#define RS_MAX_ROWS 100
struct ResultSet {
  char*** data;
  int     rows;
  int     cols;

  void (*free)(ResultSet*);
  void (*print)(ResultSet*);
};

ResultSet* make_result_set();

#endif /* CBANK_QUERY_H */