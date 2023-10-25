/*-----------------------------------------------------------------*/
/**
    Projet REV party - Structure Matrice header
    \file matrix.h
    \author : Ugo VALLAT
    \date Date de création : 25/10/2023
 
 */
/*-----------------------------------------------------------------*/

/**
 * @brief Cette librairie implémente implémente une matrice d'entiers
 *
 * @note Matrice de taille fixe pour garrantir un accès en lecture et
 *  en écriture de l'ordre de O(1)
 *
 * @note Implémente un nombre varié d'accesseur et d'opérateurs pour
 * à privilégiés pour de meilleures performances.
 */


#ifndef __MATRIX__H__
#define __MATRIX__H__
#include <stdio.h>
#include <stdbool.h>


/*------------------------------------------------------------------*/
/*                        STRUCTURE MATRIX                           */
/*------------------------------------------------------------------*/


/* Définition opaque de la structure Matrix */
typedef struct s_matrix Matrix;
typedef Matrix * ptrMatrix;



/** \brief Crée une matrice de dimension fixe
    \param[in] nbl Nombre de lignes
    \param[in] nbc Nombre de colonnes
    \param[in] default_value valeur par défault à mettre dans les case (correspond à une case vide)
    \return pointeur vers la matrice, NULL si erreur
    \date  25/10/2023
*/
ptrMatrix createMatrix(int nbl, int nbc, int default_value);


/** \brief Supprime la matrice et ses données et libère la mémoire 
    \param[in] m matrice à supprimer
    \date  25/10/2023
*/
void deleteMatrix(ptrMatrix m);


/** \brief Insert un élément dans la matrice à la position (l,c)
    \param[in] m Pointeur vers la matrice
    \param[in] l Ligne de destination (l,c)
    \param[in] c Colonne de destination (l,c)
    \param[in] v valeur à ajouter
    \return Adresse de la matrice, Null si erreur
    \pre (0,0) <= (l,c) < (nb_lignes, nb_colonnes)
    \date  25/10/2023
*/
ptrMatrix matrixInsert(ptrMatrix m, int l, int c, int v);


/** \brief Supprime la valeur en position (l,c) et la remplace par la valeur default_value
    \param[in] m pointeur vers la matrice
    \param[in] l Ligne de la supprossion (l,c)
    \param[in] c Colonne de la suppression(l,c)
    \return -1 if error, 0 else
    \pre (0,0) <= (l,c) < (nb_lignes, nb_colonnes)
    \date  25/10/2023
*/
int matrixRemove(ptrMatrix m, int l, int c);


/**
 * @brief Renvoie la valeur à la position (l,c) dans la matrice
 * 
 * @param[in] m Pointeur vers la matrice
 * @param[in] l Ligne de la valeur
 * @param[in] c Colonne de la valeur
 * @return la valeur en position (l,c) 
 * @pre (0,0) <= (l,c) < (nb_lignes, nb_colonnes)
 * @date 25/10/2023
 */
int matrixGet(ptrMatrix m, int l, int c);


/**
 * \brief Renvoie les dimension de la matrice sous la forme [nb_lignes, nb_colonnes]
 * \param[in] m Pointeur vers la matrice
 * \return tableau d'entier de taille 2 : [nombre_lignes, nombre_colonnes]
 * \date  25/10/2023
 */

unsigned int* matrixShape(ptrMatrix m);




/*------------------------------------------------------------------*/
/*                          ITERATEUR                               */
/*------------------------------------------------------------------*/

/* Définition opaque de la structure MatrixIte */
typedef struct s_matrix_ite MatrixIte;
typedef struct s_matrix_ite* ptrMatrixIte;


/**
 * @brief Définie le type des fonctions prise en paramêtre de l'itérateur
 * @param[in] v Valeur de la case courante
 * @param[in] l Ligne courante
 * @param[in] c Colonne courante
 * @param[in] buff pointeur vers un buffer pouvant stocker un résultat
 * @return nouvelle valeur de la case
 *  
 */
typedef int (*fun_ite) (int v, int l, int c, void* buff);



/**
 * @brief Crée un itérateur sur la matrice passée en entrée
 * 
 * @param[in] m Pointeur vers la matrice à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément, si NULL aucun traitement
 * n'est appliqué et la matrice n'est pas modifiée
 * @param[in] buff pointeur vers le buffer fourni à fun
 * @return pointeur vers l'itérateur, NULL si erreur
 * @date 25/10/2023
 */
ptrMatrixIte createMatrixIte(ptrMatrix m, int l, int c, fun_ite fun, void* buff);


/**
 * @brief Initialise l'itérateur selon ses paramêtres
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return pointeur vers l'itérateur, NULL si erreur
 * @date 25/10/2023
 */
ptrMatrixIte initMatrixIte(ptrMatrixIte ite);


/**
 * @brief Renvoie si il reste des éléments à parcourir
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return true si il reste des éléments, sinon false
 * @date 25/10/2023
 */
bool matrixIteHasNext(ptrMatrixIte ite);

/**
 * @brief Décale l'itérateur sur le prochain élément, renvoie sa valeur avant modification,
 * applique la fonction fun et remplace la valeur par l'élément de retour de fun
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return valeur avant modification
 * @pre doit avoir un prochain élément 
 * @date 25/10/2023
 */
int matrixIteNext(ptrMatrixIte ite);


/**
 * @brief Renvoie la valeur courrante après application de la fonction fun
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return élément courant
 * @pre doit être sur un élément
 * @date 25/10/2023
 */
int matrixIteGetValue(ptrMatrixIte ite);


/**
 * @brief Supprime l'itérateur et libère la mémoire
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @date 25/10/2023
 */
void deleteMatrixIte(ptrMatrixIte ite);






/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/


/**
 * @brief Applique la fonction fun à tous les éléments spédifiés de la matrice
 * 
 * @param[in] m Pointeur vers la matrice à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément, si NULL aucun traitement
 * n'est appliqué et la matrice n'est pas modifiée
 * @param[in] buff pointeur vers le buffer fourni à fun
 * @return -1 si erreur, 0 sinon
 * @date 25/10/2023
 */
int matrixMap(ptrMatrix m, int l, int c, fun_ite fun, void* buff);


/**
 * @brief Renvoie la somme des élément de la matrice, d'une ligne ou d'une colonne
 * 
 * @param m Pointeur vers la matrice
 * @param l Ligne cible (-1 pour toutes les lignes)
 * @param c Colonne cible (-1 pour toutes les colonnes)
 * @return somme des éléments
 * @date 25/10/2023
 */
int matrixSom(ptrMatrix m, int l, int c);


/**
 * @brief Renvoie la plus grande valeur de la matrice, d'une ligne ou d'une colonne
 * 
 * @param m Pointeur vers la matrice
 * @param l Ligne cible (-1 pour toutes les lignes)
 * @param c Colonne cible (-1 pour toutes les colonnes)
 * @return Tableau d'entiers de taille 3 contenant [valeur_max, ligne, colonne]
 * @date 25/10/2023
 */
int* matrixMax(ptrMatrix m, int l, int c);

/**
 * @brief Renvoie la plus petite valeur de la matrice, d'une ligne ou d'une colonne
 * 
 * @param m Pointeur vers la matrice
 * @param l Ligne cible (-1 pour toutes les lignes)
 * @param c Colonne cible (-1 pour toutes les colonnes)
 * @return Tableau d'entiers de taille 3 contenant [valeur_min, ligne, colonne]
 * @date 25/10/2023
 */
int* matrixMin(ptrMatrix m, int l, int c);


typedef bool (*fun_filter)(int v, int l, int c);

/**
 * @brief Filtre la matrice et renvoie une copie de la matrice avec uniquement
 * les éléments tel que fun(elem) = true
 * 
 * @param m Pointeur vers la matrice
 * @param fun Fonction de filtrage de type fun_filter
 * @return Pointeur vers la copie filtrée
 * @date 25/10/2023
 */
ptrMatrix matrixFilter(ptrMatrix m, fun_filter fun);


/**
 * @brief Copie une matrice dans une autre de dimension égale ou supérieure
 * 
 * @param[in] matrix_src Pointeur vers la matrice source (à copier)
 * @param[in] matrix_dest Pointeur vers la matrice de destination (copie)
 * @return -1 si erreur, 0 sinon
 * @pre: (l_src, c_src) <= (l_dest,c_dest)
 * @date 25/10/2023
 */

int matrixCopy(ptrMatrix matrix_src, ptrMatrix matrix_dest);



/**
 * @brief Afficher la matrice dans la sortie standard stdout
 * 
 * @param m Matrice à afficher
 * @date 25/10/2023
 */
void displayMatrix(ptrMatrix m);






/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

/**
 * @brief Affiche dans le logger toutes les informations sur la matrice
 * 
 * @param[in] m matrice à logger
 * @date 25/10/2023
 */
void printListLog(ptrMatrix m);

#endif

#endif
