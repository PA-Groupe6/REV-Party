/*-----------------------------------------------------------------*/
/**
 * @file bale.h
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
#ifndef __BALE_H__
#define __BALE_H__
#include "genericlist.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
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
 * @param[in] nbl Nombre de lignes de données (hors étiquettes)
 * @param[in] nbc Nombre de colonnes de données (hors étiquettes)
 * @param[in] labels Liste générique des labels (liste de char*)
 * @pre genericListSize(labels) == nbc && Forall x in labels, typeof(x) == char*
 *
 * @return pointeur vers le ballot
 * @post Les labels sont ajoutés au colonne dans le même ordre que la liste
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
 * @param[in] b Ballot à modifer
 * @param[in] l Ligne de destination
 * @param[in] c Colonne de destination
 * @param[in] v valeur à ajouter
 * @pre l < nb_ligne && c < nb_colonnes
 * @pre b != NULL
 *
 * @return Adresse du ballot
 */
Bale *baleSetValue(Bale *b, unsigned int l, unsigned int c, int v);

/**
 * @date 5/11/2023
 * @brief Renvoie la valeur à la position (l,c) dans le ballot
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
 * @date 5/11/2023
 * @brief Renvoie les dimension du ballot sous la forme d'un couple (nb_lignes, nb_colonnes)
 *
 * @param[in] b Ballot à utiliser
 * @pre b != NULL
 *
 * @return tableau d'entier de taille 2 : [nombre_lignes, nombre_colonnes]
 */
unsigned int *baleShape(Bale *b);

/**
 * @date 5/11/2023
 * @brief Renvoie le numéro de la colonne associée à l'étiquette
 *
 * @param[in] b Ballot à utiliser
 * @param[in] label  Etiquette de la colonne recherchée
 * @pre b != NULL
 * @pre Exist label
 *
 * @return numéro de la colonne
 */
int baleLabelToColumn(Bale *b, char *label);

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
 */
char *baleColumnToLabel(Bale *b, unsigned int c);

/*------------------------------------------------------------------*/
/*                          ITERATEUR                               */
/*------------------------------------------------------------------*/

/**
 * @date 5/11/2023
 * @brief Définition opaque de la structure BaleIte
 */
typedef struct s_baleIte BaleIte;
typedef struct s_baleIte *ptrBaleIte;

/**
 * @date 5/11/2023
 * @brief Définie le type des fonctions prise en paramêtre de l'itérateur
 *
 * @param[in] v Valeur de la case courante
 * @param[in] l Ligne courante
 * @param[in] c Colonne courante
 * @param[in] buff pointeur vers un buffer pouvant stocker un résultat
 * @pre l < nb_ligne && c < nb_colonnes
 *
 */
typedef void (*fun_ite_bale)(int v, unsigned int l, unsigned int c, void *buff);

/**
 * @date 5/11/2023
 * @brief Crée un itérateur sur le ballot passé en entrée et le positionne
 * avant le premier élément
 *
 * @param[in] b Ballot à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément sans modification du ballot,
 * si NULL aucun traitement n'est effectué et l'itérateur ne renvoie que la valeur courante
 * @param[in] buff pointeur vers le buffer fourni à fun
 * @pre b != NULL
 * @pre -1 <= l < nb_ligne && -1 <= c < nb_colonnes
 *
 * @return Pointeur vers l'itérateur
 */
BaleIte *createBaleIte(Bale *b, int l, int c, fun_ite_bale fun, void *buff);

/**
 * @date 5/11/2023
 * @brief Renvoie vrai si il reste des éléments à parcourir
 *
 * @param[in] ite Itérateur questionné
 * @pre ite != NULL
 *
 * @return true si il reste des éléments, sinon false
 */
bool baleIteHasNext(BaleIte *ite);

/**
 * @date 5/11/2023
 * @brief Décale l'itérateur sur le prochain élément, renvoie sa valeur et réalise
 *  le traitement de fun sans modification du ballot
 *
 * @param[in] ite Itérateur à utiliser
 * @pre ite != NULL
 * @pre baleIteHasNext(ite) == true
 *
 * @return Valeur courante
 */
int baleIteNext(BaleIte *ite);

/**
 * @date 5/11/2023
 * @brief Renvoie la valeur courrante
 *
 * @param[in] ite Itérateur à utiliser
 * @pre ite != NULL
 * @pre Appel à next avant
 *
 * @return Valeur de l'élément de courant
 */
int baleIteGetValue(BaleIte *ite);

/**
 * @date 5/11/2023
 * @brief Supprime l'itérateur et libère la mémoire
 *
 * @param[in] ite pointeur vers l'itérateur
 * @pre ite != NULL
 * @pre *ite != NULL
 */
void deleteBaleIte(ptrBaleIte *ite);

/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/

/**
 * @date 5/11/2023
 * @brief Applique la fonction fun à tous les éléments spécifiés du ballot sans
 * les modifier
 *
 * @param[in] b Ballot à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément, si NULL aucun traitement
 * n'est effectué
 * @param[in] buff pointeur vers le buffer fourni à fun
 * @pre b != NULL
 * @pre -1 <= l < nb_ligne && -1 <= c < nb_colonnes
 */
void baleMap(Bale *b, int l, int c, fun_ite_bale fun, void *buff);

/**
 * @date 5/11/2023
 * @brief Renvoie la somme des élément positifs du ballot, d'une ligne ou d'une colonne
 *
 * @param[in] b Pointeur vers le ballot
 * @param[in] l Ligne cible (-1 pour toutes les lignes)
 * @param[in] c Colonne cible (-1 pour toutes les colonnes)
 * @pre b != NULL
 * @pre -1 <= l < nb_ligne && -1 <= c < nb_colonnes
 *
 * @return somme des éléments positifs
 */
int baleSom(Bale *b, int l, int c);

/**
 * @date 5/11/2023
 * @brief Renvoie la/les plus grande valeur du ballot, d'une ligne ou d'une colonne
 *
 * @param[in] b Pointeur vers le ballot
 * @param[in] l Ligne cible (-1 pour toutes les lignes)
 * @param[in] c Colonne cible (-1 pour toutes les colonnes)
 * @pre b != NULL
 * @pre -1 <= l < nb_ligne && -1 <= c < nb_colonnes
 *
 * @return Une liste générique de tableaux d'entiers de taille 3 contenant [valeur_max, ligne,
 *colonne]
 **/
GenList *baleMax(Bale *b, int l, int c);

/**
 * @date 5/11/2023
 * @brief Renvoie la/les plus petite valeur du ballot, d'une ligne ou d'une colonne
 *
 * @param[in] b Pointeur vers le ballot
 * @param[in] l Ligne cible (-1 pour toutes les lignes)
 * @param[in] c Colonne cible (-1 pour toutes les colonnes)
 * @pre b != NULL
 * @pre -1 <= l < nb_ligne && -1 <= c < nb_colonnes
 *
 * @return Une liste générique de tableaux d'entiers de taille 3 contenant [valeur_min, ligne,
 *colonne]
 **/
GenList *baleMin(Bale *b, int l, int c);

/**
 * @date 5/11/2023
 * @brief Définie le type des fonctions prise en paramêtre du filter
 *
 * @param[in] v Valeur de la case courante
 * @param[in] l Ligne courante
 * @param[in] c Colonne courante
 * @param[in] buff Buffer
 * @pre l < nb_ligne && c < nb_colonnes
 *
 * @return true si l'élément est conservé, sinon false
 */
typedef bool (*fun_filter)(int v, unsigned int l, unsigned int c, void *buff);

/**
 * @date 5/11/2023
 * @brief Filtre le ballot et renvoie une copie du ballot avec uniquement
 * les éléments tel que fun(elem) = true
 *
 * @param[in] b Pointeur vers le ballot
 * @param[in] fun Fonction de filtrage de type fun_filter
 * @param[in] buff Buffer pour fun_filter
 * @pre b != NULL
 * @pre fun != NULL
 *
 * @return Pointeur vers la copie filtrée
 */
Bale *baleFilter(Bale *b, fun_filter fun, void *buff);

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

/**
 * @date 5/11/2023
 * @brief Afficher le ballot dans la sortie standard stdout
 *
 * @param[in] b Ballot à afficher
 * @pre b != NULL
 */
void displayBale(Bale *b);

/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

/**
 * @date 5/11/2023
 * @brief Affiche dans le logger toutes les informations sur le ballot
 *
 * @param[in]  b ballot à logger
 * @pre b != NULL
 */
void printBaleLog(Bale *b);

#endif

#endif