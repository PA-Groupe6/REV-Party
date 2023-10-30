/**
 * @file list.h
 * @author VALLAT Ugo 
 * @date 29/10/2023
 *
 * @brief Cette librairie implémente une liste pseudo statique d'entiers
 *
 * Implémente la liste sous forme d'un tableau statique et alloue
 * de la mémoire dynamiquement lorsque qu'il est plein
 *
 * @note haute performance en lecture( O(1) ) mais faible en écriture ( O(n))
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée, il est 
 * fortement recomandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur
 */


#ifndef __LIST_H__
#define __LIST_H__
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>


/*------------------------------------------------------------------*/
/*                        STRUCTURE LIST                            */
/*------------------------------------------------------------------*/


/* Définition opaque de la structure list */
typedef struct s_list List;
typedef List * ptrList;



/** 
 * @author VALLAT ugo
 * @date  29/10/2023
 * @brief Crée une liste vide
 *
 * @param[in] size Espace mémoire initial (en nombre d'éléments)
 *
 * @return pointeur vers la liste, NULL si erreur
*/
List* createList(unsigned int size);


/** 
 * @author VALLAT ugo
 * @date  29/10/2023
 * @brief Supprime la liste et libère la mémoire
 *
 * @param[in] l liste à supprimer
*/
void deleteList(ptrList* l);


/** 
 * @author VALLAT ugo
 * @date  29/10/2023
 * @brief Ajoute l'élément à la fin de la liste
 *
 * @param[in] l Pointeur vers la liste
 * @param[in] v Valeur à ajouter
 *
 * @return Pointeur de la liste, NULL si erreur
*/
List* listAdd(List* l, int v);


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
List* listInsert(List* l, int v, unsigned int i);


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
int listPop(List* l);


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
int listRemove(List* l, unsigned int i);


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
 * @return Valeur lue, -1 si erreur
 **/
int listGet(List* l, unsigned int i);


/**
 * @author VALLAT ugo
 * @date 29/10/2023
 *
 * @brief Renvoie si la liste est vide
 *
 * @param[in] l Pointeur vers la liste
 * @return true si vide ou l=NULL, false sinon
 */
bool listEmpty(List* l);

/**
 * @author VALLAT ugo
 * @date  29/10/2023
 * @brief Renvoie la taille de la liste (position + 1 du dernier élément)
 *
 * @param[in] l Pointeur vers la liste
 * @return taille de la liste, -1 si erreur
 *
 */

unsigned int listSize(List* l);

/**
 * @author VALLAT ugo
 * @date 30/10/2023
 * @brief Copie la liste en entrée
 * 
 * @param[in] l Pointeur de la liste à copier
 *
 * @return  Pointeur vers la copie, NULL si erreur
 *
 */

List* listCopy(List* l);



/**
 * @author VALLAT ugo
 * @date 29/10/2023
 * @brief Afficher la liste dans la sortie par standard
 * 
 * @param[in] l liste à afficher
 *
 */
void displayList(List* l);





/*------------------------------------------------------------------*/
/*                         ITERATEUR                                */
/*------------------------------------------------------------------*/

typedef struct s_list_ite ListIte;
typedef struct s_list_ite* ptrListIte;

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
 * @note Une copie de la liste est faite au moment de la création, toute modification
 * de la liste ne sera pas visible par l'itérateur
 **/
ListIte* createListIte(List* l, int dir);



/**
 * @author VALLAT ugo
 * @date 22/10/2023
 * @brief Renvoie si il reste des éléments à parcourir dans la liste
 * 
 * @param[in] ite pointeur vers l'itérateur
 *
 * @return true si il reste des éléments, sinon false
 **/
bool listIteHasNext(ListIte* ite);


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
int listIteNext(ListIte* ite);


/**
 * @author VALLAT ugo
 * @date 22/10/2023
 * @brief Renvoie la valeur courrante
 * 
 * @param[in] ite pointeur vers l'itérateur
 * 
 * @pre doit être sur un élément
 * 
 * @return élément courant, -1 si erreur
 */
int listIteGetValue(ListIte* ite);


/**
 * @author VALLAT ugo
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
 * @author VALLAT ugo
 * @date 29/10/2023
 */
void printListLog(List* l);

#endif

#endif