#ifndef CBANK_JSON_BUILDER_H
#define CBANK_JSON_BUILDER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JSON_BUF_SIZE 4096

typedef struct {
  char buf[JSON_BUF_SIZE];
  int  offset;
  bool is_first;
} JsonBuilder;

void json_init(JsonBuilder* jb);

void json_start_object(JsonBuilder* jb);
void json_end_object(JsonBuilder* jb);
void json_start_array(JsonBuilder* jb, const char* key);
void json_end_array(JsonBuilder* jb);

void json_add_string(JsonBuilder* jb, const char* key, const char* value);
void json_add_int(JsonBuilder* jb, const char* key, int value);
void json_add_float(JsonBuilder* jb, const char* key, double value);
void json_add_bool(JsonBuilder* jb, const char* key, bool value);
void json_add_null(JsonBuilder* jb, const char* key);

void        json_add_comma_if_needed(JsonBuilder* jb);
const char* json_build(JsonBuilder* jb);

#endif /* CBANK_JSON_BUILDER_H */