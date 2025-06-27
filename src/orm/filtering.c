#include "filtering.h"

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
