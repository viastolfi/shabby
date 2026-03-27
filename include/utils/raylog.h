#ifndef RAYLOG_H
#define RAYLOG_H

#include "raylib.h"
#include <cstdarg>
#include <cstdio>
#include <iostream>

class Raylog {
public:
  static Raylog& GetInstance(int logLevel = 3) 
  {
    static Raylog r(logLevel);
    
    return r;
  }

  void SetLogLevel(int logLevel) {
    _log_level = logLevel; 
    SetTraceLogLevel(logLevel);
  }

  int GetLogLevel() const {
    return _log_level; 
  }

  void Log(int logLevel, const char* text, ...) {
    char buffer[1024];

    va_list args;
    va_start(args, text);
    vsnprintf(buffer, sizeof(buffer), text, args);
    va_end(args);

    if (IsWindowReady()) TraceLog(logLevel, "%s", buffer);
    else std::cout << buffer << std::endl;
  }

private:
  int _log_level = 3;

  explicit Raylog(int logLevel)
    :_log_level(logLevel)
  {
    SetTraceLogLevel(_log_level);
  }

  Raylog(const Raylog& r) = delete;
  void operator=(const Raylog& r);
  Raylog(Raylog&& r) = delete;
  Raylog& operator=(Raylog&& r) = delete;
};

#endif // RAYLOG_H
