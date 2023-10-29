/*-----------------------------------------------------------------*/
/**
 * @file duel.h
 * @author VALLAT Ugo
 * @date 29/10/2023
 * @brief Structure Duel header
 * 
 * @brief Une matrice de duels correspond à une matric où chaque ligne correspond au 
 * score d'un candidat en duel face à un autre candidat de la colonne
 *
 * Les lignes et les colonnes son étiquetée avec le nom du candidat.
 *
 * Il y a autant de lignes que de colonnes et l'étiquette d'une ligne et d'une colonne de même indice est
 * similaire 
 *
 * La matrice de duels est de taille fixe et ses éléments ne sont pas modifiables après avoir été chargés
 * dans la matrice.
 *
 * @remark Les dimensions d'une matrice de duels sont fixes
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée, il est 
 * fortement recomandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur
*/
/*-----------------------------------------------------------------*/
#ifndef __DUEL_H__
#define __DUEL_H__
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

/**
 * @def DEFAULT_VALUE
 * @brief valeur par défaut d'une case
 * @note ne doit pas faire partie de l'ensemble des valeurs possibles des éléments stockés
*/
#define DEFAULT_VALUE -2


/*------------------------------------------------------------------*/
/*                         STRUCTURE DUEl                           */
/*------------------------------------------------------------------*/

/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Définition opaque de la structure Duel
*/
typedef struct s_duel Duel;
typedef Duel * ptrDuel;


/**
 * @author VALLAT ugo
 * @date  29/10/2023
 * @brief Crée une matrice de duels
 * 
 * @param[in] nbl Nombre de lignes de données (hors étiquettes)
 * @param[in] nbc Nombre de colonnes de données (hors étiquettes)
 * 
 * @return pointeur vers la matrice de duels, NULL si erreur
*/
Duel* createDuel(unsigned int nbl, unsigned int nbc);


/** 
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Supprime la matrice de duels et ses données en libérant la mémoire 
 * 
 * @param[in] d pointeur vers la matrice de duels à supprimer
*/
void deleteDuel(ptrDuel* d);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Ajoute un élément dans la matrice de duels à la position (l,c), l'élément
 * ne pourra plus être modifié
 * 
 * @param[in] d Matrice de duels à remplir
 * @param[in] l Ligne de destination
 * @param[in] c Colonne de destination
 * @param[in] v valeur à ajouter
 * @pre l < nb_ligne && c < nb_colonnes                                /!\ UTF-8 chars?
 * 
 * @return Adresse de la matrice de duels, NULL si erreur
*/
Duel* duelSetValue(Duel *d, unsigned int l, unsigned int c, int v);



/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Renvoie la valeur à la position (l,c) dans la matrice de duel
 * 
 * @param[in] d matrice de duels à lire
 * @param[in] l Ligne de la valeur
 * @param[in] c Colonne de la valeur
 * @pre l < nb_ligne && c < nb_colonnes
 * 
 * @return la valeur en position (l, c), -1 si erreur 
 */
int duelGet(Duel* d, unsigned int l, unsigned int c);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Renvoie les dimension de la matrice de duels sous la forme d'un couple (nb_lignes, nb_colonnes)
 * 
 * @param[in] d matrice de duels à utiliser
 * 
 * @return tableau d'entier de taille 2 : [nombre_lignes, nombre_colonnes], NULL si erreur
 */
unsigned int* duelShape(Duel* d);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Met une étiquette à la colonne et à la ligne d'indice ind
 * @note Par défaut l'étiquette vaut NULL
 * 
 * @param[in] d matrice de duels à modifier
 * @param[in] ind Indice de la colonne / ligne
 * @param[in] label Etiquette
 * @pre c < nb_colonnes
 * 
 * @return -1 si erreur, 0 sinon
 */
int duelSetLabel(Duel* d, unsigned int ind, char* label);

/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Renvoie le numéro de la colonne associée à l'étiquette
 * 
 * @param[in] d matrice de duels à utiliser
 * @param[in] label  Etiquette de la colonne recherchée
 * 
 * @return numéro de la colonne, -1 si erreur 
 */
int duelGetColumn(Duel* d, char* label);



/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Renvoie le numéro de la ligne associée à l'étiquette
 * 
 * @param[in] d matrice de duels à utiliser
 * @param[in] label  Etiquette de la ligne recherchée
 * 
 * @return numéro de la ligne,-1 si erreur 
 */
int duelGetLine(Duel* d, char* label);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Renvoie la valeur associée au duel label1 vs label2
 * 
 * @param[in] d matrice de duels à lire
 * @param[in] label1 nom du premier candidat
 * @param[in] label2 nom du deuxème candidat
 * @pre label1 et label2 sont des candidats valides
 * 
 * @return la valeur en position (l, c), -1 si erreur 
 */
int duelGetVS(Duel* d, char* label1, char* label2);





/*------------------------------------------------------------------*/
/*                          ITERATEUR                               */
/*------------------------------------------------------------------*/

/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Définition opaque de la structure DuelIte
*/
typedef struct s_duelIte DuelIte;
typedef struct s_duelIte* ptrDuelIte;


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Définie le type des fonctions prise en paramêtre de l'itérateur
 * 
 * @param[in] v Valeur de la case courante
 * @param[in] l Ligne courante
 * @param[in] c Colonne courante
 * @param[in] buff pointeur vers un buffer pouvant stocker un résultat
 * 
 */
typedef void (*fun_ite_Duel) (int v, unsigned int l, unsigned int c, void* buff);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Crée un itérateur sur la matrice de duels passé en entrée et le positionne
 * avant le premier élément
 * 
 * @param[in] d matrice de duels à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément sans modification de la matrice de duels,
 * si NULL aucun traitement n'est effectué et l'itérateur ne renvoie que la valeur courante
 * @param[in] buff pointeur vers le buffer fourni à fun
 * 
 * @return Pointeur vers l'itérateur, NULL si erreur
 */
DuelIte* createDuelIte(Duel* d, int l, int c, fun_ite_Duel fun, void* buff);



/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Renvoie vrai si il reste des éléments à parcourir
 * 
 * @param[in] ite Itérateur questionné
 * 
 * @return true si il reste des éléments, sinon false
 */
bool duelIteHasNext(DuelIte* ite);

/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Décale l'itérateur sur le prochain élément, renvoie sa valeur et réalise
 *  le traitement de fun sans modification de la matrice de duels
 * 
 * @param[in] ite Itérateur à utiliser
 * @pre DuelIteHasNext(ite) == true
 * 
 * @return Valeur courante, -1 si erreur
 */
int duelIteNext(DuelIte* ite);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Renvoie la valeur courrante
 * 
 * @param[in] ite Itérateur à utiliser
 * @pre ite != initDuelIte(ite)
 * 
 * @return Valeur de l'élément de courant, -1 si erreur 
 */
int duelIteGetValue(DuelIte* ite);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Supprime l'itérateur et libère la mémoire
 * 
 * @param[in] ite pointeur vers l'itérateur
 */
void deleteDuelIte(ptrDuelIte* ite);






/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Applique la fonction fun à tous les éléments spécifiés de la matrice de duels sans
 * les modifier
 * 
 * @param[in] d matrice de duels à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément, si NULL aucun traitement
 * n'est effectué
 * @param[in] buff pointeur vers le buffer fourni à fun
 * 
 * @return -1 si erreur , 0 sinon
 */
int duelMap(Duel* d, int l, int c, fun_ite_Duel fun, void* buff);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Renvoie la somme des élément positifs de la matrice de duels, d'une ligne ou d'une colonne
 * 
 * @param[in] d Pointeur vers la matrice de duel
 * @param[in] l Ligne cible (-1 pour toutes les lignes)
 * @param[in] c Colonne cible (-1 pour toutes les colonnes)
 * 
 * @return somme des éléments positifs, -1 si erreur 
 */
int duelSom(Duel* d, int l, int c);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Renvoie la plus grande valeur de la matrice de duels, d'une ligne ou d'une colonne
 * 
 * @param d Pointeur vers la matrice de duel
 * @param l Ligne cible (-1 pour toutes les lignes)
 * @param c Colonne cible (-1 pour toutes les colonnes)
 * @return Tableau d'entiers de taille 3 contenant [valeur_max, ligne, colonne], NULL si 
 * erreur 
 */
int* duelMax(Duel* d, int l, int c);

/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Renvoie la plus petite valeur du matrice de duels, d'une ligne ou d'une colonne
 * 
 * @param d Pointeur vers la matrice de duel
 * @param l Ligne cible (-1 pour toutes les lignes)
 * @param c Colonne cible (-1 pour toutes les colonnes)
 * @return Tableau d'entiers de taille 3 contenant [valeur_min, ligne, colonne], NULL si 
 * erreur 
 */
int* duelMin(Duel* d, int l, int c);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Définie le type des fonctions prise en paramêtre du filter
 * 
 * @param[in] v Valeur de la case courante
 * @param[in] l Ligne courante
 * @param[in] c Colonne courante
 * @return true si l'élément est conservé, sinon false
 */
typedef bool (*fun_filter)(int v, int l, int c);

/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Filtre la matrice de duels et renvoie une copie de la matrice de duels avec uniquement
 * les éléments tel que fun(elem) = true
 * 
 * @param d Pointeur vers la matrice de duel
 * @param fun Fonction de filtrage de type fun_filter
 * @return Pointeur vers la copie filtrée, NULL si 
 * erreur 
 */
Duel* duelFilter(Duel* d, fun_filter fun);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Copie une matrice de duels source vers une matrice de duels destination de même taille
 * 
 * @param[in] duel_src Pointeur vers la matrice de duels source (à copier)
 * @param[in] duel_dest Pointeur vers la matrice de duels de destination (copie)
 * @return -1 si erreur , 0 sinon
 * @pre: (l_src, c_src) <= (l_dest,c_dest)
 */

int duelCopy(Duel* duel_src, Duel* duel_dest);



/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Afficher la matrice de duels dans la sortie standard stdout
 * 
 * @param d matrice de duels à afficher
 */
void displayDuel(Duel* d);






/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

void printDuelLog(Duel* d);

#endif

#endif