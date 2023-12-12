/**
 * 
 * @file majority_judgment.c
 * @author IVANOVA ALina 
 * @date 24/11/2023
 *
 * @brief Fichier d'implementation de la méthode Jugement majoritaire.  
 *
 * Ce module implémente la méthode de Jugement majoritaire pour la structure de données bale.h
 *
 * Изменить!!!!!!!!!!!!!!!!!!!!!1
 *
 * Ce fichier d'en-tête fournit les fonctions et les structures nécessaires pour calculer les résultats selon la méthode de Condorcet pour les données contenues dans duel.h.
 *
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée, il est 
 * fortement recomandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "../structure/bale.h"
#include <string.h>
#include <malloc.h>
#include "../structure/genericlist.h"
#include "majority_judgment.h"

/**
 * @author Alina IVANOVA
 * @date 24/11/2023 
 */
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}


/**
 * @author Alina IVANOVA
 * @date 24/11/2023 
 */
int* sortingVotesCandidate(Bale* bale, int candidate){
    int nbVotes = baleNbVoter(bale);
    int* votes = malloc(sizeof(int)*nbVotes);
    for (int voter = 0; voter<nbVotes; voter++)votes[voter] = baleGetValue(bale, voter, candidate);
    qsort(votes, nbVotes, sizeof(int), compare);
    return votes;
}


/**
 * @author Alina IVANOVA
 * @date 24/11/2023 
 */
int medianeCandidate(Bale* bale, int candidate){
    int nbVotes = baleNbVoter(bale);;
    int* votes = sortingVotesCandidate(bale,candidate);
    if(nbVotes%2!=0) {
        int a = votes[(nbVotes/2)];
        free(votes);
        return a;
    }
    else{
        int a = (votes[nbVotes/2]+ votes[nbVotes/2-1])/2;
        free(votes);
        return a;
    }
}


/**
 * @author Alina IVANOVA
 * @date 24/11/2023 
 */
GenList* severalWinners(Bale* bale, int nbWinningCandidates, int maxMediane, int* indexesMaxMed){
    GenList* candidates;
    Candidate* candidatePerc = malloc(sizeof(Candidate)); 
    int nbVotes = baleNbVoter(bale);
    float percentageInf, percentageSup;
    //creating each candidate struct with percentages
    for (int cand = 0; cand<nbWinningCandidates;cand++){
        int* sortedVotes = sortingVotesCandidate(bale, indexesMaxMed[cand]);
        int electeursInf = 0;
        int currentVote = sortedVotes[electeursInf];
        while(currentVote!=maxMediane){
            electeursInf++;
            currentVote = sortedVotes[electeursInf];
        }
        percentageInf = (float) electeursInf/nbVotes;
        int electeursMed = electeursInf;
        while(currentVote == maxMediane){//combien on a les valeur egles a mediane
            electeursMed++;
            currentVote = sortedVotes[electeursInf];
        }
        electeursMed -=electeursInf; 
        percentageSup = (float) (nbVotes-electeursMed-electeursInf)/nbVotes;//percentage of votes Superieurs
        candidatePerc->candIndex = indexesMaxMed[cand];
        candidatePerc->percentInf = percentageInf;
        candidatePerc->percentSup = percentageSup;
        candidatePerc->mediane = maxMediane;
        genListAdd(candidates, candidatePerc);
    }
    return candidates;
}


/**
 * @author Alina IVANOVA
 * @date 24/11/2023 
 */
GenList* bestCand(GenList* winners, int nbVotes){
    int nbCand = genListSize(winners);
    float maxPerc = 0, percSup;
    int* IndLessEvilCand = malloc(sizeof(int)*nbCand);
    Candidate* currentCandidate = malloc(sizeof(Candidate));
    GenList* bestCandidates;
    for (int i = 0; i<nbCand; i++){
        currentCandidate = genListGet(winners, i);
        percSup = currentCandidate->percentSup;
        if(maxPerc>percSup){
            while(!bestCandidates) free((Candidate*)genListPop(bestCandidates));
            maxPerc = percSup;
            genListAdd(bestCandidates, currentCandidate);
        }
        else if(maxPerc==percSup){
            genListAdd(bestCandidates, currentCandidate);
        }
    }
    return bestCandidates;
}


/**
 * @author Alina IVANOVA
 * @date 24/11/2023 
 */
GenList* leastHarmCand(GenList* winners, int nbVotes){
    int nbCand = genListSize(winners);
    float minPerc = 1, percInf;
    int* IndLessEvilCand = malloc(sizeof(int)*nbCand);
    Candidate* currentCandidate = malloc(sizeof(Candidate));
    GenList* lessEvilCandidates;
    for (int i = 0; i<nbCand; i++){
        currentCandidate = genListGet(winners, i);
        percInf = currentCandidate->percentInf;
        if(minPerc>percInf){
            while(!lessEvilCandidates) free((Candidate*)genListPop(lessEvilCandidates));
            minPerc = percInf;
            genListAdd(lessEvilCandidates, currentCandidate);
        }
        else if(minPerc==percInf){
            genListAdd(lessEvilCandidates, currentCandidate);
        }
    }
    return lessEvilCandidates;
}


/**
 * @author Alina IVANOVA
 * @date 24/11/2023 
 */
GenList* winningCandidates(Bale* bale){
    int nbCand = baleNbCandidat(bale);
    int* medianes = malloc(sizeof(int)*nbCand);
    int maxMediane = 0, k = 1;
    int* indexMaxMed = malloc(sizeof(int)*nbCand);
    for(int cand = 0; cand< nbCand; cand++){
        medianes[cand] = medianeCandidate(bale, cand);
        if (medianes[cand]>maxMediane){
            maxMediane = medianes[cand];
            k = 1;
            indexMaxMed[0] = cand;
        }
        else if (medianes[cand] == maxMediane){
            k++;
            indexMaxMed[k-1] = cand;
        }
    }
    GenList* winner_s;
    if (k>1){
        winner_s = severalWinners(bale, k, maxMediane,indexMaxMed);
        GenList* leastEvilCand = leastHarmCand(winner_s, baleNbVoter(bale));
        GenList* bestOutOfBest;
        if(genListSize(leastEvilCand)>1) 
            bestOutOfBest = bestCand(leastEvilCand,baleNbVoter(bale));
        }
    else {
        int winner = indexMaxMed[0];
        free(indexMaxMed);
    }
    return winner_s;
}




/**
 * @author Alina IVANOVA
 * @date 24/11/2023 
 */
GenList* theWinnerMajorityJudgment(Bale* bale){
    GenList* winners = winningCandidates(bale);
    return winners;
}