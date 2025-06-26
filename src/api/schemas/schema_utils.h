#include <stdbool.h>

#define JSON_VALUE_FIELD_MATCHER "\"%[^\"]\""

bool parse_json_field(const char* json_haystack, const char* field, char* out);