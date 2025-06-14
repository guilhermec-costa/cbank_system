#ifndef CBANK_UTILS_H
#define CBANK_UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INVALID_CHOICE -1
#define MIN_CHOICE 1
#define MAX_CHOICE 3

int validate_choice(const char* expr);
void ask_null_terminated_input_str(char* const src, size_t src_size,
                                   const char* question);

static inline void colorize(const char* color) { printf("%s", color); };
const char* hash_str(const char* const str, const char* const salt);
const char* gen_acc_id();
struct tm now();

static inline const char* fmt_date(char* buf, size_t buf_s,
                                   const struct tm date) {
  const char* fmt = "%04d-%02d-%02d %02d:%02d:%02d";
  snprintf(buf, buf_s, fmt, date.tm_year + 1900, date.tm_mon + 1,
           date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
  return buf;
}

#endif /* CBANK_UTILS_H */
