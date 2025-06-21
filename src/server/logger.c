#include "logger.h"

#include "../utils.h"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

void log_with_level(const char* level_str, const char* color, const char* fmt, va_list args) {
  struct tm _now = now();

  char time_buf[64];
  strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &_now);

  fprintf(stdout, "%s[%s] [%s] ", color, time_buf, level_str);
  vfprintf(stdout, fmt, args);
  fprintf(stdout, "%s\n", COLOR_RESET);
};

static void __info(const char* __fmt, va_list args) {
  log_with_level("INFO", COLOR_INFO, __fmt, args);
};

static void __debug(const char* __fmt, va_list args) {
  log_with_level("DEBUG", COLOR_DEBUG, __fmt, args);
};

static void __warn(const char* __fmt, va_list args) {
  log_with_level("WARN", COLOR_WARN, __fmt, args);
};

static void __error(const char* __fmt, va_list args) {
  log_with_level("ERROR", COLOR_ERROR, __fmt, args);
};

static void __log(const struct Logger* logger, enum LogLevel level, const char* __fmt, ...) {
  if (level < logger->level)
    return;

  va_list args;
  va_start(args, __fmt); // last named parameter

  switch (level) {
    case DEBUG:
      logger->debug(__fmt, args);
      break;
    case INFO:
      logger->info(__fmt, args);
      break;
    case WARN:
      logger->warn(__fmt, args);
      break;
    case ERROR:
      logger->error(__fmt, args);
      break;
  }
};

struct Logger* get_global_logger() {
  static struct Logger logger;
  static int           initialized = 0;
  if (!initialized) {
    logger       = get_logger();
    logger.level = DEBUG;
    initialized  = 1;
  }

  return &logger;
}

struct Logger get_logger() {
  struct Logger logger = {0};
  logger.level         = DEBUG;
  logger.log           = __log;
  logger.debug         = __debug;
  logger.info          = __info;
  logger.warn          = __warn;
  logger.error         = __error;

  return logger;
};