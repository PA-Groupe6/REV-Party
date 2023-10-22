/*-----------------------------------------------------------------*/
/**
    Projet REV party - Structur list header
    \file sdlist.h
    \author : Ugo VALLAT
    \date creation date : 20/10/2023
 
 */
/*-----------------------------------------------------------------*/

/**
 * @brief Cette librairie implémente une liste pseudo statique de type TYPE
 *
 * Implémente la liste sous forme d'un tableau statique et alloue
 * de la mémoire dynamiquement lorsque qu'il est plein
 *
 * @note haute performance en lecture( O(1) ) mais faible en écriture ( O(n))
 *
 */


#ifndef __LIST__H__
#define __LIST__H__
#include <stdio.h>
#include <stdbool.h>


/*------------------------------------------------------------------*/
/*                         STRUCTURE                                */
/*------------------------------------------------------------------*/


/* Définition opaque de la structure list */
typedef struct s_list List;
typedef List * ptrList;

#define TYPE double


/** \brief Crée une liste vide
    \param[in] size Espace mémoire initial (en nombre d'éléments)
    \return pointeur vers la liste, NULL si erreur
    \date  20/10/2023
*/
ptrList createList(int size);


/** \brief Supprime la liste et libère la mémoire
    \param[in] l liste à supprimer
    \date  20/10/2023
*/
void deleteList(ptrList l);


/** \brief Add value v to the end of the list
    \param[in] l pointer to the adress of the list
    \param[in] v value to add
    \return new adress of the list, null if error
    \date  20/10/2023
*/
ptrList listAdd(ptrList l, TYPE v);


/** \brief Insert a value in the list to the position require
    \param[in] l pointer to the adress of the list
    \param[in] v value to add
    \param[in] i position
    \return new adress of the list, null if error
    \pre: 0 <= i <= listSize
    \date  20/10/2023
*/
ptrList listInsert(ptrList l, TYPE v, int i);


/**
 * @brief Supprime le dernier élément de la liste
 * 
 * @param[in] l list
 * @return -1 si erreur, 0 sinon
 * @pre taille liste > 0
 * @date 20/10/2023
 */
int listPop(ptrList l);


/** \brief Remove a value in the list to the position require
    \param[in] l pointer to the adress of the list
    \param[in] i position
    \return -1 if error, 0 else
    \pre: 0 <= i < listSize
    \date  20/10/2023
*/
int listRemove(ptrList l, int i);


/**
 * \brief Return if the list is empty
 * \param[in] l 
 
 */
bool listEmpty(ptrList l);

/**
 * \brief Return the size of the list (position + 1 of the last element)
    \param[in] l
    \date  20/10/2023
 */

unsigned int listSize(ptrList l);

/**
 * @brief Create new list and copy all the data from old list into new list
 * 
 * @param oldList list to copy
 * @param new_size size of the new list
 * @pre: size >= old list size
 * @return pointer to the new list, null if error
 * @date 20/10/2023
 */

ptrList listCopy(ptrList old_list, int new_size);


/**
 * @brief Return the value at position i
 * 
 * @param l list
 * @param i position of the element to return
 * @pre: 0 <= i < list size
 * @return value 
 * @date 20/10/2023
 */
TYPE listGet(ptrList l, int i);


/**
 * @brief display the list in the default output
 * 
 * @param l list to display
 * @date 20/10/2023
 */
void displayList(ptrList l);

/*------------------------------------------------------------------*/
/*                         ITERATEUR                                */
/*------------------------------------------------------------------*/

typedef struct s_list_ite ListIte;
typedef struct s_list_ite* ptrListIte;

#define FROM_BEGIN 0
#define FROM_END 1

/**
 * @brief Crée un itérateur sur la liste passée en entrée
 * 
 * @param[in] l liste à parcourrir
 * @param[in] dir sens de parcours de l'itérateur (FROM_BEGIN ou FROM_END)
 * @return pointeur vers l'itérateur
 * @date 22/10/2023
 */
ptrListIte createListIte(ptrList l, int dir);

/**
 * @brief Renvoie si il reste des éléments à parcourir dans la liste
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return true si il reste des éléments, sinon false
 * @date 22/10/2023
 */
bool listIteHasNext(ptrListIte ite);

/**
 * @brief Décale l'itérateur sur le prochain élément et renvoie sa valeur
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return prochain élément
 * @pre doit avoir un prochain élément 
 * @date 22/10/2023
 */
TYPE listIteNext(ptrListIte ite);

/**
 * @brief Supprime l'élément courant
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return -1 si erreur, 0 sinon
 * @pre nécessite appel de next avant chaque appel de remove
 * @date 22/10/2023
 */
TYPE listIteRemove(ptrListIte ite);

/**
 * @brief Supprime l'itérateur et libère la mémoire
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return -1 si erreur, 0 sinon 
 * @date 22/10/2023
 */
int deleteListIte(ptrListIte ite);


/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

/**
 * @brief display in the logger all the information about the list and the elements
 * 
 * @param l list to print in logger
 * @date 20/10/2023
 */
void printListLog(ptrList l);

#endif

#endif
