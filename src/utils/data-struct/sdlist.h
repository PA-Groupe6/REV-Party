/*-----------------------------------------------------------------*/
/**
    Projet REV party - Structur list
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


/** \brief Create an empty list
    \param[in] size allocated space for the list
    \date  20/10/2023
*/
ptrList createList(int size);


/** \brief DElete the list
    \param[in] l
    \date  20/10/2023
*/
void deleteList(ptrList l);


/** \brief Add value v to the end of the list
    \param[in] v value to add
    \date  20/10/2023
*/
ptrList listAdd(ptrList l, int v);


/** \brief Insert a value in the list to the position require
    \param[in] v value to add
    \param[in] i position
    \pre: 0 <= i <= listSize
    \date  20/10/2023
*/
ptrList listInsert(ptrList l, int v, int i);


/** \brief Remove a value in the list to the position require
    \param[in] i position
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

#endif
