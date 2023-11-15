/**
 * @file single_member.h
 * @author IVANOVA ALina 
 * @date 4/11/2023
 *
 * @brief Fichier d'implementation de la méthode Single Member(Uninominal) à un tour
 *
 * Ce module implémente la méthode de scrutin uninominal à un tour pour la structure de données bale.h.
 *
 * La méthode de scrutin est utilisée pour déterminer l'option gagnante à partir des votes. 
 * Cette méthode est particulièrement utile dans les processus de vote et de décision.
 *
 * Les électeurs votent pour un seul candidat. Celui qui obtient le plus grand nombre de voix, même sans obtenir la majorité absolue, remporte l'élection.
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée. Il est fortement recommandé de la vérifier, surtout pour les fonctions ne renvoyant pas de pointeur.
 */


#include "single_member.h"
#include "../structure/bale.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <malloc.h>




int* voteCount(Bale* bale){
    int nb_votes = baleShape(bale)[0];
    int nb_candidates = baleShape(bale)[1];
    int* votesComplete = malloc(sizeof(int)*nb_candidates);
    memset(votesComplete, 0, sizeof(int)*nb_candidates);
    for (int i = 0; i<nb_votes;i++){
        //on recoit le liste avec le(les) candidat(s) qui a recu le val max a partir d'electeur i
        GenList* winner = baleMax(bale, i, -1);
        if (genListSize(winner)==1){//si on a qu'un seul candidat avec le note max, on prend en compte le vote
            int cand = ((int*)genListGet(winner, 0))[2];//num de candidat avec le val max
            votesComplete[cand] += 1;//on ajoute le vote
        }
    }
    return votesComplete;
}

int maxVotesCandidat(int* votes, int nb_votes){
    int max = votes[0];
    int winningCandidate = 0;
    for (int i = 1; i < nb_votes; i++ ){
        if (votes[i] > max) {
            max = votes[i];
            winningCandidate = i;
        }
    }
    return winningCandidate;
}


char* theWinnerOneRound(Bale* bale){
    int* summaryOfVotes = voteCount(bale);
    int winningCandidate = maxVotesCandidat(summaryOfVotes, baleShape(bale)[1]);
    char* ultimateWinner = baleColumnToLabel(bale, winningCandidate);
    return ultimateWinner ;
}