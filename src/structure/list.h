/*-----------------------------------------------------------------*/
/**
    Projet REV party - Structur list header
    \file list.h
    \author : Ugo VALLAT
    \date Date de création : 20/10/2023
 
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


#ifndef __LIST__H__
#define __LIST__H__
#include <stdio.h>
#include <stdbool.h>


/*------------------------------------------------------------------*/
/*                        STRUCTURE LIST                            */
/*------------------------------------------------------------------*/


/* Définition opaque de la structure list */
typedef struct s_list List;
typedef List * ptrList;



/** \brief Crée une liste vide
    \param[in] size Espace mémoire initial (en nombre d'éléments)
    \return pointeur vers la liste, NULL si erreur+
    \date  20/10/2023
*/
ptrList createList(unsigned int size);


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
ptrList listAdd(ptrList l, int v);


/** \brief Insert a value in the list to the position require
    \param[in] l pointer to the adress of the list
    \param[in] v value to add
    \param[in] i position
    \return new adress of the list, null if error
    \pre: 0 <= i <= listSize
    \date  20/10/2023
*/
ptrList listInsert(ptrList l, int v, int i);


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
 * @brief Return the value at position i
 * 
 * @param[in] l list
 * @param[in] i position of the element to return
 * @pre: 0 <= i < list size
 * @return value 
 * @date 20/10/2023
 */
int listGet(ptrList l, int i);


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
 * @brief Copie une liste source dans la liste destination de taille égale ou supérieure
 * 
 * @param[in] list_src liste source à copier
 * @param[in] list_dest liste destination
 * @return  -1 si erreur, 0 sinon
 * @pre size_src <= size_dest
 * @date 25/10/2023
 */

int listCopy(ptrList list_src, ptrList list_dest);



/**
 * @brief display the list in the default output
 * 
 * @param[in] l list to display
 * @date 20/10/2023
 */
void displayList(ptrList l);

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
 * @brief Crée un itérateur sur la liste passée en entrée
 * 
 * @param[in] l liste à parcourrir
 * @param[in] dir sens de parcours de l'itérateur (FROM_BEGIN ou FROM_END)
 * @return pointeur vers l'itérateur, NULL si erreur
 * @date 22/10/2023
 */
ptrListIte createListIte(ptrList l, int dir);


/**
 * @brief Initialise l'itérateur selon ses paramêtres
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return pointeur vers l'itérateur, NULL si erreur
 * @date 22/10/2023
 */
ptrListIte initListIte(ptrListIte ite);


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
 * @return -1 si erreur, 0 sinon
 * @pre doit avoir un prochain élément 
 * @date 22/10/2023
 */
int listIteNext(ptrListIte ite);


/**
 * @brief Renvoie la valeur courrante
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return élément courant
 * @pre doit être sur un élément
 * @date 22/10/2023
 */
int listIteGetValue(ptrListIte ite);


/**
 * @brief Supprime l'itérateur et libère la mémoire
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @date 22/10/2023
 */
void deleteListIte(ptrListIte ite);


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
void printListLog(ptrList l);

#endif

#endif
