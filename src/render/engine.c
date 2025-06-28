#include "engine.h"

#include "stdio.h"

#include <stdlib.h>
#include <string.h>

char* load_template_to_string(const char* path) {
  FILE* f = fopen(path, "rb");
  if (!f)
    return NULL;

  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);
  rewind(f);

  char* buf = malloc(size + 1);
  if (!buf) {
    fclose(f);
    return NULL;
  }

  fread(buf, 1, size, f);
  buf[size] = '\0';

  return buf;
};

char* render_template(const char* template, TemplateVar* vars, size_t var_count) {
  size_t out_size = strlen(template) + 1024;
  char*  out_buf  = malloc(out_size);
  if (!out_buf)
    return NULL;

  size_t out_idx = 0;

  const char* p = template;

  while (*p) {
    if (*p == '{' && *(p + 1) == '{') {
      const char* var_start = p + 2;
      const char* var_end   = strstr(var_start, "}}");
      if (var_end) {
        size_t key_len = var_end - var_start;

        char key[128] = {0};
        strncpy(key, var_start, key_len);

        const char* value = "";
        for (size_t i = 0; i < var_count; i++) {
          if (strcmp(key, vars[i].key) == 0) {
            value = vars[i].value;
            break;
          }
        }

        size_t value_len = strlen(value);
        if (value_len + out_idx >= out_size - 1)
          break;
        memcpy(out_buf + out_idx, value, value_len);
        out_idx += value_len;

        p = var_end + 2;
        continue;
      }
    }

    if (out_idx < out_size - 1)
      out_buf[out_idx++] = *p++;
  }
  out_buf[out_size - 1] = '\0';
  return out_buf;
}