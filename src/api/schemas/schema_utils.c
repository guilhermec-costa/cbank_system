#include "schema_utils.h"

#include <stdio.h>

bool parse_json_field(const char* json_haystack, const char* field, char* out) {
  char matcher[1024];
  snprintf(matcher, sizeof(matcher), "\"%s\": \"%%[^\"]\"", field);
  matcher[sizeof(matcher) - 1] = '\0';
  if (sscanf(json_haystack, matcher, out) != 1) {
    return false;
  };

  return true;
}