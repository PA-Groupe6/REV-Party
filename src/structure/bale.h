/*-----------------------------------------------------------------*/
/**
 * @file bale.h
 * @author VALLAT Ugo, LAFORGE Mateo
 * @brief Structure Ballot header
 *
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
 * @note Taille maximum des étiquetées = @ref MAX_LENGHT_LABEL
 *
 * @remark Les dimensions d'un ballot sont fixes
 *
 * @remark En cas d'erreur, toutes les fonctions du ballot exit le progamme avec un
 * message d'erreur
 */
/*-----------------------------------------------------------------*/
#ifndef __BALE_H__
#define __BALE_H__
#include "genericlist.h"
#include "matrix.h"
#include <errno.h>
#include "genericlist.h"
#include "matrix.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include "data_struct_utils.h"
#include <stdio.h>
#include "data_struct_utils.h"

/**
 * @def DEFAULT_VALUE
 * @brief valeur par défaut d'une case
 * @note ne doit pas faire partie de l'ensemble des valeurs possibles des éléments stockés
 */
#define DEFAULT_VALUE -2

/*------------------------------------------------------------------*/
/*                        STRUCTURE BALLOT                          */
/*------------------------------------------------------------------*/

/**
 * @date 5/11/2023
 * @brief Définition opaque de la structure Bale
 */
typedef struct s_bale Bale;
typedef Bale *ptrBale;

/**
 * @date  5/11/2023
 * @brief Crée un ballot
 *
 *
 * @param[in] nbl Nombre de lignes de données (hors étiquettes)
 * @param[in] nbc Nombre de colonnes de données (hors étiquettes)
 * @param[in] labels Liste générique des labels (liste de char*)
 * @pre genericListSize(labels) == nbc && Forall x in labels, typeof(x) == char*
 * @pre sizeof(label) < @ref MAX_LENGHT_LABEL
 *
 * @return pointeur vers le ballot
 * @post Les labels sont ajoutés au colonne dans le même ordre que la liste
 * @remark Copie les labels dans le balot avec strncpy()
 */
Bale *createBale(unsigned int nbl, unsigned int nbc, GenList *labels);

/**
 * @date 5/11/2023
 * @brief Supprime le ballot et ses données en libérant la mémoire
 *
 * @param[in] b pointeur vers le ballot à supprimer
 * @pre b != NULL
 */
void deleteBale(ptrBale *b);

/**
 * @date 5/11/2023
 * @brief Ajoute un élément dans le ballot à la position (l,c), l'élément
 * ne pourra plus être modifié
 *
 *
 * @param[in] b Ballot à modifer
 * @param[in] l Ligne de destination
 * @param[in] c Colonne de destination
 * @param[in] v valeur à ajouter
 * @pre l < nb_ligne && c < nb_colonnes
 * @pre b != NULL
 * @pre value at (l,c) == ( @ref DEFAULT_VALUE )
 *
 * @return Adresse du ballot
 */
Bale *baleSetValue(Bale *b, unsigned int l, unsigned int c, int v);

/**
 * @date 5/11/2023
 * @brief Renvoie la valeur à la position (l,c) dans le ballot
 *
 *
 * @param[in] b Ballot à lire
 * @param[in] l Ligne de la valeur
 * @param[in] c Colonne de la valeur
 * @pre l < nb_ligne && c < nb_colonnes
 * @pre b != NULL
 *
 * @return la valeur en position (l, c)
 */
int baleGetValue(Bale *b, unsigned int l, unsigned int c);

/**
 * @date 14/11/2023
 * @brief Renvoie le nombre de votants (nombre lignes)
 *
 * @param[in] b Ballot à utiliser
 * @pre b != NULL
 *
 * @return nombre de votants
 */
unsigned int baleNbVoter(Bale *b);

/**
 * @date 14/11/2023
 * @brief Renvoie le nombre de candidats (nombre colonnes)
 *
 * @param[in] b Ballot à utiliser
 * @pre b != NULL
 *
 * @return nombre de candidats
 */
unsigned int baleNbCandidat(Bale *b);


/**
 * @date 5/11/2023
 * @brief Renvoie le label associé à la colonne
 *
 * @param[in] b Ballot à utiliser
 * @param[in] c numéro de la colonne
 * @pre b != NULL
 * @pre c < nb_colonnes
 *
 * @return label
 * @note label retourné doit être free()
 */
char *baleColumnToLabel(Bale *b, unsigned int c);

/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/

/**
 * @date 5/11/2023
 * @brief Renvoie la/les plus petite valeur du ballot, d'une ligne ou d'une colonne
 *
 * @param[in] b Pointeur vers le ballot
 * @param[in] l Ligne cible (-1 pour toutes les lignes)
 * @param[in] c Colonne cible (-1 pour toutes les colonnes)
 * @pre -1 <= l < nb_ligne && -1 <= c < nb_colonnes
 * @pre b != NULL
 *
 * @return Une liste générique de tableaux d'entiers de taille 3 contenant [valeur_min, ligne, colonne]
 */
GenList *baleMin(Bale *b, int l, int c);


/**
 * @date 5/11/2023
 * @brief Copie un ballot
 *
 * @param[in] b Pointeur vers le ballot source (à copier)
 * @pre b != NULL
 *
 * @return Pointeur vers la copie
 */

Bale *baleCopy(Bale *b);


#endif