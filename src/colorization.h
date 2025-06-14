#ifndef CBANK_COLORIZATION_H
#define CBANK_COLORIZATION_H

#define COLOR_RESET "\033[0m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_CYAN "\033[1;36m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_RED "\033[1;31m"
#define COLOR_AQUAMARINE "\033[1;36m"

#include <stdio.h>

static inline void colorize(const char* color) {
  printf("%s", color);
};

#endif /* CBANK_COLORIZATION_H */
