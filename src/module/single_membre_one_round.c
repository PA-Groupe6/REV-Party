/**
 * @author Alina IVANOVA
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
#include "../logger.h"
#include "../structure/list.h"


/**
 * @author Alina IVANOVA
 * @date 21/11/2023 
 */
int* voteCount(Bale* bale){
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

/**
 * @author Alina IVANOVA, Corentin LUDWIG
 * @date 21/11/2023 
 */
List* maxVotesCandidat(int* votes, int nb_candidat){
    int max = 0;
    List *winner = createList(1);
    for (int i = 1; i < nb_candidat; i++ ){
        if (votes[i] > max) {
            max = votes[i];
            listSet(winner,i,0);
            for(int j = 1; j<listSize(winner)-1;i++){
                listRemove(winner,j);
            }
        } else if( votes[i] == max){
            listAdd(winner,i);

        }
    }



    return winner;
}

/**
 * @author Alina IVANOVA
 * @date 21/11/2023 
 */
GenList* theWinnerOneRound(Bale* bale){
    GenList *list = createGenList(1);
    WinnerSingle winner;
    unsigned nb_candidat = baleNbCandidat(bale);

    /* décompte des voies de chaque candidat */
    int* summaryOfVotes = voteCount(bale);

    /* Récupération du nom du gagnant */
    List* winningCandidates = maxVotesCandidat(summaryOfVotes, nb_candidat);

    for (int i = 0; i < listSize(winningCandidates); i++)
    {
        int winningCandidate = listGet(winningCandidates,i);
        strncpy(winner.name, baleColumnToLabel(bale, winningCandidate), MAX_LENGHT_LABEL);
        /* calcul du score */
        winner.score = ((float)summaryOfVotes[winningCandidate]/nb_candidat) * 100;
        genListAdd(list,(void*)&winner);
    }

    deleteList(&winningCandidates);

    return list;
}