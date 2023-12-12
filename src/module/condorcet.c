#include <string.h>
#include <malloc.h>
#include "condorcet.h"
#include "../structure/graph.h"
#include "../structure/data_struct_utils.h"

/**************
*   MINIMAX   *
***************/

/**
 * @author Alina IVANOVA
 * @date 20/11/2023 
 */
WinnerCondorcet* CondorcetWinnerCriterion(Duel* duel) {
    int nbCandidats= duelNbCandidat(duel);
    int maxWins = 0;
    int winner = -1;
    for (int cand1 = 0; cand1<nbCandidats;cand1++){
        int winsCandidate = 0;
        for (int cand2 = 0; cand2 < nbCandidats; cand2++){
            int won = duelGetValue(duel, cand1, cand2); 
            int lost = duelGetValue(duel, cand2, cand1); 
            if(won > lost){
                winsCandidate++;
            }
        }
        if (maxWins<winsCandidate){
            maxWins = winsCandidate;
            winner = cand1;
        }
        else if (maxWins == winsCandidate) {
            winner = -1;
        }
    }
    if (winner == -1) return NULL;
    WinnerCondorcet* vainqueur = malloc(sizeof(WinnerCondorcet));
    char* winner_name =  duelIndexToLabel(duel, winner);
    strncpy(vainqueur->name, winner_name, MAX_LENGHT_LABEL);
    free(winner_name);
    vainqueur->score = maxWins;
    return vainqueur;
}

/**
 * @author Alina IVANOVA
 * @date 20/11/2023 
 */
GenList* miniMaxCandidat(Duel* duel) {
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
GenList* theWinnerMinimax(Duel* duel) {
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

/*******************
*   RANKED PAIRS   *
*******************/

GenList* theWinnerRankedPairs(Duel* duel){
    // en attente de l'implémentation de la fonction graphToSortedList
    // TODO
    printf("NON IMPLEMENTEE!\n");
    return createGenList(10);
}

/**************
*   SCHULZE   *
**************/

 GenList* theWinnerSchulze(Duel* duel) {
    // TODO
    printf("NON IMPLEMENTEE!\n");
    return createGenList(10);
}
