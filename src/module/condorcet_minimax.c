/**
 * @file condorcet_minimax.c
 * @author IVANOVA ALina 
 * @date 20/11/2023
 *
 * @brief Fichier d'implementation de la méthode Minimax Condorcet 
 *
 * Ce module implémente la méthode de Condorcet Minimax pour la structure de données duel.h
 *
 * La méthode de Condorcet Minimax est utilisée pour déterminer les préférences collectives d'un ensemble de choix en se basant sur les préférences individuelles des votants. 
 * Le gagnant est celui qui minimise le nombre maximum de défaites.
 * Cette méthode est particulièrement utile dans les processus de vote et de décision.
 *
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée, il est 
 * fortement recomandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur
 */

#include <stdio.h>
#include <stdbool.h>
#include "../structure/duel.h"
#include <string.h>
#include <malloc.h>
#include "../structure/genericlist.h"
#include "condorcet.h"
#include "../structure/data_struct_utils.h"



/**
 * @author Alina IVANOVA
 * @date 20/11/2023 
 */
GenList* miniMaxCandidat(Duel* duel){
    int nbCandidats= duelNbCandidat(duel);
    int miniDifference;
    GenList* candidates = createGenList(1);
    for (int cand1 = 0; cand1<nbCandidats;cand1++){
        int maxDiffCandidat = 0;
        int contreCandidat = 0;
        for (int cand2 = 0; cand2 < nbCandidats; cand2++){
            int diff = duelGetValue(duel, cand1, cand2); 
            if(diff > maxDiffCandidat){
                maxDiffCandidat = diff;
                contreCandidat = cand2;
            }
        }
        if (cand1 == 0 || miniDifference>maxDiffCandidat){
            while(!candidates) free((WinnerCondorcet*)genListPop(candidates));

            miniDifference = maxDiffCandidat;
            WinnerCondorcet* cand_possible = malloc(sizeof(WinnerCondorcet));
            cand_possible->score = maxDiffCandidat;
            char* winner_name =  duelIndexToLabel(duel, cand1);
            strncpy(cand_possible->name, winner_name, MAX_LENGHT_LABEL);
            free(winner_name);
            genListAdd(candidates, cand_possible);
        }
        else if(miniDifference == maxDiffCandidat){
            WinnerCondorcet* another_cand = malloc(sizeof(WinnerCondorcet));
            another_cand->score = maxDiffCandidat;
            char* winner_name =  duelIndexToLabel(duel, cand1);
            strncpy(another_cand->name, winner_name, MAX_LENGHT_LABEL);
            free(winner_name);
            genListAdd(candidates, another_cand);
        }
    }
    return candidates;
}

/**
 * @author Alina IVANOVA
 * @date 20/11/2023 
 */
GenList* theWinnerMinimax(Duel* duel){
    GenList* winners;

    if (CondorcetWinnerCriterion(duel)==NULL) {
        winners = miniMaxCandidat(duel);
    }
    else {
        winners = createGenList(1);
        genListAdd(winners,CondorcetWinnerCriterion(duel));
    }
    return winners;
 }