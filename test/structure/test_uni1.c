/*-----------------------------------------------------------------*/
/**
    Projet REV party - Structur list test
    \file test_uni1.c
    \author : Ugo VALLAT
    \date creation date : 20/10/2023
 
 */
/*-----------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include "../../src/structure/sdlist.h"

TYPE list1[10] = {1,2,3,4,5,6,7,8,9};

// bool test_create(int size) {
//     ptrList l = createList(size);
//     return(l != NULL);
// }

bool test_add(ptrList l, TYPE *l_to_add, int nb) {
    for(int i = 0; i < nb; i++) {
        if (listAdd(&l, l_to_add[i]) == NULL) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    ptrList l1 = createList(5);
    if (!test_add(l1, list1, 10)){
        fprintf(stderr, "erreur add");
        return(1);
    }
    deleteList(l1);
    return 0;
}