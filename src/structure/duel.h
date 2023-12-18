/**
 * @file duel.h
 * @author VALLAT Ugo, LUDWIG Corentin
 * @brief Structure Duel header
 *
 * @brief Une matrice de duels correspond à une matric où chaque ligne correspond au
 * score d'un candidat en duel face à un autre candidat de la colonne
 *
 * Les lignes et les colonnes son étiquetée avec le nom du candidat.
 *
 * Il y a autant de lignes que de colonnes et le label d'une ligne et d'une colonne de même
 * indice est similaire
 *
 * La matrice de duels est de taille fixe et ses éléments ne sont pas modifiables après avoir été
 * chargés dans la matrice.
 *
 * @note Taille maximum des étiquetées = @ref MAX_LENGHT_LABEL
 *
 * @remark Les dimensions d'une matrice de duels sont fixes
 *
 * @remark En cas d'erreur, toutes les fonctions de la matrice de duels exit le progamme avec un
 * message d'erreur
 */

#ifndef __DUEL_H__
#define __DUEL_H__
#include "genericlist.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include "matrix.h"
#include "bale.h"

/**
 * @def DEFAULT_VALUE
 * @brief valeur par défaut d'une case
 * @note ne doit pas faire partie de l'ensemble des valeurs possibles des éléments stockés
 */
#define DEFAULT_VALUE -2

/*------------------------------------------------------------------*/
/*                         STRUCTURE DUEL                           */
/*------------------------------------------------------------------*/

/**
 * @date 5/11/2023
 * @brief Définition opaque de la structure Duel
 */
typedef struct s_duel Duel;
typedef Duel *ptrDuel;

/**
 * @date  5/11/2023
 * @brief Crée une matrice de duels
 *
 * @param[in] nbl Nombre de lignes de données (hors étiquettes)
 * @param[in] nbc Nombre de colonnes de données (hors étiquettes)
 * @param[in] labels Liste générique des labels (liste de char*)
 * @pre genericListSize(labels) == nbc && Forall x in labels, typeof(x) == char*
 *
 * @return pointeur vers la matrice de duels
 * @post Les labels sont ajoutés au colonne dans le même ordre que la liste
 */
Duel *createDuel(unsigned int nb, GenList *labels);

/**
 * @date 5/11/2023
 * @brief Supprime la matrice de duels et ses données en libérant la mémoire
 * @pre d != NULL
 *
 * @param[in] d pointeur vers la matrice de duels à supprimer
 */
void deleteDuel(ptrDuel *d);

/**
 * @date 5/11/2023
 * @brief Ajoute un élément dans la matrice de duels à la position (l,c), l'élément
 * ne pourra plus être modifié
 *
 * @param[in] d Matrice de duels à remplir
 * @param[in] l Ligne de destination
 * @param[in] c Colonne de destination
 * @param[in] v valeur à ajouter
 * @pre l < nb_ligne && c < nb_colonnes
 * @pre d != NULL
 *
 * @return Adresse de la matrice de duels
 */
Duel *duelSetValue(Duel *d, unsigned int l, unsigned int c, int v);

/**
 * @date 5/11/2023
 * @brief Renvoie la valeur à la position (l,c) dans la matrice de duel
 *
 * @param[in] d matrice de duels à lire
 * @param[in] l Ligne de la valeur
 * @param[in] c Colonne de la valeur
 * @pre l < nb_ligne && c < nb_colonnes
 * @pre d != NULL
 *
 * @return la valeur en position (l, c)
 */
int duelGetValue(Duel *d, unsigned int l, unsigned int c);

/**
 * @date 14/11/2023
 * @brief Donne le nombre de candidats (nombre de lignes / colonnes)
 *
 * @param[in] d matrice de duels à utiliser
 * @pre d != NULL
 *
 * @return Nombre de candidat
 */
unsigned int duelNbCandidat(Duel *d);


/**
 * @date 5/11/2023
 * @brief Renvoie le label associé à l'indice de la ligne / colonne
 *
 * @param[in] d matrice de duels à utiliser
 * @param[in] index  indice de la ligne / colonne
 * @pre d != NULL
 * @pre c < nb_ligne
 *
 * @return Label associé
 */
char *duelIndexToLabel(Duel *d, unsigned int index);



/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/

/**
 * @date 5/11/2023
 * @brief Copie une matrice de duels
 *
 * @param[in] d Pointeur vers la matrice de duels source (à copier)
 * @pre d != NULL
 *
 * @return Pointeur vers la copie
 */

Duel *duelCopy(Duel *d);


/**
 * @date 16/12/2023
 * @author Ugo VALLAT
 * @brief Converti un ballot en duel
 * 
 * @param[in] b Ballot à vonvertir
 * @return Duel* 
 */
Duel* duelFromBale(Bale *b);

#endif