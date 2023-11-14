
/**
 * @file genericlist.h
 * @author VALLAT Ugo
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
 * @remark En cas d'erreur, toutes les fonctions de liste générique exit le progamme avec un
 * message d'erreur
 */

#ifndef __GENLIST_H__
#define __GENLIST_H__
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

/*------------------------------------------------------------------*/
/*                     STRUCTURE LIST GENERIC                       */
/*------------------------------------------------------------------*/

/* Définition opaque de la structure list */
typedef struct s_gen_list GenList;
typedef GenList *ptrGenList;

/**
 * @date  5/11/2023
 * @brief Crée une liste vide
 *
 * @param[in] size Espace mémoire initial (en nombre d'éléments)
 *
 * @return pointeur vers la liste
 */
GenList *createGenList(unsigned int size);

/**
 * @date  5/11/2023
 * @brief Supprime la liste mais ne supprime pas la donées pointée
 * @pre l != NULL
 * @pre *l != NULL
 *
 * @param[in] l liste à supprimer
 */
void deleteGenList(ptrGenList *l);

/**
 * @date  5/11/2023
 * @brief Ajoute l'élément à la fin de la liste
 *
 * @param[in] l Pointeur vers la liste
 * @param[in] v Valeur à ajouter
 * @pre l != NULL
 */
void genListAdd(GenList *l, void *v);

/**
 * @date  5/11/2023
 * @brief Insert une valeur à la position i
 *
 * @param[in] l Pointeur vers la liste
 * @param[in] v Valeur à ajouter
 * @param[in] i position
 * @pre l != NULL
 *
 * @pre i <= listSize
 */
void genListInsert(GenList *l, void *v, unsigned int i);

/**
 * @date 5/11/2023
 * @brief Supprime le dernier élément de la liste
 *
 * @param[in] l list
 * @pre l != NULL
 *
 * @pre taille liste > 0
 * @return Valeur avant supression
 **/
void* genListPop(GenList *l);

/**
 * @date  5/11/2023
 * @brief Supprime l'élément à la position i
 *
 * @param[in] l Pointeur vers la liste
 * @param[in] i position
 * @pre l != NULL
 *
 * @pre i < listSize
 * @return Valeur avant supression
 */
void* genListRemove(GenList *l, unsigned int i);

/**
 * @brief Lire la valeur à la position i
 *
 * @param[in] l Pointeur vers la liste
 * @param[in] i Position de l'élément
 *
 * @pre l != NULL
 * @pre i < list size
 *
 * @return Valeur lue
 **/
void *genListGet(GenList *l, unsigned int i);

/**
 * @author VALLAT Ugo
 * @date 5/11/2023
 * @brief Change la valeur à la position i par une nouvelle valeur
 *
 * @param[in] l Pointeur vers la liste
 * @param[in] v Nouvelle valeur
 * @param[in] i Position
 * @pre l != NULL
 */
void genListSet(GenList *l, void *v, unsigned int i);

/**
 * @date 5/11/2023
 *
 * @brief Renvoie si la liste est vide
 *
 * @param[in] l Pointeur vers la liste
 * @pre l != NULL
 *
 * @return true si vide, false sinon
 */
bool genListEmpty(GenList *l);

/**
 * @date  5/11/2023
 * @brief Renvoie la taille de la liste (position + 1 du dernier élément)
 *
 * @param[in] l Pointeur vers la liste
 * @pre l != NULL
 *
 * @return taille de la liste
 *
 */

unsigned int genListSize(GenList *l);

/**
 * @date 30/10/2023
 * @brief Copie la liste en entrée
 *
 * @param[in] l Pointeur de la liste à copier
 * @pre l != NULL
 *
 * @return  Pointeur vers la copie
 *
 */
GenList *genListCopy(GenList *l);

/**
 * @date 5/11/2023
 * @brief Afficher la valeur des pointeurs dans la sortie par standard
 *
 * @param[in] l liste à afficher
 * @pre l != NULL
 *
 */
void displayGenList(GenList *l);

/*------------------------------------------------------------------*/
/*                         ITERATEUR                                */
/*------------------------------------------------------------------*/

typedef struct s_gen_list_ite GenListIte;
typedef struct s_gen_list_ite *ptrGenListIte;

/* l'itérateur commence au début de la liste */
#define FROM_BEGIN 1
/* l'itérateur commence à la fin de la liste */
#define FROM_END 0

/**
 * @date 5/11/2023
 * @brief Crée un itérateur sur la liste passée en entrée qui la parcours
 * depuis le début ( @ref FROM_BEGIN) ou la fin ( @ref FROM_END)
 *
 * @param[in] l liste à parcourrir
 * @param[in] dir sens de parcours de l'itérateur ( @ref FROM_BEGIN ou @ref FROM_END)
 * @pre l != NULL
 *
 * @return pointeur vers l'itérateur
 * @note Une copie de la liste est faite au moment de la création, toute modification
 * de la liste ne sera pas visible par l'itérateur
 **/
GenListIte *createGenListIte(GenList *l, int dir);

/**
 * @date 5/11/2023
 * @brief Renvoie si il reste des éléments à parcourir dans la liste
 *
 * @param[in] ite pointeur vers l'itérateur
 * @pre ite != NULL
 *
 * @return true si il reste des éléments, sinon false
 **/
bool genListIteHasNext(GenListIte *ite);

/**
 * @date 5/11/2023
 * @brief Décale l'itérateur sur le prochain élément et renvoie sa valeur
 *
 * @param[in] ite pointeur vers l'itérateur
 * @pre ite != NULL
 * @pre genListIteHasNext(ite) == true
 */
void genListIteNext(GenListIte *ite);

/**
 * @date 5/11/2023
 * @brief Renvoie la valeur courrante
 *
 * @param[in] ite pointeur vers l'itérateur
 * @pre ite != NULL
 *
 * @pre doit être sur un élément
 *
 * @return élément courant
 */
void *genListIteGetValue(GenListIte *ite);

/**
 * @date 5/11/2023
 * @brief Supprime l'itérateur et libère la mémoire
 *
 * @param[in] ite pointeur vers l'itérateur
 * @pre ite != NULL
 * @pre *ite != NULL
 **/
void deleteGenListIte(ptrGenListIte *ite);

/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

/**
 * @date 5/11/2023
 * @brief display in the logger all the information about the list and the elements
 *
 * @param[in] l list to print in logger
 * @pre l != NULL
 */
void printGenListLog(GenList *l);

#endif

#endif