/*-----------------------------------------------------------------*/
/**
    Projet REV party - Structure Lkliste chaînée générique
    \file sdgenlkLklist.h
    \author : Ugo VALLAT
    \date creation date : 22/10/2023
 
 */
/*-----------------------------------------------------------------*/

/**
 * @brief Cette librairie implémente une Lkliste doublement chaînée générique
 *
 * Implémente la Lkliste sous forme de noeuds et utilise une sentinelle
 * La list ne contient que des void * (adresses des valeurs)
 *
 * @note haute performance en écriture( O(1) ) mais faible en lecture ( O(n))
 *
 */


#ifndef __GEN_LK_Lklist__H__
#define __GEN_LK_Lklist__H__

#include <stdio.h>
#include <stdbool.h>


/*------------------------------------------------------------------*/
/*                STRUCTURE Lklist CHAINEE GENERIQUE                  */
/*------------------------------------------------------------------*/


/* Définition opaque de la structure linked Lklist */
typedef struct s_lk_Lklist Lklist;
typedef Lklist * ptrLklist;



/** \brief Crée une Lkliste vide
    \param[in] size Espace mémoire initial (en nombre d'éléments)
    \return pointeur vers la Lkliste, NULL si erreur
    \date  22/10/2023
*/
ptrLklist createLkLklist();


/** \brief Supprime la Lkliste et libère la mémoire
    \param[in] l Lklist à supprimer
    \date  22/10/2023
*/
void deleteLkLklist(ptrLklist l);


/** \brief Ajoute d'un élément à la fin de la liste
    \param[in] l Lklist
    \param[in] v adresse de l'élément à ajouter
    \return adresse de la liste, null si erreur
    \date  22/10/2023
*/
ptrLklist LklistAdd(ptrLklist l, void * v);


/** \brief Insertion d'un élément dans la liste à la position i
    \param[in] l Lklist
    \param[in] v adresse de l'élément à ajouter
    \param[in] i position
    \return adresse de la liste, null si erreur
    \pre: 0 <= i <= LklistSize
    \date  22/10/2023
*/
ptrLklist LklistInsert(ptrLklist l, void * v, int i);


/**
 * @brief Supprime le dernier élément de la Lkliste
 * 
 * @param[in] l Lklist
 * @return -1 si erreur, 0 sinon
 * @pre taille Lkliste > 0
 * @date 22/10/2023
 */
int LklistPop(ptrLklist l);


/** \brief Supprime la valeur à la position i
    \param[in] l Lklist
    \param[in] i position
    \return -1 si erreur, 0 sinon
    \pre: 0 <= i < LklistSize
    \date  22/10/2023
*/
int LklistRemove(ptrLklist l, int i);


/**
 * \brief Renvoie true si la liste est vide
 * \param[in] l Lklist
 * \date  22/10/2023
 */
bool LklistEmpty(ptrLklist l);

/**
 * \brief Renvoie la taille de la liste (nombre d'éléments)
    \param[in] l Lklist
    \date  22/10/2023
 */

unsigned int LklistSize(ptrLklist l);

/**
 * @brief Crée une copie de la liste
 * 
 * @param old_Lklist Lklist à copier
 * @pre: size >= old Lklist size
 * @return adresse de la nouvelle Lklist, null si erreur
 * @date 22/10/2023
 */

ptrLklist LklistCopy(ptrLklist old_Lklist);


/**
 * @brief Renvoie la valeur à la position i dans la liste
 * 
 * @param l Lklist
 * @param i position de l'élément
 * @pre: 0 <= i < Lklist size
 * @return adresse de l'élément, null si erreur
 * @date 22/10/2023
 */
void * LklistGet(ptrLklist l, int i);


/**
 * @brief affiche la liste dans stdout
 * 
 * @param l Lklist à afficher
 * @date 22/10/2023
 */
void displayLklist(ptrLklist l, char* (*funPrint)(void *));



/*------------------------------------------------------------------*/
/*                         ITERATEUR                                */
/*------------------------------------------------------------------*/

typedef struct s_Lklist_ite LklistIte;
typedef struct s_Lklist_ite* ptrLklistIte;

/* l'itérateur commence au début de la Lkliste */
#define FROM_BEGIN 1
/* l'itérateur commence à la fin de la Lkliste */
#define FROM_END 0

/**
 * @brief Crée un itérateur sur la Lkliste passée en entrée
 * 
 * @param[in] l Lkliste à parcourrir
 * @param[in] dir sens de parcours de l'itérateur (FROM_BEGIN ou FROM_END)
 * @return pointeur vers l'itérateur, NULL si erreur
 * @date 22/10/2023
 */
ptrLklistIte createLklistIte(ptrLklist l, int dir);


/**
 * @brief Initialise l'itérateur selon ses paramêtres
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return pointeur vers l'itérateur, NULL si erreur
 * @date 22/10/2023
 */
ptrLklistIte initLklistIte(ptrLklistIte ite);


/**
 * @brief Renvoie si il reste des éléments à parcourir dans la Lkliste
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return true si il reste des éléments, sinon false
 * @date 22/10/2023
 */
bool LklistIteHasNext(ptrLklistIte ite);

/**
 * @brief Décale l'itérateur sur le prochain élément et renvoie sa valeur
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return -1 si erreur, 0 sinon
 * @pre doit avoir un prochain élément 
 * @date 22/10/2023
 */
int LklistIteNext(ptrLklistIte ite);


/**
 * @brief Renvoie la valeur courrante
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @return élément courant
 * @pre doit être sur un élément
 * @date 22/10/2023
 */
void * LklistIteValue(ptrLklistIte ite);


/**
 * @brief Supprime l'itérateur et libère la mémoire
 * 
 * @param[in] ite pointeur vers l'itérateur
 * @date 22/10/2023
 */
void deleteLklistIte(ptrLklistIte ite);


/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

/**
 * @brief affiche toutes les information sur la liste dans le logger
 * 
 * @param l Lklist à afficher
 * @date 22/10/2023
 */
void printLklistLog(ptrLklist l);

#endif


#endif
