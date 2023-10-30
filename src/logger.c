#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "logger.h"

FILE* output;

void init_logger(const char* file_path) {
#ifdef DEBUG
    output = stdout;
    printl("Logger has been asked for %s, set to stdout (DEBUG MODE)\n", file_path);
#else
    if (file_path) {
        output = fopen(file_path, "w");
        printl("Logger has been asked for %s, set to %s\n", file_path, file_path);
    } else {
        output = stdout;
        printl("Logger has been asked for stdout, set to stdout\n");
    }
#endif
}

void printl(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(output, format, args);
    va_end(args);
}

void warnl(const char* file_name, const char* fun_name, const char* format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(output, "[exitl] %s > %s : ", file_name, fun_name);
    vfprintf(output, format, args);
    va_end(args);
}

void exitl(const char* file_name, const char* fun_name, int exit_value, char* format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(output, "[exitl] %s > %s : ", file_name, fun_name);
    vfprintf(output, format, args);
    va_end(args);
    close_logger();
    exit(exit_value);
}

void close_logger() {
    if (output != stdout)
        fclose(output);
}
