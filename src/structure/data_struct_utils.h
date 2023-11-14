/**
 * @file data_struct_utils.h
 * @author VALLAT Ugo
 *
 * @brief Cette librairie regroupe des outils pour l'implémentation des
 * structures de données
 *
 */

#ifndef __DS_UTILS_H__
#define __DS_UTILS_H__

#include "genericlist.h"

/* Taille maximale des labels */
#define MAX_LENGHT_LABEL 256

/* taille affichage Cases */
#define DISPLAY_LENGHT_BOX 16


/**
 * @date  5/11/2023
 * @brief Exit le programme avec message si arg == NULL
 * @param[in] arg argument à vérifier
 * @param[in] caller Nom de la fonction appelante
 * @pre file != NULL
 * @pre fun != NULL
 * @pre name != NULL
 *
 */
void testArgNull(void *arg, char *file, char *fun, char *name);

/**
 * @date  13/11/2023
 * @brief Copier une liste générique de labels
 * @param[in] labels liste de labels
 * @pre labels != NULL
 * @return copie de la liste
 */
GenList* copyLabels(GenList* labels);

/**
 * @date  13/11/2023
 * @brief Cherche un label dans une liste de labels
 * @param[in] labels liste de labels
 * @param[in] label llabel à chercher
 * @pre labels != NULL && label != NULL
 * @return position du label dans la liste, -1 si introuvable
 */
int searchLabel(GenList* labels, char* label);


#endif