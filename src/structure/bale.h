/*-----------------------------------------------------------------*/
/**
    Projet REV party - Structure Ballot header
    \file bale.h
    \author : Ugo VALLAT
    \date Date de création : 25/10/2023
 
 */
/*-----------------------------------------------------------------*/

/**
 * @brief Cette librairie implémente implémente la structure de données
 * ballots. Chaque ligne correspond à un électeur et chaque colonne à
 * un candidat. Chaque ligne et chaque colonne est donc étiquetée
 *
 * @note Matrice de taille fixe pour garrantir un accès en lecture et
 *  en écriture de l'ordre de O(1)
 *
 * @note Le ballot utilise la structure matrix
 */


#ifndef __BALE__H__
#define __BALE__H__
#include <stdio.h>
#include <stdbool.h>
#include "matrix.h"


/*------------------------------------------------------------------*/
/*                        STRUCTURE BALLOT                           */
/*------------------------------------------------------------------*/


/* Définition opaque de la structure Bale */
typedef struct s_bale Bale;
typedef Bale * ptrBale;



/** \brief Crée une ballot de dimension fixe
    \param[in] nbl Nombre de lignes de données (hors étiquettes)
    \param[in] nbc Nombre de colonnes de données (hors étiquettes)
    \return pointeur vers le ballot, NULL si erreur
    \date  25/10/2023
*/
ptrBale createBale(int nbl, int nbc);


/** \brief Supprime le ballot et ses données et libère la mémoire 
    \param[in] b ballot à supprimer
    \date  25/10/2023
*/
void deleteBale(ptrBale b);


/** \brief Insert un élément dans le ballot à la position (l,c)
    \param[in] b Pointeur vers le ballot
    \param[in] l Ligne de destination (l,c)
    \param[in] c Colonne de destination (l,c)
    \param[in] v valeur à ajouter
    \return Adresse du ballot, Null si erreur
    \pre (0,0) <= (l,c) < (nb_lignes, nb_colonnes)
    \date  25/10/2023
*/
ptrBale baleInsert(ptrBale b, int l, int c, int v);


/** \brief Supprime la valeur en position (l,c) et la remplace par la valeur -1
    \param[in] b pointeur vers le ballot
    \param[in] l Ligne de la suppression (l,c)
    \param[in] c Colonne de la suppression(l,c)
    \return -1 if error, 0 else
    \pre (0,0) <= (l,c) < (nb_lignes, nb_colonnes)
    \date  25/10/2023
*/
int baleRemove(ptrBale b, int l, int c);


/**
 * @brief Renvoie la valeur à la position (l,c) dans le ballot
 * 
 * @param[in] b Pointeur vers le ballot
 * @param[in] l Ligne de la valeur
 * @param[in] c Colonne de la valeur
 * @return la valeur en position (l,c) 
 * @pre (0,0) <= (l,c) < (nb_lignes, nb_colonnes)
 * @date 25/10/2023
 */
int baleGet(ptrBale b, int l, int c);


/**
 * \brief Renvoie les dimension du ballot sous la forme [nb_lignes, nb_colonnes]
 * \param[in] b Pointeur vers le ballot
 * \return tableau d'entier de taille 2 : [nombre_lignes, nombre_colonnes]
 * \date  25/10/2023
 */

unsigned int* baleShape(ptrBale b);



/**
 * @brief Met une étiquette à une colonne, par défaut l'étiquette vaut NULL
 * 
 * @param[in] b Pointeur vers le ballot
 * @param[in] c Indice de la colonne
 * @param[in] label Etiquette
 * @return -1 si erreur, 0 sinon
 * @pre 0 <= c < ballot_nb_colonnes
 */
int baleSetLabel(ptrBale b, int c, char* label);

/**
 * @brief Renvoie le numéro de la colonne associée à l'étiquette
 * 
 * @param[in] b Pointeur vers le ballot
 * @param[in] label  Etiquette de la colonne recherchée
 * @return numéro de la colonne, -1 sinon
 */
int baleGetColumn(ptrBale b, char* label);

/*------------------------------------------------------------------*/
/*                          ITERATEUR                               */
/*------------------------------------------------------------------*/

/* Définition opaque de la structure MatrixIte */
typedef struct s_baleIte BaleIte;
typedef struct s_matrixIte* ptrBaleIte;


/**
 * @brief Définie le type des fonctions prise en paramêtre de l'itérateur
 * @param[in] v Valeur de la case courante
 * @param[in] l Ligne courante
 * @param[in] c Colonne courante
 * @param[in] buff pointeur vers un buffer pouvant stocker un résultat
 * @return nouvelle valeur de la case
 * @date 25/10/2023
 */
typedef int (*fun_ite) (int v, int l, int c, void* buff);



/**
 * @brief Crée un itérateur sur le ballot passé en entrée
 * 
 * @param[in] b Pointeur vers le ballot à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément, si NULL aucun traitement
 * n'est appliqué et le ballot n'est pas modifiée
 * @param[in] buff pointeur vers le buffer fourni à fun
 * @return pointeur vers l'itérateur, NULL si erreur
 * @date 25/10/2023
 */
ptrBaleIte createBaleIte(ptrBale b, int l, int c, fun_ite fun, void* buff);


/**
 * @brief Initialise l'itérateur selon ses paramêtres
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return pointeur vers l'itérateur, NULL si erreur
 * @date 25/10/2023
 */
ptrBaleIte initBaleIte(ptrBaleIte ite);


/**
 * @brief Renvoie si il reste des éléments à parcourir
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return true si il reste des éléments, sinon false
 * @date 25/10/2023
 */
bool baleIteHasNext(ptrBaleIte ite);

/**
 * @brief Décale l'itérateur sur le prochain élément, renvoie sa valeur avant modification,
 * applique la fonction fun et remplace la valeur par l'élément de retour de fun
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return valeur avant modification
 * @pre doit avoir un prochain élément 
 * @date 25/10/2023
 */
int baleIteNext(ptrBaleIte ite);


/**
 * @brief Renvoie la valeur courrante après application de la fonction fun
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return élément courant
 * @pre doit être sur un élément
 * @date 25/10/2023
 */
int baleIteGetValue(ptrBaleIte ite);


/**
 * @brief Supprime l'itérateur et libère la mémoire
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @date 25/10/2023
 */
void deleteBaleIte(ptrBaleIte ite);






/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/


/**
 * @brief Applique la fonction fun à tous les éléments spédifiés du ballot
 * 
 * @param[in] b Pointeur vers le ballot à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément, si NULL aucun traitement
 * n'est appliqué et le ballot n'est pas modifiée
 * @param[in] buff pointeur vers le buffer fourni à fun
 * @return -1 si erreur, 0 sinon
 * @date 25/10/2023
 */
int baleMap(ptrBale b, int l, int c, fun_ite fun, void* buff);


/**
 * @brief Renvoie la somme des élément du ballot, d'une ligne ou d'une colonne
 * 
 * @param b Pointeur vers le ballot
 * @param l Ligne cible (-1 pour toutes les lignes)
 * @param c Colonne cible (-1 pour toutes les colonnes)
 * @return somme des éléments
 * @date 25/10/2023
 */
int baleSom(ptrBale b, int l, int c);


/**
 * @brief Renvoie la plus grande valeur du ballot, d'une ligne ou d'une colonne
 * 
 * @param m Pointeur vers le ballot
 * @param l Ligne cible (-1 pour toutes les lignes)
 * @param c Colonne cible (-1 pour toutes les colonnes)
 * @return Tableau d'entiers de taille 3 contenant [valeur_max, ligne, colonne]
 * @date 25/10/2023
 */
int* baleMax(ptrBale b, int l, int c);

/**
 * @brief Renvoie la plus petite valeur du ballot, d'une ligne ou d'une colonne
 * 
 * @param m Pointeur vers le ballot
 * @param l Ligne cible (-1 pour toutes les lignes)
 * @param c Colonne cible (-1 pour toutes les colonnes)
 * @return Tableau d'entiers de taille 3 contenant [valeur_min, ligne, colonne]
 * @date 25/10/2023
 */
int* baleMin(ptrBale b, int l, int c);


/**
 * @brief Définie le type des fonctions prise en paramêtre du filter
 * 
 * @param[in] v Valeur de la case courante
 * @param[in] l Ligne courante
 * @param[in] c Colonne courante
 * @return true si l'élément est conservé, sinon false
 * @date 25/10/2023
 */
typedef bool (*fun_filter)(int v, int l, int c);

/**
 * @brief Filtre le ballot et renvoie une copie du ballot avec uniquement
 * les éléments tel que fun(elem) = true
 * 
 * @param b Pointeur vers la matrice
 * @param fun Fonction de filtrage de type fun_filter
 * @return Pointeur vers la copie filtrée
 * @date 25/10/2023
 */
ptrBale matrixFilter(ptrBale b, fun_filter fun);


/**
 * @brief Copie un ballot source vers un ballot destination de même taille
 * 
 * @param[in] bale_src Pointeur vers le ballot source (à copier)
 * @param[in] bale_dest Pointeur vers le ballot de destination (copie)
 * @return -1 si erreur, 0 sinon
 * @pre: (l_src, c_src) <= (l_dest,c_dest)
 * @date 25/10/2023
 */

int baleCopy(ptrBale bale_src, ptrBale bale_dest);



/**
 * @brief Afficher le ballot dans la sortie standard stdout
 * 
 * @param m Ballot à afficher
 * @date 25/10/2023
 */
void displayBale(ptrBale b);






/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

/**
 * @brief Affiche dans le logger toutes les informations sur le ballot
 * 
 * @param[in] b ballot à logger
 * @date 25/10/2023
 */
void printListLog(ptrBale b);

#endif

#endif
