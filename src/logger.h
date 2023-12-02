/**
 * @file logger.h
 * @author LAFORGE Mateo
 * @brief Header de l'afficheur
 * 
 * L'afficheur a pour rôle de fournir une interface
 * d'affichage formaté généralisée l'ensemble du projet
 * 
 * @remark La sortie par défaut du logger est stdout
*/
#ifndef __LOGGER__H__
#define __LOGGER__H__

#include "structure/list.h"
#include "structure/genericlist.h"
#include "structure/matrix.h"
#include "structure/bale.h"
#include "structure/duel.h"
#include "structure/graph.h"

#include "module/single_member.h"

/**
 * @date 29/10/2023
 * @brief Initialise le logger en définissant sa sortie sur un chemin
 * @remark en mode DEBUG la sortie sera toujours la valeur par défaut
 * 
 * @param[in] file_name Le chemin du fichier où écrira le logger
 * @note mettre NULL pour laisser la valeur par défaut (stdout)
*/
void init_logger(const char* file_path);

/**
 * @date 27/10/2023
 * @brief Écrit dans la sortie du logger
 * 
 * @param[in] format Format du message à envoyer suivis d'un varargs (même syntaxe qu'un printf)
*/
void printl(const char* format, ...);

/**
 * @date 27/10/2023
 * @brief Affiche un message d'erreur dans le logger
 * @remarks le message d'erreur pourra prendre une forme dépendante
 *  de la sortie actuelle du logger e.g. message en rouge pour stdout
 *  caractères particulier dans un fichier
 * 
 * @param[in] file_name Nom du fichier appelant
 * @param[in] fun_name Nom de la fonction appelante
 * @param[in] format Format du message à envoyer suivis d'un varargs (même syntaxe qu'un printf)
 */
void warnl(const char* file_name, const char* fun_name, const char* format, ...);

/**
 * @date 27/10/2023
 * @brief Exit en affichant un message et la valeur de retour dans le logger
 * 
 * @param[in] file_name Nom du fichier appelant
 * @param[in] fun_name Nom de la fonction appelante
 * @param[in] exit_value Valeur de retour du programme
 * @param[in] format Format du message à envoyer suivis d'un varargs (même syntaxe qu'un printf)
 */
void exitl(const char* file_name, const char* fun_name, int exit_value, char* format, ...);

/**
 * @date 29/10/2023
 * @brief Ferme le logger
 * @remark Ne fais rien si la sortie est stdout
*/
void close_logger();

/*
    =======================================
    === Affichage structures de données ===
    =======================================
*/

/**
 * @date 26/11/2023
 * @brief Afficher la liste dans le logger
 *
 * @param[in] l liste à afficher
 *
 */
void displayListLog(List *l);

/**
 * @date 26/11/2023
 * @brief Afficher la valeur des pointeurs dans le logger
 *
 * @param[in] l liste à logger
 * @pre l != NULL
 *
 */
void displayGenListLog(GenList *l);

/**
 * @date 26/11/2023
 * @brief Afficher la matrice dans le logger
 *
 * @param[in] m Matrice à logger
 * @pre m != NULL
 */
void displayMatrixLog(Matrix *m);

/**
 * @date 26/11/2023
 * @brief Affiche le ballot dans le logger
 *
 * @param[in]  b ballot à logger
 * @pre b != NULL
 */
void displayBaleLog(Bale *b);

/**
 * @date 26/11/2023
 * @brief Afficher la matrice de duels dans la sortie standard stdout
 *
 * @param[in] d matrice de duels à afficher
 * @pre d != NULL
 */
void displayDuelLog(Duel *d);


/**
 * @date 26/11/2023
 * @brief Afficher le graph dans le logger
 *
 * @param[in] d graph à afficher
 */
void displayGraphLog(Graph *g);




/*
    ===========================
    === Affichage Résultats ===
    ===========================
*/

/**
 * @date 26/11/2023
 * @brief Affiche la liste des résultats pour Uninominale à 1 tours
 * 
 * @param[in] l Liste des vainqueurs (WinnerSingle)
 */
void displayListWinnerSingleTwo(GenList *l);

/**
 * @date 26/11/2023
 * @brief Affiche la liste des résultats pour Uninominale à 2 tours
 * 
 * @param[in] l Liste des vainqueurs (WinnerSingleTwo)
 */
void displayListWinnerSingle(GenList *l);


/**
 * @date 1/12/2023
 * @brief Affiche la liste des résultats pour Condoret
 * 
 * @param[in] l Liste des vainqueurs (WinnerCondorcet)
 */
void displayListWinnerCondorcet(GenList *l, char* name_algo);


#endif