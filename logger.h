#ifndef LOGGER_H
#define LOGGER_H
#include <stdio.h>
#include <stdarg.h>

enum LOG_LEVEL{L_DEBUG, L_INFO, L_WARNING, L_CRITICAL, L_ERROR};
static const char* LOG_LEVEL_MAPPING[] = {
  "DEBUG", "INFO", "WARNING", "CRITICAL", "ERROR",
};


static int current_level = L_DEBUG;

void g_log(LOG_LEVEL level, const char* msg, va_list args) {
  if (level >= current_level) {
    printf("%s:\t ", LOG_LEVEL_MAPPING[level]);
    vprintf(msg, args);
    printf("\n");
  }
}

void g_logDebug(const char* msg, ...) {
  va_list argptr;
  va_start(argptr, msg);
  g_log(L_DEBUG, msg, argptr);
  va_end(argptr);
}
void g_logInfo(const char* msg, ...) {
  va_list argptr;
  va_start(argptr, msg);
  g_log(L_INFO, msg, argptr);
  va_end(argptr);
}
void g_logWarning(const char* msg, ...) {
  va_list argptr;
  va_start(argptr, msg);
  g_log(L_WARNING, msg, argptr);
  va_end(argptr);
}
void g_logCritical(const char* msg, ...) {
  va_list argptr;
  va_start(argptr, msg);
  g_log(L_CRITICAL, msg, argptr);
  va_end(argptr);
}
void g_logError(const char* msg, ...) {
  va_list argptr;
  va_start(argptr, msg);
  g_log(L_ERROR, msg, argptr);
  va_end(argptr);
}

void log_set_level(LOG_LEVEL level) {
  current_level = level;
}

class Logger
{
private:
    char const* m_name;
    void log(LOG_LEVEL level, const char* msg, ...)
    {
      va_list argptr;
      printf("[%s]\t", m_name);
      va_start(argptr, msg);
      g_log(level, msg, argptr);
      va_end(argptr);
    }
public:
    Logger(char const* name) : m_name(name) {}
    void logDebug(const char* msg, ...) {
      va_list argptr;
      va_start(argptr, msg);
      this->log(L_DEBUG, msg, argptr);
      va_end(argptr);
    }
    void logInfo(const char* msg, ...) {
      va_list argptr;
      va_start(argptr, msg);
      this->log(L_INFO, msg, argptr);
      va_end(argptr);
    }
    void logWarning(const char* msg, ...) {
      va_list argptr;
      va_start(argptr, msg);
      this->log(L_WARNING, msg, argptr);
      va_end(argptr);
    }
    void logCritical(const char* msg, ...) {
      va_list argptr;
      va_start(argptr, msg);
      this->log(L_CRITICAL, msg, argptr);
      va_end(argptr);
    }
    void logError(const char* msg, ...) {
      va_list argptr;
      va_start(argptr, msg);
      this->log(L_ERROR, msg, argptr);
      va_end(argptr);
    }
};


#endif
