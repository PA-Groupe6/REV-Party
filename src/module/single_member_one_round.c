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
    unsigned nb_votes = baleNbVoter(bale);
    unsigned nb_candidates = baleNbCandidat(bale);
    int* votesComplete = malloc(sizeof(int)*nb_candidates);
    memset(votesComplete, 0, sizeof(int)*nb_candidates);
    for (unsigned i = 0; i<nb_votes;i++){
        //on recoit le liste avec le(les) candidat(s) qui a recu le val max a partir d'electeur i
        GenList* winner = baleMin(bale, i, -1);
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

/**
 * @author Alina IVANOVA, Corentin LUDWIG
 * @date 21/11/2023 
 */
List* maxVotesCandidat(int* votes, int nb_candidat){
    int max = votes[0];
    List *winner = createList(1);
    listAdd(winner, 0);
    for (int i = 1; i < nb_candidat; i++ ){
        if (votes[i] > max) {
            max = votes[i];
            while(!listEmpty(winner))
                listPop(winner);
            listAdd(winner, i);
        } else if( votes[i] == max){
            listAdd(winner,i);
        }
    }
    return winner;
}

void displayWinnerSingleMemberSingle(WinnerSingle *winner) {
    printf("\t<+> %s : %3.2f\n", winner->name, winner->score);
}

/**
 * @author Alina IVANOVA
 * @date 21/11/2023 
 */
GenList* theWinnerOneRound(Bale* bale){
    GenList *list = createGenList(1);
    WinnerSingle *winner;
    char* winner_label;
    unsigned nb_candidat = baleNbCandidat(bale);

    if(nb_candidat == 0) return list;

    /* décompte des voies de chaque candidat */
    int* summaryOfVotes = voteCount(bale);

    /* Récupération du nom du gagnant */
    List* winningCandidates = maxVotesCandidat(summaryOfVotes, nb_candidat);

    for (unsigned i = 0; i < listSize(winningCandidates); i++)
    {
        winner = malloc(sizeof(WinnerSingle));
        int winningCandidate = listGet(winningCandidates,i);

        /* récupération du label candidat */
        winner_label = baleColumnToLabel(bale, winningCandidate);
        strncpy(winner->name, winner_label, MAX_LENGHT_LABEL);
        free(winner_label);

        /* calcul du score */
        winner->score = ((float)summaryOfVotes[winningCandidate]/baleNbVoter(bale)) * 100;
        genListAdd(list,(void*)winner);
    }
    free(summaryOfVotes);
    deleteList(&winningCandidates);

    return list;
}


/*
    ===================
    ===  AFFICHEUR  ===
    ===================
*/



unsigned maxLenghtLabelWinner(GenList *l) {
    unsigned nb_winners = genListSize(l);
    unsigned max = 0;
    unsigned tmp;
    for(unsigned i = 0; i < nb_winners; i++) {
        tmp = strlen(((WinnerSingle*)genListGet(l, i))->name);
        if(tmp > max) max = tmp;
    }
    return max;
}


/**
 * @brief Affiche nb_fois la chaine de caractère s
 * 
 * @param s Chaine de caractères à afficher
 * @param nb Nombre de fois
 */
void printStringN(char* s, unsigned nb) {
    for(unsigned i = 0; i < nb; i++) {
        printf("%s",s);
    }
}


void displayListWinnerSingle(GenList *l) {
    unsigned nb_winners = genListSize(l);
    unsigned max_lenght_winner = maxLenghtLabelWinner(l);
    unsigned max_lenght_case = max_lenght_winner + 20;

    /* bordure haute */
    printf("\t╔");
    printStringN("═", max_lenght_case);
    printf("╗\n");

    /* titre */
    printf("\t║  RESULTATS UNI 1 :");
    printStringN(" ", max_lenght_case-19);
    printf("║\n");

    /* vainqueurs */
    WinnerSingle *wtmp;
    for(unsigned i = 0; i < nb_winners; i++) {
        wtmp = genListGet(l, i);
        printf("\t║  • %-*s : %3.2f        ║\n", max_lenght_winner, wtmp->name, wtmp->score);
    }

    /* bordure basse */
    printf("\t╚");
    printStringN("═", max_lenght_case);
    printf("╝\n");
}