#include "executors.h"
typedef struct {
  const char* col;
  const char* value;
} InsertPair;

#define MAX_INSERT_PAIRS 50

typedef struct InsertQuery InsertQuery;

typedef InsertQuery* (*SetFn)(InsertQuery* q, const char* column, const char* value);
typedef int (*InsertQueryExecutor)(InsertQuery* q);

struct InsertQuery {
  const char* table;
  int         set_count;

  InsertPair* insert_pair;

  SetFn               set;
  InsertQueryExecutor execute;
  void (*destroy)(InsertQuery* q);
};