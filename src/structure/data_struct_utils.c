#include "../logger.h"
#include <stdio.h>
#include <stdlib.h>
#include "data_struct_utils.h"
#include "genericlist.h"
#include <string.h>

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void testArgNull(void *arg, char *file, char *fun, char *name) {
    if (arg == NULL)
        exitl(file, "hisArgNull", EXIT_FAILURE, "[%s in %s] %s : argument NULL", fun, file, name);
}


/**
 * @date  13/11/2023
 * @author Ugo VALLAT
 */
GenList* copyLabels(GenList* labels) {
    testArgNull(labels, "data_struct_utils.c", "copyLabels","labels");
    char *label;
    GenList* list = createGenList(genListSize(labels));

    for(int i = 0; i < genListSize(labels); i++) {
        label = malloc(sizeof(char)*MAX_LENGHT_LABEL);
        if(strncpy(label, genListGet(labels, i), MAX_LENGHT_LABEL) == NULL)
            exitl("data_struct_utils.c", "copyLabels", EXIT_FAILURE, "Echec copie label %s", genListGet(labels, i));
        genListAdd(list, label);
    }

    return list;
}

/**
 * @date  13/11/2023
 * @author Ugo VALLAT
 */
int searchLabel(GenList* labels, char* label) {
    testArgNull(labels, "data_struct_utils.c", "searchLabel","labels");
    testArgNull(label, "data_struct_utils.c", "searchLabel","label");
    if(strlen(label) > MAX_LENGHT_LABEL)
        exitl("bale.c", "baleLabelToColumn", EXIT_FAILURE, "taille label %s > MAX_LENGHT_LABEL", label);
    
    int i;
    for(i = 0; i < genListSize(labels); i++) {
        if(strncmp(label, (char*)genListGet(labels, i), MAX_LENGHT_LABEL) == 0)
            return i;
    }
    if(i == genListSize(labels))
        i = -1;
    return i;
 }
