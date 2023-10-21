/*-----------------------------------------------------------------*/
/**
    Projet REV party - Structur list header
    \file sdlist.h
    \author : Ugo VALLAT
    \date creation date : 20/10/2023
 
 */
/*-----------------------------------------------------------------*/

/**
    \brief This library implement an pseudo-static list using static table.
    It can only store Integer (int).
 */

#ifndef __LIST__H__
#define __LIST__H__
#include <stdio.h>
#include <stdbool.h>

/* Opaque definition of type List */
typedef struct s_list List;
typedef List * ptrList;

#define TYPE double


/** \brief Create an empty list
    \param[in] size allocated space for the list
    \return adress of the new list, -1 if error
    \date  20/10/2023
*/
ptrList createList(int size);


/** \brief DElete the list
    \param[in] l
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
 */

ptrList listCopy(ptrList oldList, int new_size);


/**
 * @brief Return the value at position i
 * 
 * @param l list
 * @param i position of the element to return
 * @pre: 0 <= i < list size
 * @return value 
 */
TYPE listGet(ptrList l, int i);


/**
 * @brief display the list in the default output
 * 
 * @param l list to display
 */
void displayList(ptrList l);

#ifdef DEBUG

/**
 * @brief display in the logger all the information about the list and the elements
 * 
 * @param l list to print in logger
 */
void printListLog(ptrList l);

#endif

#endif
