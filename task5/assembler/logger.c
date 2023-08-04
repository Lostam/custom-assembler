#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

char* getTimestamp() {
    time_t currentTime = time(NULL);
    char* timestamp = ctime(&currentTime);
    timestamp[strlen(timestamp) - 1] = '\0';  // Remove trailing newline
    return timestamp;
}

void debug(const char* format, ...) {
    printf("[%s] [DEBUG] ", getTimestamp());

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}

void info(const char* format, ...) {
    printf("[%s] [INFO] ", getTimestamp());

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}

void warn(const char* format, ...) {
    printf("[%s] [WARN] ", getTimestamp());

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}

void error(const char* format, ...) {
    fprintf(stderr, "[%s] [ERROR] ", getTimestamp());

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, "\n");
}

// fixme :: maybe improve this code later;