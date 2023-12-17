/*-----------------------------------------------------------------*/
/**
 * @file bale.c
 * @author VALLAT Ugo, LAFORGE Mateo
 * @brief Structure Ballot header
 *
 * @brief Un ballot est une matrice où chaque ligne correspond à un électeur et chaque colonne à
 * un candidat.
 *
 * les colonnes son étiquetée avec le nom du candidat.
 *
 * Le ballot est de taille fixe et ses éléments ne sont pas modifiables après avoir été chargés
 * dans le ballot.
 *
 * @note Taille maximum des étiquetées = @ref MAX_LENGHT_LABEL
 *
 * @remark Les dimensions d'un ballot sont fixes
 *
 * @remark En cas d'erreur, toutes les fonctions du ballot exit le progamme avec un
 * message d'erreur
 */
/*-----------------------------------------------------------------*/


#include "data_struct_utils.h"
#include "genericlist.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "bale.h"
#include "matrix.h"
#include <malloc.h>
#include <string.h>



/**
 * @date  13/11/2023
 * @brief Définition de la structure Bale
 */
struct s_bale {
    int default_value;
    GenList* labels;    /* Liste des labels */
    Matrix* matrix;     /* Matrice contenant les données */
};

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Crée un ballot
 */
Bale *createBale(unsigned int nbl, unsigned int nbc, GenList *labels) {
#ifdef DEBUG
    testArgNull(labels, "bale.c", "createBale", "labels");
    if(genListSize(labels) != nbc)
        exitl("bale.c", "createBale", EXIT_FAILURE, "Nombre labels (%d) != nombre colonnes (%d)", genListSize(labels), nbc);
#endif

    Bale* bale = malloc(sizeof(Bale));
    bale->labels = copyLabels(labels);
    bale->matrix = createMatrix(nbl, nbc, DEFAULT_VALUE);
    bale->default_value = DEFAULT_VALUE;
    return bale;
}

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Supprime le ballot et ses données en libérant la mémoire
 */
void deleteBale(ptrBale *b) {
#ifdef DEBUG
    testArgNull(b, "bale.c", "deleteBale", "b");
    testArgNull(*b, "bale.c", "deleteBale", "*b");
#endif

    deleteMatrix(&(*b)->matrix);
    while(!genListEmpty((*b)->labels))
        free(genListPop((*b)->labels));
    deleteGenList(&(*b)->labels);
    free(*b);
    *b = NULL;
}

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Ajoute un élément dans le ballot à la position (l,c), l'élément
 */
Bale *baleSetValue(Bale *b, unsigned int l, unsigned int c, int v) {
#ifdef DEBUG
    testArgNull(b, "bale.c", "baleSetValue", "b");
    if(matrixGet(b->matrix, l, c) != b->default_value)
        exitl("bale.c", "baleSetValue", EXIT_FAILURE, "Impossible de modifier une valeur déjà set");
#endif
    matrixSet(b->matrix, l, c, v);
    return b;
}

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Renvoie la valeur à la position (l,c) dans le ballot
 */
int baleGetValue(Bale *b, unsigned int l, unsigned int c) {
#ifdef DEBUG
    testArgNull(b, "bale.c", "baleGetValue", "b");
#endif

    return matrixGet(b->matrix, l, c);
}

/**
 * @date 14/11/2023
 * @author Ugo VALLAT
 */
unsigned int baleNbVoter(Bale *b) {
#ifdef DEBUG
    testArgNull(b, "bale.c", "baleNbVoter", "b");
#endif

    return matrixNbLines(b->matrix);
}

/**
 * @date 14/11/2023
 * @author Ugo VALLAT
 */
unsigned int baleNbCandidat(Bale *b) {
#ifdef DEBUG
    testArgNull(b, "bale.c", "baleNbCandidat", "b");
#endif

    return matrixNbColonnes(b->matrix);
}


/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Renvoie le numéro de la colonne associée à l'étiquette
 */
int baleLabelToColumn(Bale *b, char *label) {
#ifdef DEBUG
    testArgNull(b, "bale.c", "baleLabelToColumn", "b");
    testArgNull(label, "bale.c", "baleLabelToColumn", "label");
#endif

    return searchLabel(b->labels, label);
}

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Renvoie le label associé à la colonne
 */
char *baleColumnToLabel(Bale *b, unsigned int c) {
#ifdef DEBUG
    testArgNull(b, "bale.c", "baleColumnToLabel", "b");
    if(c >= genListSize(b->labels))
        exitl("bale.c", "baleColumnToLabel", EXIT_FAILURE, "Argument c (%d) invalide", c);
#endif

    char* label = malloc(sizeof(char)*MAX_LENGHT_LABEL);
    strncpy(label, genListGet(b->labels, c), MAX_LENGHT_LABEL);
    return label;
}

/*------------------------------------------------------------------*/
/*                          ITERATEUR                               */
/*------------------------------------------------------------------*/

typedef struct buff_ite {
    fun_ite_bale fun;
    void* buff;
}BuffIte;

int fun_bale_to_matrix(int v, unsigned int l, unsigned int c, void *buff) {
    BuffIte *buff_ite = (BuffIte*)buff;
    buff_ite->fun(v,l,c,buff_ite->buff);
    return v;
}

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Crée un itérateur sur le ballot passé en entrée et le positionne
 * avant le premier élément
 */
BaleIte *createBaleIte(Bale *b, int l, int c, fun_ite_bale fun, void *buff) {
#ifdef DEBUG
    testArgNull(b, "bale.c", "createBaleIte", "b");
#endif

    BuffIte *buffite = malloc(sizeof(BuffIte));
    buffite->buff = buff;
    buffite->fun = fun;
    return createMatrixIte(b->matrix, l, c, fun_bale_to_matrix, buffite);
}

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Renvoie vrai si il reste des éléments à parcourir
 */
bool baleIteHasNext(BaleIte *ite) {
    return matrixIteHasNext(ite);
}

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Décale l'itérateur sur le prochain élément, renvoie sa valeur et réalise
 *  le traitement de fun sans modification du ballot
 */
int baleIteNext(BaleIte *ite) {
    return matrixIteNext(ite);
}

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Renvoie la valeur courrante
 */
int baleIteGetValue(BaleIte *ite) {
    return matrixIteGetValue(ite);
}

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Supprime l'itérateur et libère la mémoire
 */
void* deleteBaleIte(ptrBaleIte *ite) {
    BuffIte *buff_ite = deleteMatrixIte(ite);
    void* buff = buff_ite->buff;
    free(buff_ite);
    return buff;
}

/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Applique la fonction fun à tous les éléments spécifiés du ballot sans
 * les modifier
 */
void baleMap(Bale *b, int l, int c, fun_ite_bale fun, void *buff) {
    testArgNull(b, "bale.c", "baleMap", "b");
    BuffIte *buff_ite = malloc(sizeof(BuffIte));
    buff_ite->buff = buff;
    buff_ite->fun = fun;
    matrixMap(b->matrix, l, c, fun_bale_to_matrix, buff_ite);
    free(buff_ite);
}

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Renvoie la somme des élément positifs du ballot, d'une ligne ou d'une colonne
 */
int baleSom(Bale *b, int l, int c) {
#ifdef DEBUG
    testArgNull(b, "bale.c", "baleSom", "b");
#endif

    return matrixSum(b->matrix, l, c);
}

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Renvoie la/les plus grande valeur du ballot, d'une ligne ou d'une colonne
 **/
GenList *baleMax(Bale *b, int l, int c) {
#ifdef DEBUG
    testArgNull(b, "bale.c", "baleMax", "b");
#endif

    return matrixMax(b->matrix, l, c);
}

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Renvoie la/les plus petite valeur du ballot, d'une ligne ou d'une colonne
 **/
GenList *baleMin(Bale *b, int l, int c) {
#ifdef DEBUG
    testArgNull(b, "bale.c", "baleMin", "b");
#endif

    return matrixMin(b->matrix, l, c);
}


/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Filtre le ballot et renvoie une copie du ballot avec uniquement
 * les éléments tel que fun(elem) = true
 */
Bale *baleFilter(Bale *b, fun_filter_bale fun, void *buff) {
#ifdef DEBUG
    testArgNull(b, "bale.c", "baleFilter", "b");
#endif

    Bale* new = malloc(sizeof(Bale));
    new->default_value = b->default_value;
    new->labels = copyLabels(b->labels);
    new->matrix = matrixFilter(b->matrix, fun, buff);
    return new;
}

/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Copie un ballot
 */

Bale *baleCopy(Bale *b) {
#ifdef DEBUG
    testArgNull(b, "bale.c", "baleCopy", "b");
#endif

    Bale* cp = malloc(sizeof(Bale));
    cp->default_value = b->default_value;
    cp->labels = copyLabels(b->labels);
    cp->matrix = matrixCopy(b->matrix);
    return cp;
}
