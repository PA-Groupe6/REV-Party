/*-----------------------------------------------------------------*/
/**
 * @file bale.h
 * @author VALLAT Ugo, LAFORGE Mateo
 * @date 25/10/2023
 * @brief Structure Ballot header
 * 
 * @brief Un ballot est une matrice où chaque ligne correspond à un électeur et chaque colonne à
 * un candidat.
 * les colonnes son étiquetée avec le nom du candidat.
 * Les ballot est de taille fixe et ses éléments ne sont pas modifiables après avoir été chargés
 * dans le ballot.
 *
 * @remark Les dimensions d'un ballot sont fixes
*/
/*-----------------------------------------------------------------*/
#ifndef __BALE_H__
#define __BALE_H__
#include <stdio.h>
#include <stdbool.h>

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
 * @date 26/10/2023
 * @brief Définition opaque de la structure Bale
*/
typedef struct s_bale Bale;
typedef Bale * ptrBale;


/**
 * @date  26/10/2023
 * @brief Crée un ballot
 * 
 * @param[in] nbl Nombre de lignes de données (hors étiquettes)
 * @param[in] nbc Nombre de colonnes de données (hors étiquettes)
 * 
 * @return pointeur vers le ballot, NULL si erreur
*/
Bale* createBale(unsigned int nbl, unsigned int nbc);


/** 
 * @date 26/10/2023
 * @brief Supprime le ballot et ses données en libérant la mémoire 
 * 
 * @param[in] b pointeur vers le ballot à supprimer
*/
void deleteBale(ptrBale* b);


/**
 * @date 26/10/2023
 * @brief Insert un élément dans le ballot à la position (l,c)
 * 
 * @param[in] b Ballot à modifer
 * @param[in] l Ligne de destination
 * @param[in] c Colonne de destination
 * @param[in] v valeur à ajouter
 * @pre l < nb_ligne && c < nb_colonnes                                /!\ UTF-8 chars?
 * 
 * @return Adresse du ballot
 * @note renvoie NULL si erreur
*/
Bale* baleInsert(Bale *b, unsigned int l, unsigned int c, int v);


/** 
 * @date 26/10/2023
 * @brief Supprime la valeur en position (l, c),
 * se traduira par une substition par @ref DEFAULT_VALUE
 * 
 * @param[in] b Ballot à modifer
 * @param[in] l Ligne de la suppression
 * @param[in] c Colonne de la suppression
 * @pre l < nb_ligne && c < nb_colonnes
 * 
 * @return -1 si erreur, 0 sinon
*/
int baleRemove(Bale* b, unsigned int l, unsigned int c);


/**
 * @date 26/10/2023
 * @brief Renvoie la valeur à la position (l,c) dans le ballot
 * 
 * @param[in] b Ballot à utiliser
 * @param[in] l Ligne de la valeur
 * @param[in] c Colonne de la valeur
 * @pre l < nb_ligne && c < nb_colonnes
 * 
 * @return la valeur en position (l, c) 
 */
int baleGet(Bale* b, unsigned int l, unsigned int c);


/**
 * @date 26/10/2023
 * @brief Renvoie les dimension du ballot sous la forme d'un couple (nb_lignes, nb_colonnes)
 * 
 * @param[in] b Ballot à utiliser
 * 
 * @return tableau d'entier de taille 2 : [nombre_lignes, nombre_colonnes]
 */
unsigned int* baleShape(Bale* b);


/**
 * @date 26/10/2023
 * @brief Met une étiquette à une colonne
 * @note Par défaut l'étiquette vaut NULL
 * 
 * @param[in] b Ballot à modifier
 * @param[in] c Indice de la colonne
 * @param[in] label Etiquette
 * @pre c < nb_colonnes
 * 
 * @return -1 si erreur, 0 sinon
 */
int baleSetLabel(Bale* b, unsigned int c, char* label);

/**
 * @date 26/10/2023
 * @brief Renvoie le numéro de la colonne associée à l'étiquette
 * 
 * @param[in] b Ballot à utiliser
 * @param[in] label  Etiquette de la colonne recherchée
 * 
 * @return numéro de la colonne
 * @note renvoie -1 si erreur
 */
int baleGetColumn(Bale* b, char* label);




/*------------------------------------------------------------------*/
/*                          ITERATEUR                               */
/*------------------------------------------------------------------*/

/**
 * @date 26/10/2023
 * @brief Définition opaque de la structure BaleIte
*/
typedef struct s_baleIte BaleIte;
typedef struct s_baleIte* ptrBaleIte;


/**
 * @date 26/10/2023
 * @brief Définie le type des fonctions prise en paramêtre de l'itérateur
 * 
 * @param[in] v Valeur de la case courante
 * @param[in] l Ligne courante
 * @param[in] c Colonne courante
 * @param[in] buff pointeur vers un buffer pouvant stocker un résultat
 * 
 */
typedef void (*fun_ite_bale) (int v, unsigned int l, unsigned int c, void* buff);


/**
 * @date 26/10/2023
 * @brief Crée un itérateur sur le ballot passé en entrée et le positionne
 * avant le premier élément
 * 
 * @param[in] b Ballot à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément, si NULL aucun traitement
 * n'est appliqué et le ballot n'est pas modifiée
 * @param[in] buff pointeur vers le buffer fourni à fun
 * 
 * @return Bale sur laquelle l'itérateur se basera
 * @note renvoie NULL si erreur
 */
BaleIte* createBaleIte(Bale* b, int l, int c, fun_ite_bale fun, void* buff);



/**
 * @date 26/10/2023
 * @brief Renvoie vrai si il reste des éléments à parcourir
 * 
 * @param[in] ite Itérateur questionné
 * 
 * @return true si il reste des éléments, sinon false
 */
bool baleIteHasNext(BaleIte* ite);

/**
 * @date 26/10/2023
 * @brief Décale l'itérateur sur le prochain élément, renvoie sa valeur avant modification,
 * applique la fonction fun et remplace la valeur par l'élément de retour de fun
 * 
 * @param[in] ite Itérateur à utiliser
 * @pre baleIteHasNext(ite) == true
 * 
 * @return Valeur courante
 */
int baleIteNext(BaleIte* ite);


/**
 * @date 26/10/2023
 * @brief Renvoie la valeur courrante après application de la fonction fun
 * 
 * @param[in] ite Itérateur à utiliser
 * @pre ite != initBaleIte(ite)
 * 
 * @return Valeur de l'élément de courant
 */
int baleIteGetValue(BaleIte* ite);


/**
 * @date 26/10/2023
 * @brief Supprime l'itérateur et libère la mémoire
 * 
 * @param[in] ite pointeur vers l'itérateur
 */
void deleteBaleIte(ptrBaleIte* ite);






/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/


/**
 * @date 26/10/2023
 * @brief Applique la fonction fun à tous les éléments spécifiés du ballot sans
 * les modifier
 * 
 * @param[in] b Ballot à parcourir
 * @param[in] l Ligne à parcourir (-1 pour parcourir toutes les lignes)
 * @param[in] c Colonne à parcourir (-1 pour parcourir toutes les colonnes)
 * @param[in] fun Fonction à appliquer à chaque élément, si NULL aucun traitement
 * n'est appliqué
 * @param[in] buff pointeur vers le buffer fourni à fun
 * 
 * @return -1 si erreur, 0 sinon
 */
int baleMap(Bale* b, int l, int c, fun_ite_bale fun, void* buff);


/**
 * @date 26/10/2023
 * @brief Renvoie la somme des élément positifs du ballot, d'une ligne ou d'une colonne
 * 
 * @param[in] b Pointeur vers le ballot
 * @param[in] l Ligne cible (-1 pour toutes les lignes)
 * @param[in] c Colonne cible (-1 pour toutes les colonnes)
 * 
 * @return somme des éléments positifs
 */
unsigned int baleSom(Bale* b, int l, int c);


/**
 * @brief Renvoie la plus grande valeur du ballot, d'une ligne ou d'une colonne
 * 
 * @param m Pointeur vers le ballot
 * @param l Ligne cible (-1 pour toutes les lignes)
 * @param c Colonne cible (-1 pour toutes les colonnes)
 * @return Tableau d'entiers de taille 3 contenant [valeur_max, ligne, colonne]
 * @date 25/10/2023
 */
int* baleMax(Bale* b, int l, int c);

/**
 * @brief Renvoie la plus petite valeur du ballot, d'une ligne ou d'une colonne
 * 
 * @param m Pointeur vers le ballot
 * @param l Ligne cible (-1 pour toutes les lignes)
 * @param c Colonne cible (-1 pour toutes les colonnes)
 * @return Tableau d'entiers de taille 3 contenant [valeur_min, ligne, colonne]
 * @date 25/10/2023
 */
int* baleMin(Bale* b, int l, int c);


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
 * @param b Pointeur vers le ballot
 * @param fun Fonction de filtrage de type fun_filter
 * @return Pointeur vers la copie filtrée
 * @date 25/10/2023
 */
Bale* baleFilter(Bale* b, fun_filter fun);


/**
 * @brief Copie un ballot source vers un ballot destination de même taille
 * 
 * @param[in] bale_src Pointeur vers le ballot source (à copier)
 * @param[in] bale_dest Pointeur vers le ballot de destination (copie)
 * @return -1 si erreur, 0 sinon
 * @pre: (l_src, c_src) <= (l_dest,c_dest)
 * @date 25/10/2023
 */

int baleCopy(Bale* bale_src, Bale* bale_dest);



/**
 * @brief Afficher le ballot dans la sortie standard stdout
 * 
 * @param m Ballot à afficher
 * @date 25/10/2023
 */
void displayBale(Bale* b);






/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

void printBaleLog(Bale* b);

#endif

#endif