#include "executors.h"

#include <stdlib.h>

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
