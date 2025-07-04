#ifndef CBANK_UTILS_H
#define CBANK_UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INVALID_CHOICE -1
#define PRE_LOGIN_MIN_CHOICE 1
#define PRE_LOGIN_MAX_CHOICE 3

#define POST_LOGIN_MIN_CHOICE 1
#define POST_LOGIN_MAX_CHOICE 7

int  validate_choice(const char* expr, int min, int max);
void ask_null_terminated_input_str(char* const src, size_t src_size, const char* question);

const char* hash_str(const char* const str, const char* const salt);
const char* gen_acc_id();
struct tm   now();

static inline const char* fmt_date(char* buf, size_t buf_s, const struct tm date) {
  const char* fmt = "%04d-%02d-%02d %02d:%02d:%02d";
  snprintf(buf, buf_s, fmt, date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour,
           date.tm_min, date.tm_sec);
  return buf;
}

static inline void terminate_str_by_newline(char* const str) {
  str[strcspn(str, "\n")] = '\0';
};

static inline void flush_stdin() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

const char* get_fmt_now();
char*       ltrim(char* s);
char*       remove_char(char* str, char find);

const char* double_to_ascii(double value, int precision);

char* read_line(FILE* f);

#endif /* CBANK_UTILS_H */
