#include <stdarg.h>

enum LogLevel { DEBUG = 0, INFO = 1, WARN = 2, ERROR = 3 };

#define COLOR_RESET "\033[0m"
#define COLOR_DEBUG "\033[36m" // Cyan
#define COLOR_INFO "\033[32m"  // Green
#define COLOR_WARN "\033[33m"  // Yellow
#define COLOR_ERROR "\033[31m" // Red

struct Logger {
  enum LogLevel level;
  void (*log)(const struct Logger* logger, const enum LogLevel level, const char* __fmt, ...);
  void (*info)(const char* __fmt, va_list args);
  void (*debug)(const char* __fmt, va_list args);
  void (*warn)(const char* __fmt, va_list args);
  void (*error)(const char* __fmt, va_list args);
};

struct Logger get_logger();