#ifndef CBANK_UTILS_H
#define CBANK_UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INVALID_CHOICE -1
#define MIN_CHOICE 1
#define MAX_CHOICE 3

int validate_choice(const char* expr);
void ask_null_terminated_input_str(char* const src, size_t src_size,
                                   const char* question);

static inline void colorize(const char* color) { printf("%s", color); };
const char* hash_str(const char* const str, const char* const salt);

#endif /* CBANK_UTILS_H */
