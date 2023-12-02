#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include "../test_utils.h"
#include "../../src/structure/list.h"

 #define UINT_MAX_VALUE 4294967295U

StringBuilder* string_builder;
int return_value;

void segfault_handler() {

}

bool testCreateList() {
    emptyStringBuilder(string_builder);

    // test liste taille 0
    addLineStringBuilder(string_builder, "\ntest sur liste taille 0...");
    List* l1 = createList(0);
    if (!l1) return false; // assert notNull
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    // test liste taille maximale
    addLineStringBuilder(string_builder, "\ntest sur liste taille négative...");
    List* l2 = createList(UINT_MAX_VALUE);
    if (!l2) return false; // assert notNull
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    // test liste taille quelconque
    addLineStringBuilder(string_builder, "\ntest sur liste taille 10...");
    List* l3 = createList(10);
    if (!l3) return false; // assert notNull
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    return true;
}

bool testListAdd() {
    emptyStringBuilder(string_builder);

    // test ajout à la fin
    addLineStringBuilder(string_builder, "\ntest sur liste taille quelconque...");
    List* l1 = createList(3);
    listAdd(l1, 5);
    if (listGet(l1, 0) != 5) return false;
    addLineStringBuilder(string_builder, "\n\t- 1 ajout fonctionne");
    listAdd(l1, 2);
    if (listGet(l1, 0) != 5) return false;
    addLineStringBuilder(string_builder, "\n\t- 1er élément reste inchangé");
    if (listGet(l1, 1) != 2) return false;
    addLineStringBuilder(string_builder, "\n\t- élément après le premier");
    listAdd(l1, 4);
    if (listGet(l1, 2) != 4) return false;
    addLineStringBuilder(string_builder, "\n\t- élément ajouté à la fin");

    // test taille inconséquentielle
    addLineStringBuilder(string_builder, "\ntest sur liste taille 0...");
    List* l2 = createList(2);
    listAdd(l2, 3);
    listAdd(l2, 4);
    listAdd(l2, 5);
    if (!l2) return false; // assert notNull
    addLineStringBuilder(string_builder, "\n\t- list toujours valide après réalocation");
    if (listGet(l2, 0) != 3 || listGet(l2, 1) != 4 || listGet(l2, 2) != 5) return false; // assert equal
    addLineStringBuilder(string_builder, "\n\t- ordre inchangé après réalocation\n");
/*
    // test dépassement d'entier
    addLineStringBuilder(string_builder, "\ntest sur liste taille max...");
    List* l3 = createList(UINT_MAX_VALUE);
    List* add_return;
    for (unsigned int i = 0; i<UINT_MAX_VALUE; i++) {
        add_return = listAdd(l3, i);
    }
    if (add_return) return false;
    addLineStringBuilder(string_builder, "\n\t- la liste est bien devenue invalide\n");
*/
    return true;
}

int main() {

    string_builder = createStringBuilder();

    return_value = 0;

    if (!testCreateList()) {
        return_value += 1; /* 2^0 */
        printFailure("testCreateList");
        printStringBuilder(string_builder);
    } else printSuccess("testCreateList");

    if (!testListAdd()) {
        return_value += 2; /* 2^1 */
        printFailure("testAddList");
        printStringBuilder(string_builder);
    } else printSuccess("testAddList");


    return return_value;

}