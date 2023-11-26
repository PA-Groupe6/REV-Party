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
Duel *createDuel(unsigned int nbl, unsigned int nbc, GenList *labels);

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
 * @brief Renvoie le numéro de la colonne associée au label
 *
 * @param[in] d matrice de duels à utiliser
 * @param[in] label  label de la colonne recherchée
 * @pre d != NULL
 *
 * @return numéro de la colonne
 */
int duelLabelToColumn(Duel *d, char *label);

/**
 * @date 5/11/2023
 * @brief Renvoie l'indice de la ligne/colonne associé au label
 *
 * @param[in] d matrice de duels à utiliser
 * @param[in] label  label de la ligne recherchée
 * @pre d != NULL
 * @pre Exist label
 *
 * @return Indice associé
 */
int duelLabelToIndex(Duel *d, char *label);

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
/*                          ITERATEUR                               */
/*------------------------------------------------------------------*/

/**
 * @date 5/11/2023
 * @brief Définition opaque de la structure DuelIte
 */
typedef struct s_duelIte DuelIte;
typedef struct s_duelIte *ptrDuelIte;

/**
 * @date 5/11/2023
 * @brief Définie le type des fonctions prise en paramêtre de l'itérateur
 *
 * @param[in] v Valeur de la case courante
 * @param[in] l Ligne courante
 * @param[in] c Colonne courante
 * @param[in] buff pointeur vers un buffer pouvant stocker un résultat
 *
 */
typedef void (*fun_ite_Duel)(int v, unsigned int l, unsigned int c, void *buff);

/**
 * @date 5/11/2023
 * @brief Crée un itérateur sur la matrice de duels passé en entrée et le positionne
 * avant le premier élément
 *
 * @param[in] d matrice de duels à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément sans modification de la matrice de duels,
 * si NULL aucun traitement n'est effectué et l'itérateur ne renvoie que la valeur courante
 * @param[in] buff pointeur vers le buffer fourni à fun
 * @pre d != NULL
 *
 * @return Pointeur vers l'itérateur
 */
DuelIte *createDuelIte(Duel *d, int l, int c, fun_ite_Duel fun, void *buff);

/**
 * @date 5/11/2023
 * @brief Renvoie vrai si il reste des éléments à parcourir
 *
 * @param[in] ite Itérateur questionné
 * @pre ite != NULL
 *
 * @return true si il reste des éléments, sinon false
 */
bool duelIteHasNext(DuelIte *ite);

/**
 * @date 5/11/2023
 * @brief Décale l'itérateur sur le prochain élément, renvoie sa valeur et réalise
 *  le traitement de fun sans modification de la matrice de duels
 *
 * @param[in] ite Itérateur à utiliser
 * @pre DuelIteHasNext(ite) == true
 * @pre ite != NULL
 *
 * @return Valeur courante
 */
int duelIteNext(DuelIte *ite);

/**
 * @date 5/11/2023
 * @brief Renvoie la valeur courrante
 *
 * @param[in] ite Itérateur à utiliser
 * @pre Appel à next avant
 * @pre ite != NULL
 *
 * @return Valeur de l'élément de courant
 */
int duelIteGetValue(DuelIte *ite);

/**
 * @date 5/11/2023
 * @brief Supprime l'itérateur et libère la mémoire
 * @pre ite != NULL
 *
 * @param[in] ite pointeur vers l'itérateur
 * @return adresse du buffer
 */
void* deleteDuelIte(ptrDuelIte *ite);


/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/

/**
 * @date 5/11/2023
 * @brief Applique la fonction fun à tous les éléments spécifiés de la matrice de duels sans
 * les modifier
 *
 * @param[in] d matrice de duels à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément, si NULL aucun traitement
 * n'est effectué
 * @param[in] buff pointeur vers le buffer fourni à fun
 * @pre d != NULL
 */
void duelMap(Duel *d, int l, int c, fun_ite_Duel fun, void *buff);

/**
 * @date 5/11/2023
 * @brief Renvoie la somme des élément positifs de la matrice de duels, d'une ligne ou d'une colonne
 *
 * @param[in] d Pointeur vers la matrice de duel
 * @param[in] l Ligne cible (-1 pour toutes les lignes)
 * @param[in] c Colonne cible (-1 pour toutes les colonnes)
 * @pre d != NULL
 *
 * @return somme des éléments positifs
 */
int duelSom(Duel *d, int l, int c);

/**
 * @date 5/11/2023
 * @brief Renvoie la/les plus grande valeur de la matrice de duels, d'une ligne ou d'une colonne
 *
 * @param[in] d Pointeur vers la matrice de duel
 * @param[in] l Ligne cible (-1 pour toutes les lignes)
 * @param[in] c Colonne cible (-1 pour toutes les colonnes)
 * @pre d != NULL
 *
 * @return Une liste générique de tableaux d'entiers de taille 3 contenant [valeur_max, ligne,
 *colonne]
 **/
GenList *duelMax(Duel *d, int l, int c);

/**
 * @date 5/11/2023
 * @brief Renvoie la/les plus petite valeur du matrice de duels, d'une ligne ou d'une colonne
 *
 * @param[in] d Pointeur vers la matrice de duel
 * @param[in] l Ligne cible (-1 pour toutes les lignes)
 * @param[in] c Colonne cible (-1 pour toutes les colonnes)
 * @pre d != NULL
 *
 * @return Une liste générique de tableaux d'entiers de taille 3 contenant [valeur_min, ligne,
 *colonne]
 **/
GenList *duelMin(Duel *d, int l, int c);

typedef fun_filter_matrix fun_filter_duel;

/**
 * @date 5/11/2023
 * @brief Filtre la matrice de duels et renvoie une copie de la matrice de duels avec uniquement
 * les éléments tel que fun(elem) = true
 *
 * @param[in] d Pointeur vers la matrice de duel
 * @param[in] fun Fonction de filtrage de type fun_filter
 * @param[in] buff Buffer pour fun_filter
 * @pre d != NULL
 * @pre fun != NULL
 *
 * @return Pointeur vers la copie filtrée
 */
Duel *duelFilter(Duel *d, fun_filter_duel fun, void *buff);

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
 * @date 5/11/2023
 * @brief Afficher la matrice de duels dans la sortie standard stdout
 *
 * @param[in] d matrice de duels à afficher
 * @pre d != NULL
 */
void displayDuel(Duel *d);


#endif