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

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 * @brief Exit le programme avec message si arg == NULL
 * @param[in] arg argument à vérifier
 * @param[in] caller Nom de la fonction appelante
 *
 */
void testArgNull(void *arg, char *file, char *fun, char *name);

#endif