/**
 * @file list.h
 * @author VALLAT Ugo
 *
 * @brief Cette librairie implémente une liste pseudo statique d'entiers
 *
 * Implémente la liste sous forme d'un tableau statique et alloue
 * de la mémoire dynamiquement lorsque qu'il est plein
 *
 * @note haute performance en lecture( O(1) ) mais faible en écriture ( O(n))
 *
 * @remark En cas d'erreur, toutes les fonctions de liste exit le progamme avec un
 * message d'erreur
 */

#ifndef __LIST_H__
#define __LIST_H__
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

/*------------------------------------------------------------------*/
/*                        STRUCTURE LIST                            */
/*------------------------------------------------------------------*/

/* Définition opaque de la structure list */
typedef struct s_list List;
typedef List *ptrList;

/**
 * @date  5/11/2023
 * @brief Crée une liste vide
 *
 * @param[in] memory_size Espace mémoire initial (en nombre d'éléments)
 *
 * @return pointeur vers la liste
 * @note Alloue la mémoire mais n'est pas initialisée (taille liste = 0)
 */
List *createList(unsigned memory_size);

/**
 * @date  5/11/2023
 * @brief Supprime la liste et libère la mémoire
 *
 * @param[in] l liste à supprimer
 * @pre l != NULL
 * @pre *l != NULL
 */
void deleteList(ptrList *l);

/**
 * @date  5/11/2023
 * @brief Ajoute l'élément à la fin de la liste
 *
 * @param[in] l Pointeur vers la liste
 * @param[in] v Valeur à ajouter
 * @pre l != NULL
 */
void listAdd(List *l, int v);

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
void listInsert(List *l, int v, unsigned i);

/**
 * @date 5/11/2023
 * @brief Supprime le dernier élément de la liste
 *
 * @param[in] l list
 * @pre l != NULL
 *
 * @pre taille liste > 0
 * @return valeur avant suppression
 **/
int listPop(List *l);

/**
 * @date  5/11/2023
 * @brief Supprime l'élément à la position i
 *
 * @param[in] l Pointeur vers la liste
 * @param[in] i position
 * @pre l != NULL
 *
 * @pre i < listSize
 * @return valeur avant suppression
 */
int listRemove(List *l, unsigned i);

/**
 * @date 5/11/2023
 * @brief Lire la valeur à la position i
 *
 * @param[in] l Pointeur vers la liste
 * @param[in] i Position de l'élément
 * @pre l != NULL
 *
 * @pre i < list size
 *
 * @return Valeur lue
 **/
int listGet(List *l, unsigned i);

/**
 * @author VALLAT Ugo
 * @date 31/10/2023
 * @brief Change la valeur à la position i par une nouvelle valeur
 *
 * @param[in] l Pointeur vers la liste
 * @param[in] v Nouvelle valeur
 * @param[in] i Position
 * @pre l != NULL
 */
void listSet(List *l, int v, unsigned i);

/**
 * @date 5/11/2023
 *
 * @brief Renvoie si la liste est vide
 *
 * @param[in] l Pointeur vers la liste
 * @pre l != NULL
 * @return true si vide, false sinon
 */
bool listEmpty(List *l);

/**
 * @date  5/11/2023
 * @brief Renvoie la taille de la liste (position + 1 du dernier élément)
 *
 * @param[in] l Pointeur vers la liste
 * @return taille de la liste
 *
 */

unsigned listSize(List *l);

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

List *listCopy(List *l);


/**
 * @brief Remet la liste à 0
 * 
 * @param l Liste à vider
 */
void listClear(List *l);

#endif