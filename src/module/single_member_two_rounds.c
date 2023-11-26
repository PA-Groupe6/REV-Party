/**
 * @author IVANOVA ALina 
 * @date 4/11/2023
 *
 * @brief Fichier d'implémentation de la méthode Single Member (Uninominal) à deux tours
 *
 * Ce module implémente la méthode de Scrutin Uninominal à deux tours pour la structure de données bale.h.
 *
 * La méthode de Scrutin est utilisée pour déterminer l'option gagnante à partir des votes. 
 * Cette méthode est particulièrement utile dans les processus de vote et de décision.
 *
 * Les électeurs votent pour un seul candidat. Celui qui obtient le plus grand nombre de voix, même sans obtenir la majorité absolue, remporte l'élection.
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée. Il est fortement recommandé de la vérifier, surtout pour les fonctions ne renvoyant pas de pointeur.
 */

#include "single_member.h"
#include "../structure/bale.h"
#include "../structure/duel.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <malloc.h>
#include "../structure/list.h"



int* voteCountFirstTour(Bale* bale){
    int nb_votes = baleNbVoter(bale);
    int nb_candidates = baleNbCandidat(bale);
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

int* twoMaxVotesCandidats(int* votes){
    int* twoWinningCandidates = malloc(sizeof(int)*2);
    memset(twoWinningCandidates, 0, sizeof(int)*2);
    int max1 = votes[0];
    int max2 = votes[0];
    int firstWinningCandidate = 0;
    int secondWinningCandidate = 0;
    for (int i = 1; i < sizeof(votes); i++ ){
        if (votes[i] > max2) {
            if (votes[i]>max1){
                max1 = votes[i];
                firstWinningCandidate = i;
            }else{
                max2 = votes[i];
                secondWinningCandidate = i;
            }
        }
    }
    twoWinningCandidates[0] = firstWinningCandidate;
    twoWinningCandidates[1] = secondWinningCandidate;
    return twoWinningCandidates;
}

// int voteCountSecondTour(Duel* duel,int* twoCandidates){
//     int* twoCandidatesVotes = malloc(sizeof(int)*2);
//     int firstVotes = duelGetValue(duel, twoCandidates[0], twoCandidates[1]);
//     int secondVotes = duelGetValue(duel, twoCandidates[1], twoCandidates[0]);
//     if(firstVotes>secondVotes){
//         return 0;
//     }
//     return 1;
// }


// int voteCountSecondTour(Bale *bale,List* Candidates){
//     List *score = createList(listSize(Candidates));
//     int nb_votant = baleNbVoter(bale);
//     List *vote = createList(listSize(Candidates));
//     for(int i = 0; i<nb_votant;i++){
//         for(int j = 0; j<listSize(Candidates);j++){
//             listSet(vote,baleGetValue(bale,i , listGet(Candidates,j)),i);
//         }

        
//     }
// }



GenList* theWinnerTwoRounds(Bale* bale){
    GenList *list = createGenList(1);
    WinnerSingle winner;
    unsigned nb_candidat = baleNbCandidat(bale);


    int* firstTourResults = voteCountFirstTour(bale);
    int* firstTourWinners = twoMaxVotesCandidats(firstTourResults);
    int score;


    if( (score =((float)firstTourResults[firstTourWinners[0]]/nb_candidat) * 100) > 50){ //on verify si le gagnant du premier tour a plus de 50% des voies

        strncpy(winner.name, baleColumnToLabel(bale, firstTourWinners[0]), MAX_LENGHT_LABEL);
        /* calcul du score */
        winner.score = score;
        genListAdd(list,(void*)&winner);
        return list;
    }

    //debut du second tour

    int nbOfWinner = voteCountSecondTour(bale,firstTourWinners);
    char* ultimateWinner = baleColumnToLabel(bale, nbOfWinner);

    return list;
}