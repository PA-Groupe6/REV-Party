/**
 * @file single_member.h
 * @author IVANOVA Alina 
 * @date 4/11/2023
 *
 * @brief Fichier d'en-tête pour le module Single-Member (Uninominal)
 *
 * Ce fichier d'en-tête fournit les fonctions et les structures nécessaires pour calculer les résultats selon la méthode de scrutin pour les données contenues dans bale.h et duel.h.
 *
 * La méthode de scrutin est utilisée pour déterminer l'option gagnante à partir des votes. Elle est particulièrement utile dans les processus de vote et de décision.
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée. Il est fortement recommandé de la vérifier, surtout pour les fonctions ne renvoyant pas de pointeur.
 */

#ifndef __SINGLEMEMBER_H__
#define __SINGLEMEMBER_H__
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include "../structure/bale.h"
#include "../structure/duel.h"


/*------------------------------------------------------------------*/
/*                        MODULE SINGLE_MEMBER                       */
/*------------------------------------------------------------------*/



typedef struct s_winner_single {
    char name[256];
    float score;
}WinnerSingle;

/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on cherche le candidat gagnant à partir d'un tour des votes, qui a le max quantité des votes gagne
 *
 * @param[in] bale matrice des ballots fournis pour chaque candidat
 *
 * @return Le nom du candidat gagnant
*/
GenList* theWinnerOneRound(Bale* bale);


/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on cherche le candidat gagnant à partir des deux tours des votes, qui a le max quantité des votes gagne
 *
 * @param[in] bale matrice des ballots fournis pour chaque candidat
 * @param[in] duel matrice des duels entre tous le candidats
 *
 * @return Le nom du candidat gagnant
*/
GenList* theWinnerTwoRounds(Bale* bale);

#endif