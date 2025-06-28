#ifndef CBANK_ENGINE_H
#define CBANK_ENGINE_H

#include <stdlib.h>

typedef struct {
  const char* key;
  const char* value;
} TemplateVar;

char* load_template_to_string(const char* path);
char* render_template(const char* template, TemplateVar* vars, size_t var_count);

#endif /* CBANK_ENGINE_H */