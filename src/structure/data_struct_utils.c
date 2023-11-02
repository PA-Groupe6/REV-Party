#include "../logger.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void testArgNull(void *arg, char *file, char *fun, char *name) {
    if (arg == NULL)
        exitl(file, "hisArgNull", EXIT_FAILURE, "[%s in %s] %s : argument NULL", fun, file, name);
}