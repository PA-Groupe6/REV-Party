#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <execinfo.h>
#include "logger.h"

FILE* output = NULL;

/**
 * @date 04/11/2023
 * @author LAFORGE Mateo
 */
void init_logger(const char* file_path) {
    if (file_path) {
        output = fopen(file_path, "w+");
    } else {
        output = stdout;
    }
}

/**
 * @date 04/11/2023
 * @author LAFORGE Mateo
 */
void printl(const char* format, ...) {
#ifdef DEBUG
    if (output == NULL) {
        fprintf(stderr, "Warning: logger call to printl while being not initialized!");
        return;
    }
#endif
    va_list args;
    va_start(args, format);
    vfprintf(output, format, args);
    fflush(output); // intégrité des logs
    va_end(args);
}

/**
 * @date 04/11/2023
 * @author LAFORGE Mateo
 */
void warnl(const char* file_name, const char* fun_name, const char* format, ...) {
#ifdef DEBUG
    if (output == NULL) {
        fprintf(stderr, "Warning: logger call to warnl while being not initialized!\nlast call was from %s in %s\n", fun_name, file_name);
        return;
    }
#endif
    va_list args;
    va_start(args, format);
    fprintf(output, "[exitl] %s > %s : ", file_name, fun_name);
    vfprintf(output, format, args);
    fflush(output); // intégrité des logs
    va_end(args);
}


/**
 * @date 04/11/2023
 * @author LAFORGE Mateo
 * @brief Affiche la pile d'appel de fonction
 * @pre Drapeau de compilation -rdynamic
 */
void printStackTrace() {
    void* buffer[64];
    int nbv = backtrace(buffer, sizeof(buffer));
    char** strings = backtrace_symbols(buffer, nbv);
    for (int i = 1; i<nbv; i++) { // démarre à 1 pour ignorer l'appel de cette fonction
        fprintf(stderr, "%s\n", strings[i]);
    }
    free(strings);
}

/**
 * @date 04/11/2023
 * @author LAFORGE Mateo
 */
void exitl(const char* file_name, const char* fun_name, int exit_value, char* format, ...) {
#ifdef DEBUG
    if (output == NULL) {
        fprintf(stderr, "Warning: logger call to exitl while being not initialized!\n");
        printStackTrace();
        exit(EXIT_FAILURE);
    }
#endif
    va_list args;
    va_start(args, format);
    fprintf(output, "[exitl] %s > %s : ", file_name, fun_name);
    vfprintf(output, format, args);
    va_end(args);
    close_logger();
    printStackTrace();
    exit(exit_value);
}

/**
 * @date 04/11/2023
 * @author LAFORGE Mateo
 */
void close_logger() {
    if (output != NULL && output != stdout)
        fclose(output);
}
