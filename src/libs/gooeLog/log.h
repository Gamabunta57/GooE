#ifndef GOOE_LOG_H
#   define GOOE_LOG_H

#   ifdef WIN32
#   if defined(gooeLog_EXPORTS) // add by CMake 
#       define  GOOE_LOG_API __declspec(dllexport)
#   else
#       define  GOOE_LOG_API 
#   endif
#else
#   define GOOE_LOG_API
#endif

#   define GOOE_LOG_LEVEL_DEBUG 0
#   define GOOE_LOG_LEVEL_INFO 1
#   define GOOE_LOG_LEVEL_WARNING 2
#   define GOOE_LOG_LEVEL_ERROR 3

    GOOE_LOG_API void gooe_log(unsigned char logLevel, char* fileName, int line, char* format, ...);

#   ifdef GOOE_LOG_NONE
#       define LOG_DEBUG(fmt, ...)
#       define LOG_INFO(fmt, ...)
#       define LOG_WARN(fmt, ...)
#       define LOG_ERR(fmt, ...)
#   else
#       define LOG_DEBUG(fmt, ...) gooe_log(GOOE_LOG_LEVEL_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#       define LOG_INFO(fmt, ...) gooe_log(GOOE_LOG_LEVEL_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#       define LOG_WARN(fmt, ...) gooe_log(GOOE_LOG_LEVEL_WARNING, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#       define LOG_ERR(fmt, ...) gooe_log(GOOE_LOG_LEVEL_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#   endif

#   ifdef GOOE_LOG_IMPLEMENTATION
#   include <stdarg.h>
#   include <stdio.h>

    static char* logLevelPrefix[] = {
        "\033[36m", // cyan
        "\033[0m",  // white
        "\033[33m", // yellow
        "\033[31m", // red
    };

    GOOE_LOG_API void gooe_log(unsigned char logLevel, char* fileName, int line, char* format, ...) {
        va_list args;
        va_start(args, format);

        fprintf(stdout, "%s[%s:%d] ", logLevelPrefix[logLevel], fileName, line);
        vfprintf(stdout, format, args);
        fprintf(stdout, "%s\n", logLevelPrefix[GOOE_LOG_LEVEL_INFO]);
        fflush(stdout);

        va_end(args);
    }

#   endif
#endif