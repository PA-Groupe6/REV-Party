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
 * @note haute performance en lecture( O(1) ) mais faible en écriture ( O(n))
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée, il est 
 * fortement recomandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur
 */

#define __CONDORCET_H__
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <duel.h>


/*------------------------------------------------------------------*/
/*                        MODULE CONDORCET                       */
/*------------------------------------------------------------------*/



/**
 * @author IVANOVA ALina 
 * @date 31/10/2023
 * @brief on compare le candidat avec un autre pour trouver s'il a gagné ou non
 *
 * @param[in] duel matrcie des duels entre tous le candidats
 * @param[in] numCandidat1 Le numéro du candidat dont nous comparons les votes avec un autre candidat
 * @param[in] numCandidat2 Le numéro du candidat avec laquelle nous comparons
 * 
 * @return boolean True si le candidat a gagné la duel avec un autre, sinon False
*/
bool hasWonTheDuel(Duel duel, int numCandidat1, int numCandidat2);


/**
 * @author IVANOVA ALina 
 * @date 31/10/2023
 * @brief on calcule combien de fois le candidat a gagné dans tous ses duels
 *
 * @param[in] duel matrcie des duels entre tous le candidats
 * 
 * @return nombre des duels gagné
*/
int nbWins(Duel duel);


/**
 * @author IVANOVA ALina 
 * @date 31/10/2023
 * @brief on trouve si le candidat a gagné dans le majorité des duels ou non
 *
 * @param[in] duel matrcie des duels entre tous le candidats
 * @param[in] numCandidat le numero du candidat pour trouver ses duels
 * 
 * @return boolean True si le candidat a gagné majorité des duel, sinon False
*/
bool hasMostWinsInDuels(Duel duel, int numCandidat);



/**
 * @author IVANOVA ALina 
 * @date 31/10/2023
 * @brief (Pour minimax) on calcule la perte maximale en trouvant la valeur maximale dans la rangée correspondante de la matrice. 
 * La valeur maximale dans une rangée représente le scénario le plus défavorable pour ce candidat par rapport à tous les autres.
 *
 * @param[in] duel matrcie des duels entre tous le candidats
 * @param[in] numCandidat le numero du candidat pour trouver ses duels
 * 
 * @return la valeur maximale d'une perte dqns tous les duels
*/
int maximumLoss(Duel duel, int numCandidat);


/**
 * @author IVANOVA ALina 
 * @date 31/10/2023
 * @brief (Pour rangement des paires) On compare la force de chaque paire de candidats (Schwartz)
 *
 * @param[in] duel matrcie des duels entre tous le candidats
 * @param[in] numCandidat le numero du candidat pour trouver ses duels
 * 
 * @return une liste de ces paires avec les marges (différence de préférence entre deux candidats dans une paire)
*/
List listWinningPairs(Duel duel);





