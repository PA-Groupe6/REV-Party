
/**
 * @file genericlist.h
 * @author VALLAT Ugo 
 * @date 29/10/2023
 *
 * @brief Cette librairie implémente une liste générique pseudo statique d'entiers
 *
 * Implémente la liste sous forme d'un tableau statique et alloue
 * de la mémoire dynamiquement lorsque qu'il est plein
 *
 * La liste ne contient que des pointeur génériques vers la donnée (void*)
 *
 * @note haute performance en lecture( O(1) ) mais faible en écriture ( O(n))
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée, il est 
 * fortement recomandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur
 */


#ifndef __GENLIST_H__
#define __GENLIST_H__
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>


/*------------------------------------------------------------------*/
/*                     STRUCTURE LIST GENERIC                       */
/*------------------------------------------------------------------*/


/* Définition opaque de la structure list */
typedef struct s_gen_list GenList;
typedef GenList * ptrGenList;



/** 
 * @author VALLAT ugo
 * @date  29/10/2023
 * @brief Crée une liste vide
 *
 * @param[in] size Espace mémoire initial (en nombre d'éléments)
 *
 * @return pointeur vers la liste, NULL si erreur
*/
GenList* createList(unsigned int size);


/** 
 * @author VALLAT ugo
 * @date  29/10/2023
 * @brief Supprime la liste mais ne supprime pas la donées pointée
 *
 * @param[in] l liste à supprimer
*/
void deleteList(ptrGenList* l);


/** 
 * @author VALLAT ugo
 * @date  29/10/2023
 * @brief Ajoute l'élément à la fin de la liste
 *
 * @param[in] l Pointeur vers la liste
 * @param[in] v Valeur à ajouter
 *
 * @return nouvelle adresse de la liste, NULL si erreur
*/
GenList* listAdd(GenList* l, void* v);


/** 
 * @author VALLAT ugo
 * @date  29/10/2023
 * @brief Insert une valeur à la position i
 *
 * @param[in] l Pointeur vers la liste
 * @param[in] v Valeur à ajouter
 * @param[in] i position
 *
 * @pre i <= listSize
 *
 * @return nouvelle adresse de la liste, NULL si erreur
*/
GenList* listInsert(GenList* l, void* v, unsigned int i);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Supprime le dernier élément de la liste
 *
 * @param[in] l list
 *
 * @pre taille liste > 0
 *
 * @return -1 si erreur, 0 sinon
**/
int listPop(GenList* l);


/** 
 * @author VALLAT ugo
 * @date  29/10/2023
 * @brief Supprime l'élément à la position i
 *
 * @param[in] l Pointeur vers la liste
 * @param[in] i position
 *
 * @pre i < listSize
 *
 * @return -1 si erreur, 0 sinon
*/
int listRemove(GenList* l, unsigned int i);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Lire la valeur à la position i
 * 
 * @param[in] l Pointeur vers la liste
 * @param[in] i Position de l'élément
 *
 * @pre i < list size
 *
 * @return Valeur lue, NULL si erreur
 **/
void* listGet(GenList* l, unsigned int i);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 *
 * @brief Renvoie si la liste est vide
 *
 * @param[in] l Pointeur vers la liste
 * @return true si vide, false sinon
 */
bool listEmpty(GenList* l);

/**
 * @author VALLAT ugo
 * @date  29/10/2023
 * @brief Renvoie la taille de la liste (position + 1 du dernier élément)
 *
 * @param[in] l Pointeur vers la liste
 * @return taille de la liste, -1 si erreur
 *
 */

unsigned int listSize(GenList* l);

/**
 * @author VALLAT ugo
 * @date 25/10/2023
 * @brief Copie une liste source dans la liste destination de taille égale ou supérieure
 * 
 * @param[in] list_src liste source à copier
 * @param[in] list_dest liste destination
 * 
 * @pre size_src <= size_dest
 *
 * @return  -1 si erreur, 0 sinon
 *
 */

int listCopy(GenList* list_src, GenList* list_dest);



/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Afficher la liste dans la sortie par standard
 * 
 * @param[in] l liste à afficher
 *
 */
void displayList(GenList* l);





/*------------------------------------------------------------------*/
/*                         ITERATEUR                                */
/*------------------------------------------------------------------*/

typedef struct s_gen_list_ite GenListIte;
typedef struct s_gen_list_ite* ptrGenListIte;

/* l'itérateur commence au début de la liste */
#define FROM_BEGIN 1
/* l'itérateur commence à la fin de la liste */
#define FROM_END 0


/**
 * @author VALLAT ugo
 * @date 22/10/2023
 * @brief Crée un itérateur sur la liste passée en entrée qui la parcours
 * depuis le début ( @ref FROM_BEGIN) ou la fin ( @ref FROM_END)
 * 
 * @param[in] l liste à parcourrir
 * @param[in] dir sens de parcours de l'itérateur ( @ref FROM_BEGIN ou @ref FROM_END)
 *
 * @return pointeur vers l'itérateur, NULL si erreur
 **/
GenListIte* createListIte(GenList* l, int dir);



/**
 * @author VALLAT ugo
 * @date 22/10/2023
 * @brief Renvoie si il reste des éléments à parcourir dans la liste
 * 
 * @param[in] ite pointeur vers l'itérateur
 *
 * @return true si il reste des éléments, sinon false
 **/
bool listIteHasNext(GenListIte* ite);


/**
 * @author VALLAT ugo
 * @date 22/10/2023
 * @brief Décale l'itérateur sur le prochain élément et renvoie sa valeur
 * 
 * @param[in] ite pointeur vers l'itérateur
 *
 * @pre doit avoir un prochain élément 
 *
 * @return -1 si erreur, 0 sinon
 */
int listIteNext(GenListIte* ite);


/**
 * @author VALLAT ugo
 * @date 22/10/2023
 * @brief Renvoie la valeur courrante
 * 
 * @param[in] ite pointeur vers l'itérateur
 * 
 * @pre doit être sur un élément
 * 
 * @return élément courant, NULL si erreur
 */
void* listIteGetValue(GenListIte* ite);


/**
 * @author VALLAT ugo
 * @date 22/10/2023
 * @brief Supprime l'itérateur et libère la mémoire
 *
 * @param[in] ite pointeur vers l'itérateur
 **/
void deleteListIte(ptrGenListIte* ite);







/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

/**
 * @brief display in the logger all the information about the list and the elements
 * 
 * @param[in] l list to print in logger
 * @author VALLAT ugo
 * @date 29/10/2023
 */
void printListLog(GenList* l);

#endif

#endif