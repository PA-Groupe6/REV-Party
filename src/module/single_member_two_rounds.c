/**
 * @author Ugo VALLAT, IVANOVA ALina 
 * @date 4/11/2023
 *
 * @brief Fichier d'implémentation de la méthode Single Member (Uninominal) à deux rounds
 *
 * Ce module implémente la méthode de Scrutin Uninominal à deux rounds pour la structure de données bale.h.
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
#include <stdio.h>



int* voteCountFirstround(Bale* bale){
    unsigned nb_votes = baleNbVoter(bale);
    unsigned nb_candidates = baleNbCandidat(bale);
    int* votesComplete = malloc(sizeof(int)*nb_candidates);
    memset(votesComplete, 0, sizeof(int)*nb_candidates);
    for (unsigned i = 0; i<nb_votes;i++){
        //on recoit le liste avec le(les) candidat(s) qui a recu le val max a partir d'electeur i
        GenList* winner = baleMax(bale, i, -1);
        if (genListSize(winner)==1){//si on a qu'un seul candidat avec le note max, on prend en compte le vote
            int cand = ((int*)genListGet(winner, 0))[2];//num de candidat avec le val max
            votesComplete[cand] += 1;//on ajoute le vote
        }
        while(!genListEmpty(winner))
            free(genListPop(winner));
        deleteGenList(&winner);
    }
    return votesComplete;
}

List* winnersOfFirstRound(int* votes, int nb_candidat){
    int max1;
    List* id_max1 = createList(nb_candidat);
    
    /*Premier max */
    max1 = votes[0];
    listAdd(id_max1, 0);
    for(int i = 1; i < nb_candidat; i++) {
        if(votes[i] > max1) {
            listClear(id_max1);
            listAdd(id_max1, i);
            max1 = votes[i];
        } else if (votes[i] == max1) {
            listAdd(id_max1, i);
        }
    }
    if(listSize(id_max1) > 1 || nb_candidat == 1)
        return id_max1;
    int max2, i = 0;
    List* id_max2 = createList(nb_candidat);

    while(votes[i] >= max1) i++;

    listAdd(id_max2, i);
    max2 = votes[i];
    for(;i < nb_candidat; i++) {
        if(votes[i] > max2 && votes[i] < max1) {
            listClear(id_max2);
            listAdd(id_max2, i);
            max2 = votes[i];
        } else if (votes[i] == max2) {
            listAdd(id_max2, i);
        }
    }

    while(!listEmpty(id_max2))
        listAdd(id_max1, listPop(id_max2));
    
    deleteList(&id_max2);
    return id_max1;
}

/**
 * @brief Renvoie l'indice de la valeur max du tableau (-1 si plusieurs)
 * 
 * @param tab tableau
 * @param size taille du tableau
 * @return indice du max, -1 si plusieurs
 */
int indMaxInTab(int* tab, unsigned size) {
    int max = tab[0];
    unsigned idmax = 0;
    int nb = 1;
    for(unsigned i = 1; i < size; i++) {
        if(tab[i] > max) {
            max = tab[i];
            idmax = i;
            nb = 1;
        } else if (tab[i] == max) {
            nb++;
        }
    }
    if(nb > 1) return -1;
    return idmax;
}


int* voteCountSecondround(Bale *b, List* ftwinners){
    unsigned nb_winners = listSize(ftwinners);
    unsigned nb_voters = baleNbVoter(b);
    int *count = malloc(sizeof(int)*nb_winners);
    int tab[nb_winners];
    int ind;

    for(unsigned i = 0; i < nb_winners; i++)
        count[i] = 0;

    for(unsigned l = 0; l < nb_voters; l++) {
        for(unsigned i = 0; i < nb_winners; i++) {
            tab[i] = baleGetValue(b, l, listGet(ftwinners, i));
        }
        ind = indMaxInTab(tab, nb_winners);
        if(ind != -1)
            count[ind]++;
    }
    return count;
}


List* winnerOfSecondRound(int* scores, unsigned nb_winnners) {
    List *list = createList(nb_winnners);
    int max = scores[0];
    listAdd(list, 0);

    for(unsigned i = 1; i < nb_winnners; i++) {
        if(scores[i] > max) {
            listClear(list);
            listAdd(list, i);
            max = scores[i];
        } else if(scores[i] == max) {
            listAdd(list, i);
        }
    }
    return list;
}


WinnerSingleTwo* createWinnerInfo(Bale* b, List* winners_fst_round, int* scores, unsigned i, unsigned round) {
    WinnerSingleTwo *winner;
    char* winner_name;

    winner = malloc(sizeof(WinnerSingleTwo));
    winner_name = baleColumnToLabel(b, i);
    strncpy(winner->name, winner_name, MAX_LENGHT_LABEL);
    free(winner_name);
    winner->score = ((float)scores[i] / baleNbVoter(b))*100;
    winner->round = round;
    return winner;
}

void printTabInt(int* tab, int size, char* name) {
    printf(" [tab %s ] : [", name);
    for(int i = 0; i < size; i++)
        printf(" %d ,", tab[i]);
    printf(" ]\n");
}

GenList* theWinnerTwoRounds(Bale* bale){
    GenList *list = createGenList(1);
    
    unsigned nb_candidat = baleNbCandidat(bale);

    if(nb_candidat == 0) return list;


    int* firstroundResults = voteCountFirstround(bale);
    List* firstroundWinners = winnersOfFirstRound(firstroundResults, nb_candidat);
    unsigned nb_winners_first_round = listSize(firstroundWinners);

    for(unsigned i = 0; i < nb_winners_first_round; i++) {
        genListAdd(list, createWinnerInfo(bale, firstroundResults, listGet(firstroundWinners, i), 1));
    }

    if(nb_winners_first_round <= 1) {
        deleteList(&firstroundWinners);
        free(firstroundResults);
        return list;
    }



    //debut du second round
    int* secondroundResult = voteCountSecondround(bale,firstroundWinners);
    List* secondroundWinners = winnerOfSecondRound(secondroundResult, nb_winners_first_round);

    printTabInt(secondroundResult, nb_winners_first_round, "secondroundResult");
    while(!listEmpty(secondroundWinners)) {
        genListAdd(list, createWinnerInfo(bale, firstroundWinners, secondroundResult, listPop(secondroundWinners), 2));
    }

    deleteList(&firstroundWinners);
    deleteList(&secondroundWinners);
    free(firstroundResults);
    free(secondroundResult);
    return list;
}