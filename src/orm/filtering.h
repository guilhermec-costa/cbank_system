#ifndef CBANK_FILTERING_H
#define CBANK_FILTERING_H
#include <stdbool.h>

typedef struct {
  const char* column;
  const char* operator;
  const char* value;
} WhereCondition;

bool compare(const char* left, const char* op, const char* right);
#endif /* CBANK_FILTERING_H */