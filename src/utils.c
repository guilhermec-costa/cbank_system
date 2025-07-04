#include "utils.h"

#include <crypt.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int validate_choice(const char* choice, int min, int max) {
  int choice_to_i = atoi(choice);
  if (choice_to_i == 0 || choice_to_i < min || choice_to_i > max)
    return INVALID_CHOICE;
  return choice_to_i;
}

void ask_null_terminated_input_str(char* const src, size_t src_size, const char* question) {
  printf("%s", question);
  fgets(src, src_size, stdin);
  src[strcspn(src, "\n")] = '\0';
}

void reset_entity_attr_mem(void* ent, size_t ent_s) {
  memset(ent, 0, ent_s);
}

const char* hash_str(const char* const str, const char* const salt) {
  const char* const hash = crypt(str, salt);
  return hash;
}

const char* gen_acc_id() {
  static char buf[20];
  static int  counter = 0;
  snprintf(buf, sizeof(buf), "ACC%05d", ++counter);

  return buf;
}

struct tm now() {
  time_t    t  = time(NULL);
  struct tm tm = *localtime(&t);
  return tm;
}

const char* get_fmt_now() {
  const struct tm at_now = now();
  char            date_buf[100];
  return fmt_date(date_buf, sizeof(date_buf), at_now);
};

char* ltrim(char* c) {
  while (*c == ' ')
    c++;
  return c;
}

char* read_line(FILE* f) {
  size_t size = 128;
  char*  buf  = malloc(size);
  if (!buf)
    return NULL;

  size_t len = 0;
  int    c;

  while ((c = fgetc(f)) != EOF && c != '\n') {
    buf[len++] = (char)c;
    if (len >= size) {
      size *= 2;
      char* _tmp = realloc(buf, size);
      if (!_tmp) {
        free(buf);
        return NULL;
      }
      buf = _tmp;
    }
  }

  if (len == 0 && c == EOF) {
    free(buf);
    return NULL;
  }

  buf[len] = '\0';
  return buf;
};

char* remove_char(char* str, char find) {
  char* read  = str;
  char* write = str;

  while (*read) {
    if (*read != find) {
      *write++ = *read;
    }
    read++;
  }
  *write = '\0';
  return str;
}

const char* double_to_ascii(double value, int precision) {
  static char buf[64];

  if (precision < 0)
    precision = 6;
  if (precision > 20)
    precision = 20;

  snprintf(buf, sizeof(buf), "%.*f", precision, value);
  return buf;
}

#define RESET_ENTITY(entity) reset_entity_attr_mem(entity, sizeof(entity))
