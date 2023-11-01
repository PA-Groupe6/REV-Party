/**
 * @file matrix.h
 * @author VALLAT Ugo
 *
 * @brief Cette librairie implémente implémente une matrice d'entiers
 *
 * @note Matrice de taille fixe pour garrantir un accès en lecture et
 *  en écriture de l'ordre de O(1)
 *
 * @note Implémente un nombre varié d'accesseur et d'opérateurs pour
 * à privilégiés pour de meilleures performances.
 *
 * @remark En cas d'erreur, toutes les fonctions de la matrice exit le progamme avec un
 * message d'erreur
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

/*------------------------------------------------------------------*/
/*                        STRUCTURE MATRIX                           */
/*------------------------------------------------------------------*/

/* Définition opaque de la structure Matrix */
typedef struct s_matrix Matrix;
typedef Matrix *ptrMatrix;

/**
 * @author VALLAT ugo
 * @date  1/11/2023
 * @brief Crée une matrice de dimension fixe
 *
 * @param[in] nbl Nombre de lignes
 * @param[in] nbc Nombre de colonnes
 * @param[in] default_value valeur par défault à mettre dans les case (correspond à une case vide)
 *
 * @return pointeur vers la matrice
 */
Matrix *createMatrix(unsigned int nbl, unsigned int nbc, int default_value);

/**
 * @author VALLAT ugo
 * @date  1/11/2023
 * @brief Supprime la matrice et ses données et libère la mémoire
 *
 * @param[in] m matrice à supprimer
 */
void deleteMatrix(ptrMatrix *m);

/**
 * @author VALLAT ugo
 * @date  31/10/2023
 * @brief Positionne un élément dans la matrice à l'emplacement (l,c)
 *
 * @param[in] m Pointeur vers la matrice
 * @param[in] l Ligne de destination (l,c)
 * @param[in] c Colonne de destination (l,c)
 * @param[in] v valeur à ajouter
 *
 * @pre (l,c) < (nb_lignes, nb_colonnes)
 */
void matrixSet(Matrix *m, unsigned int l, unsigned int c, int v);

/**
 * @author VALLAT ugo
 * @date  31/10/2023
 * @brief Efface la valeur en position (l,c) et la remplace par la valeur default_value
 *
 * @param[in] m pointeur vers la matrice
 * @param[in] l Ligne de la suppression (l,c)
 * @param[in] c Colonne de la suppression(l,c)
 *
 * @pre (l,c) < (nb_lignes, nb_colonnes)
 */
void matrixErase(Matrix *m, unsigned int l, unsigned int c);

/**
 * @author VALLAT ugo
 * @date  1/11/2023
 * @brief Renvoie la valeur à la position (l,c) dans la matrice
 *
 * @param[in] m Pointeur vers la matrice
 * @param[in] l Ligne de la valeur
 * @param[in] c Colonne de la valeur
 *
 * @pre (l,c) < (nb_lignes, nb_colonnes)
 *
 * @return la valeur en position (l,c)
 */
int matrixGet(Matrix *m, unsigned int l, unsigned int c);

/**
 * @author VALLAT ugo
 * @date  1/11/2023
 * @brief Renvoie les dimension de la matrice sous la forme [nb_lignes, nb_colonnes]
 *
 * @param[in] m Pointeur vers la matrice
 *
 * @return tableau d'entiers naturels de taille 2 : [nombre_lignes, nombre_colonnes]
 */

unsigned int *matrixShape(Matrix *m);

/**
 * @author VALLAT ugo
 * @date  1/11/2023
 * @brief Ajouter une ligne à la position l
 *
 * @param[in] m Pointeur vers l'adresse de la matrice
 * @param[in] l Position de la ligne à ajouter
 *
 * @pre l <= nb_lignes
 */
void matrixInsertLine(Matrix *m, unsigned int l);

/**
 * @author VALLAT ugo
 * @date  1/11/2023
 * @brief Supprime la ligne l
 *
 * @param[in] m Pointeur vers l'adresse de la matrice
 * @param[in] l Position de la ligne à supprimer
 *
 * @pre l < nb_lignes
 */
void matrixRemoveLine(Matrix *m, unsigned int l);

/**
 * @author VALLAT ugo
 * @date  1/11/2023
 * @brief Ajouter une ligne à la position c
 *
 * @param[in] m Pointeur vers l'adresse de la matrice
 * @param[in] l Position de la ligne à ajouter
 *
 * @pre c <= nb_colonnes
 */
void matrixInsertColumn(Matrix *m, unsigned int c);

/**
 * @author VALLAT ugo
 * @date  1/11/2023
 * @brief Supprime la colonne c
 *
 * @param[in] m Pointeur vers l'adresse de la matrice
 * @param[in] l Position de la colonne à supprimer
 *
 * @pre c < nb_colonnes
 */
void matrixRemoveColumn(Matrix *m, unsigned int c);

/*------------------------------------------------------------------*/
/*                          ITERATEUR                               */
/*------------------------------------------------------------------*/

/* Définition opaque de la structure MatrixIte */
typedef struct s_matrixIte MatrixIte;
typedef struct s_matrixIte *ptrMatrixIte;

/**
 * @author VALLAT ugo
 * @date 1/11/2023
 * @brief Définie le type des fonctions prise en paramêtre de l'itérateur
 *
 * @param[in] v Valeur de la case courante
 * @param[in] l Ligne courante
 * @param[in] c Colonne courante
 * @param[in] buff pointeur vers un buffer pouvant stocker un résultat
 *
 * @return nouvelle valeur de la case
 **/
typedef int (*fun_ite)(int v, unsigned int l, unsigned int c, void *buff);

/**
 * @author VALLAT ugo
 * @date 1/11/2023
 * @brief Crée un itérateur sur la matrice passée en entrée et le positionne
 * avant le premier élément
 *
 * @param[in] m Pointeur vers la matrice à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément, si NULL aucun traitement
 * n'est appliqué et la matrice n'est pas modifiée
 * @param[in] buff pointeur vers le buffer fourni à fun
 *
 * @return pointeur vers l'itérateur
 *
 * @note la taille de la matrice ne doit pas être modifiée durant l'utilisation de
 * l'itérateur
 * @note Parcours les lignes et colonne par ordre croissant : (0,0) -> (0,1) -> ... -> (1,0) -> ...
 **/
MatrixIte *createMatrixIte(Matrix *m, int l, int c, fun_ite fun, void *buff);

/**
 * @author VALLAT ugo
 * @date 1/11/2023
 * @brief Renvoie si il reste des éléments à parcourir
 *
 * @param[in] ite pointeur vers l'itérateur
 *
 * @return true si il reste des éléments, sinon false
 **/
bool matrixIteHasNext(MatrixIte *ite);

/**
 * @author VALLAT ugo
 * @date 1/11/2023
 * @brief Décale l'itérateur sur le prochain élément, renvoie sa valeur avant modification,
 * applique la fonction fun et remplace la valeur par l'élément de retour de fun
 *
 * @param[in] ite pointeur vers l'itérateur
 *
 * @pre doit avoir un prochain élément
 *
 * @return valeur avant modification
 */
int matrixIteNext(MatrixIte *ite);

/**
 * @author VALLAT ugo
 * @date 1/11/2023
 * @brief Renvoie la valeur courrante après application de la fonction fun
 *
 * @param[in] ite pointeur vers l'itérateur
 *
 * @pre doit être sur un élément
 *
 * @return élément courant
 */
int matrixIteGetValue(MatrixIte *ite);

/**
 * @author VALLAT ugo
 * @date 1/11/2023
 * @brief Supprime l'itérateur et libère la mémoire
 *
 * @param[in] ite pointeur vers l'itérateur
 **/
void deleteMatrixIte(ptrMatrixIte *ite);

/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/

/**
 * @author VALLAT ugo
 * @date 1/11/2023
 * @brief Applique la fonction fun à tous les éléments spédifiés de la matrice
 *
 * @param[in] m Pointeur vers la matrice à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément, si NULL aucun traitement
 * n'est appliqué et la matrice n'est pas modifiée
 * @param[in] buff pointeur vers le buffer fourni à fun
 **/
void matrixMap(Matrix *m, unsigned int l, unsigned int c, fun_ite fun, void *buff);

/**
 * @author VALLAT ugo
 * @date 1/11/2023
 * @brief Renvoie la somme des élément de la matrice, d'une ligne ou d'une colonne
 *
 * @param[in] m Pointeur vers la matrice
 * @param[in] l Ligne cible (-1 pour toutes les lignes)
 * @param[in] c Colonne cible (-1 pour toutes les colonnes)
 *
 * @return somme des éléments
 **/
int matrixSom(Matrix *m, unsigned int l, unsigned int c);

/**
 * @author VALLAT ugo
 * @date 1/11/2023
 * @brief Renvoie la plus grande valeur de la matrice, d'une ligne ou d'une colonne
 *
 * @param[in] m Pointeur vers la matrice
 * @param[in] l Ligne cible (-1 pour toutes les lignes)
 * @param[in] c Colonne cible (-1 pour toutes les colonnes)
 *
 * @return Tableau d'entiers de taille 3 contenant [valeur_max, ligne, colonne]
 **/
int *matrixMax(Matrix *m, unsigned int l, unsigned int c);

/**
 * @author VALLAT ugo
 * @date1/11/2023
 * @brief Renvoie la plus petite valeur de la matrice, d'une ligne ou d'une colonne
 *
 * @param[in] m Pointeur vers la matrice
 * @param[in] l Ligne cible (-1 pour toutes les lignes)
 * @param[in] c Colonne cible (-1 pour toutes les colonnes)
 *
 * @return Tableau d'entiers de taille 3 contenant [valeur_min, ligne, colonne]
 **/
int *matrixMin(Matrix *m, unsigned int l, unsigned int c);

/**
 * @author VALLAT ugo
 * @date 1/11/2023
 * @brief Définie le type des fonctions prise en paramêtre du filter
 *
 * @param[in] v Valeur de la case courante
 * @param[in] l Ligne courante
 * @param[in] c Colonne courante
 *
 * @return true si l'élément est conservé, sinon false
 **/
typedef bool (*fun_filter)(int v, unsigned int l, unsigned int c);

/**
 * @author VALLAT ugo
 * @date 1/11/2023
 * @brief Filtre la matrice et renvoie une copie de la matrice avec uniquement
 * les éléments tel que fun(elem) = true
 *
 * @param[in] m Pointeur vers la matrice
 * @param[in] fun Fonction de filtrage de type fun_filter
 *
 * @return Pointeur vers la copie filtrée
 **/
Matrix *matrixFilter(Matrix *m, fun_filter fun);

/**
 * @author VALLAT ugo
 * @date 1/11/2023
 * @brief Copie une matrice
 *
 * @param[in] m Pointeur vers la matrice source (à copier)
 *
 * @pre (l_src, c_src) <= (l_dest,c_dest)
 * @return Pointeur vers la copie
 */

Matrix *matrixCopy(Matrix *m);

/**
 * @author VALLAT ugo
 * @date 1/11/2023
 * @brief Afficher la matrice dans la sortie standard stdout
 *
 * @param[in] m Matrice à afficher
 */
void displayMatrix(Matrix *m);

/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

/**
 * @brief Affiche dans le logger toutes les informations sur la matrice
 *
 * @param[in]  m matrice à logger
 * @author VALLAT ugo
 * @date 1/11/2023
 */
void printMatrixLog(Matrix *m);

#endif

#endif
