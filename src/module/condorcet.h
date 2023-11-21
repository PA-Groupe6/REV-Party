/**
 * @file condorcet.h
 * @author IVANOVA ALina 
 * @date 31/10/2023
 *
 * @brief Fichier d'en-tête pour le module Condorcet
 *
 * Ce module implémente la méthode de Condorcet pour la structure de données duel.h
 *
 * La méthode de Condorcet est utilisée pour déterminer les préférences collectives d'un ensemble de choix en se basant sur les préférences individuelles des votants. 
 * Cette méthode est particulièrement utile dans les processus de vote et de décision.
 *
 * Ce fichier d'en-tête fournit les fonctions et les structures nécessaires pour calculer les résultats selon la méthode de Condorcet pour les données contenues dans duel.h.
 *
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée, il est 
 * fortement recomandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur
 */

#ifndef __CONDORCET_H__
#define __CONDORCET_H__
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include "../structure/duel.h"


/*------------------------------------------------------------------*/
/*                        MODULE CONDORCET                       */
/*------------------------------------------------------------------*/


/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on utilise la méthode vainqueur de Condorcet pour savoir qu'il existe le candidat gagnant
 * 
 * @param[in] duel matrice des duels entre tous le candidats
 *
 * @return Le nom du candidat gagnant
*/
bool CondorcetWinnerCriterion(Duel* duel);


/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on utilise la méthode condorcet minimax pour trouver le candidat gagnant
 * 
 * @param[in] duel matrice des duels entre tous le candidats
 *
 * @return Le nom du candidat gagnant
*/
char theWinnerMinimax(Duel* duel);


/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on utilise la méthode condorcet avec le rangement des paires par ordre décroissant pour trouver le candidat gagnant
 *
 * @param[in] duel matrice des duels entre tous le candidats
 *
 * @return Le nom du candidat gagnant
*/
char theWinnerRankedPairs(Duel* duel);

/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on utilise la méthode de Schulze pour trouver le candidat gagnant
 *
 * @param[in] duel matrice des duels entre tous le candidats
 *
 * @return Le nom du candidat gagnant
*/
char theWinnerSchulze(Duel* duel);


#endif