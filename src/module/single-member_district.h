/**
 * @file single-member.h
 * @author IVANOVA ALina 
 * @date 4/11/2023
 *
 * @brief Fichier d'en-tête pour le module Single-Member(Uninominal)
 *
 * Ce module implémente la méthode de Scrutin pour la structure de données bale.h
 *
 * La méthode de Scrutin est utilisée pour déterminer l'option gagnante à partir des votes. 
 * Cette méthode est particulièrement utile dans les processus de vote et de décision.
 *
 * Ce fichier d'en-tête fournit les fonctions et les structures nécessaires pour calculer les résultats selon la méthode de Scrutin pour les données contenues dans bale.h.
 *
 *
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée, il est 
 * fortement recomandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur
 */


#ifndef __SINGLEMEMBER_DISTRICT_H__
#define __SINGLEMEMBER_DISTRICT_H__
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <bale.h>


/*------------------------------------------------------------------*/
/*                        MODULE SINGLE-MEMBER                       */
/*------------------------------------------------------------------*/



/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on cherche le candidat gagnant à partir d'un tour des votes, qui a le max quantité des votes gagne
 *
 * @param[in] 
 *
 * @return Le nom du candidat gagnant
*/
char theWinnerOneRound(Bale* bale);


/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on cherche le candidat gagnant à partir des deux tours des votes, qui a le max quantité des votes gagne
 *
 * @param[in] duel matrice des duels entre tous le candidats
 *
 * @return Le nom du candidat gagnant
*/
char theWinnerTwoRounds(Bale* bale);

#endif


