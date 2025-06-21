#include "json_builder.h"

void json_init(JsonBuilder* jb) {
  jb->offset   = 0;
  jb->is_first = true;
  memset(jb->buf, 0, sizeof(jb->buf));
};

void json_start_object(JsonBuilder* jb) {
  jb->offset += snprintf(jb->buf + jb->offset, JSON_BUF_SIZE - jb->offset, "{");
  jb->is_first = true;
};

void json_end_object(JsonBuilder* jb) {
  jb->offset += snprintf(jb->buf + jb->offset, JSON_BUF_SIZE - jb->offset, "}");
};

void add_comma(JsonBuilder* jb) {
  jb->offset += snprintf(jb->buf + jb->offset, JSON_BUF_SIZE - jb->offset, ",");
};

void json_start_array(JsonBuilder* jb, const char* key) {
  json_add_comma_if_needed(jb);
  jb->offset += snprintf(jb->buf + jb->offset, JSON_BUF_SIZE - jb->offset, "\"%s\":[", key);
  jb->is_first = true;
}

void json_end_array(JsonBuilder* jb) {
  jb->offset += snprintf(jb->buf + jb->offset, JSON_BUF_SIZE - jb->offset, "]");
  jb->is_first = false;
}

void json_add_comma_if_needed(JsonBuilder* jb) {
  if (!jb->is_first) {
    add_comma(jb);
  } else {
    jb->is_first = false;
  }
}

void json_add_string(JsonBuilder* jb, const char* key, const char* value) {
  json_add_comma_if_needed(jb);
  jb->offset +=
      snprintf(jb->buf + jb->offset, JSON_BUF_SIZE - jb->offset, "\"%s\":\"%s\"", key, value);
}

void json_add_int(JsonBuilder* jb, const char* key, int value) {
  json_add_comma_if_needed(jb);
  jb->offset += snprintf(jb->buf + jb->offset, JSON_BUF_SIZE - jb->offset, "\"%s\":%d", key, value);
}

void json_add_float(JsonBuilder* jb, const char* key, double value) {
  json_add_comma_if_needed(jb);
  jb->offset +=
      snprintf(jb->buf + jb->offset, JSON_BUF_SIZE - jb->offset, "\"%s\":%.6f", key, value);
}

void json_add_bool(JsonBuilder* jb, const char* key, bool value) {
  json_add_comma_if_needed(jb);
  jb->offset += snprintf(jb->buf + jb->offset, JSON_BUF_SIZE - jb->offset, "\"%s\":%s", key,
                         value ? "true" : "false");
}

void json_add_null(JsonBuilder* jb, const char* key) {
  json_add_comma_if_needed(jb);
  jb->offset += snprintf(jb->buf + jb->offset, JSON_BUF_SIZE - jb->offset, "\"%s\":null", key);
}

const char* json_build(JsonBuilder* jb) {
  return jb->buf;
}