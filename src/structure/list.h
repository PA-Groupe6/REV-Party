/*-----------------------------------------------------------------*/
/**
 * Projet REV party - Structur list header
 * @file list.h
 * @author : VALLAT Ugo 
 * @date 20/10/2023
 
 */
/*-----------------------------------------------------------------*/

/**
 * @brief Cette librairie implémente une liste pseudo statique d'entiers
 *
 * Implémente la liste sous forme d'un tableau statique et alloue
 * de la mémoire dynamiquement lorsque qu'il est plein
 *
 * @note haute performance en lecture( O(1) ) mais faible en écriture ( O(n))
 *
 */


#ifndef __LIST_H__
#define __LIST_H__
#include <stdio.h>
#include <stdbool.h>


/*------------------------------------------------------------------*/
/*                        STRUCTURE LIST                            */
/*------------------------------------------------------------------*/


/* Définition opaque de la structure list */
typedef struct s_list List;
typedef List * ptrList;



/** 
 * @date  20/10/2023
 * @brief Crée une liste vide
 *
 * @param[in] size Espace mémoire initial (en nombre d'éléments)
 *
 * @return pointeur vers la liste, NULL si erreur+
*/
List* createList(unsigned int size);


/** 
 * @date  20/10/2023
 * @brief Supprime la liste et libère la mémoire
 *
 * @param[in] l liste à supprimer
*/
void deleteList(ptrList* l);


/** 
 * @date  20/10/2023
 * @brief Add value v to the end of the list
 *
 * @param[in] l pointer to the adress of the list
 * @param[in] v value to add
 *
 * @return new adress of the list, null if error
*/
List* listAdd(List* l, int v);


/** 
 * @date  20/10/2023
 * @brief Insert a value in the list to the position require
 *
 * @param[in] l pointer to the adress of the list
 * @param[in] v value to add
 * @param[in] i position
 *
 * @pre i <= listSize
 *
 * @return new adress of the list, null if error
*/
List* listInsert(List* l, int v, unsigned int i);


/**
 * @date 20/10/2023
 * @brief Supprime le dernier élément de la liste
 *
 * @param[in] l list
 *
 * @pre taille liste > 0
 *
 * @return -1 si erreur, 0 sinon
**/
int listPop(List* l);


/** 
 * @date  20/10/2023
 * @brief Remove a value in the list to the position require
 *
 * @param[in] l pointer to the adress of the list
 * @param[in] i position
 *
 * @pre i < listSize
 *
 * @return -1 if error, 0 else
*/
int listRemove(List* l, unsigned int i);


/**
 * @date 20/10/2023
 * @brief Return the value at position i
 * 
 * @param[in] l list
 * @param[in] i position of the element to return
 *
 * @pre i < list size
 *
 * @return value 
 **/
int listGet(List* l, unsigned int i);


/**
 * @brief Return if the list is empty
 *
 * @param[in] l 
 */
bool listEmpty(List* l);

/**
 * @date  20/10/2023
 * @brief Return the size of the list (position + 1 of the last element)
 *
 * @param[in] l
 *
 */

unsigned int listSize(List* l);

/**
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

int listCopy(List* list_src, List* list_dest);



/**
 * @date 20/10/2023
 * @brief display the list in the default output
 * 
 * @param[in] l list to display
 *
 */
void displayList(List* l);





/*------------------------------------------------------------------*/
/*                         ITERATEUR                                */
/*------------------------------------------------------------------*/

typedef struct s_listIte ListIte;
typedef struct s_listIte* ptrListIte;

/* l'itérateur commence au début de la liste */
#define FROM_BEGIN 1
/* l'itérateur commence à la fin de la liste */
#define FROM_END 0


/**
 * @date 22/10/2023
 * @brief Crée un itérateur sur la liste passée en entrée qui la parcours
 * depuis le début ( @ref FROM_BEGIN) ou la fin ( @ref FROM_END)
 * 
 * @param[in] l liste à parcourrir
 * @param[in] dir sens de parcours de l'itérateur ( @ref FROM_BEGIN ou @ref FROM_END)
 *
 * @return pointeur vers l'itérateur, NULL si erreur
 **/
ListIte* createListIte(List* l, int dir);



/**
 * @date 22/10/2023
 * @brief Renvoie si il reste des éléments à parcourir dans la liste
 * 
 * @param[in] ite pointeur vers l'itérateur
 *
 * @return true si il reste des éléments, sinon false
 **/
bool listIteHasNext(ListIte* ite);


/**
 * @date 22/10/2023
 * @brief Décale l'itérateur sur le prochain élément et renvoie sa valeur
 * 
 * @param[in] ite pointeur vers l'itérateur
 *
 * @pre doit avoir un prochain élément 
 *
 * @return -1 si erreur, 0 sinon
 */
int listIteNext(ListIte* ite);


/**
 * @date 22/10/2023
 * @brief Renvoie la valeur courrante
 * 
 * @param[in] ite pointeur vers l'itérateur
 * 
 * @pre doit être sur un élément
 * 
 * @return élément courant
 */
int listIteGetValue(ListIte* ite);


/**
 * @date 22/10/2023
 * @brief Supprime l'itérateur et libère la mémoire
 *
 * @param[in] ite pointeur vers l'itérateur
 **/
void deleteListIte(ptrListIte* ite);







/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

/**
 * @brief display in the logger all the information about the list and the elements
 * 
 * @param[in] l list to print in logger
 * @date 20/10/2023
 */
void printListLog(List* l);

#endif

#endif