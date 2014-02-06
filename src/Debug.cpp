#include <Debug.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

bool libelfxx_debug = false;

namespace libelfxx {
void setDebug(bool enableDebug) {
  libelfxx_debug = enableDebug;
}
};

void error(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}

void print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stdout, fmt, args);
  va_end(args);
}

void fatal(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  exit(1);
}

void debug(const char *fmt, ...) {
  if (libelfxx_debug) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
  }
}
