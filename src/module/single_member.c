#include <stdlib.h>
#include <string.h>
#include "../structure/list.h"
#include "single_member.h"

/**
 * @author Alina IVANOVA
 * @date 21/11/2023 
 * @brief Calcul le nombre de voix pour chaque candidat
 * 
 * @param bale Ballot des votes
 * @return Tableau des scores par ordre des candidats dans le ballot
 */
unsigned* voteCountFirstRound(Bale* bale){
    unsigned nb_votes = baleNbVoter(bale);
    unsigned nb_candidates = baleNbCandidat(bale);
    unsigned* votesComplete = malloc(sizeof(int)*nb_candidates);
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
 * @name Ugo VALLAT
 * @date 30/11/2023
 * @brief Calcul les vainqueurs du premier tour
 * 
 * @param scores Tableau des scores des candiats 
 * @param nb_candidat Nombre de candidats
 * @param nb_voter Nombre de vontants
 * @return Liste des identifiants des candidats (numéro de colonne dans ballot)
 */
List* winnersOffirstRound(unsigned* scores, unsigned nb_candidat, unsigned nb_voter, int tour_voulu){
    List* winners = createList(nb_candidat);

    /* cas particuliers */
    if(nb_candidat == 0) return winners;
    if(nb_candidat == 1) {
        listAdd(winners, 0);
        return winners;
    }

    /* ### 1er parcours pour chercher le ou les meilleurs scores ### */
    unsigned max1 = scores[0]; /* score le plus grand */
    /* recherche du premier max */
    for (unsigned i = 1; i < nb_candidat; i++){
         printf("score = %d\n",scores[i]);
        if(scores[i] > max1){
            max1 = scores[i];
           
        }
    }
    
    /* ajout des candidats à la liste des gagnts */
    for(unsigned i = 0; i < nb_candidat; i++)
        if(scores[i] == max1)
            listAdd(winners, i);
    
    /* Si déjà plusieurs gagnants, pas de votes, ou 1 candidat > 50%, on arrête */
    if(listSize(winners) > 1 || max1 == 0 || max1 > 2*nb_voter || tour_voulu == 1) return winners;


    /* ### 2eme parcours pour chercher le ou les seconds ### */
    unsigned max2 = 0; /* 2eme score le plus grand */
    /* recherche du second max */
    for (unsigned i = 0; i < nb_candidat; i++)
        if(scores[i] > max2 && scores[i] != max1)
            max2 = scores[i];
        
    /* ajout des candidats à la liste des gagnts */
    for(unsigned i = 0; i < nb_candidat; i++)
        if(scores[i] == max2)
            listAdd(winners, i);
    
    return winners;
}
/***********
*   UNI1   *
***********/

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
    unsigned* summaryOfVotes = voteCountFirstRound(bale);

    /* Récupération du nom du gagnant */
    List* winningCandidates = winnersOffirstRound(summaryOfVotes, nb_candidat, baleNbVoter(bale), 1);

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

/***********
*   UNI2   *
***********/




/**
 * @name Ugo VALLAT
 * @date 30/11/2023
 * @brief Renvoie l'indice du gagnant avec le meilleur score (-1 si plusieurs)
 * 
 * @param tab tableau
 * @param size taille du tableau
 * @return indice du min, -1 si plusieurs
 */
int preferredCandidate(int* votes, unsigned nb_votes) {
    if(nb_votes == 0) return -1;

    /* recherche du meilleur score */
    int best_score = -1;
    for(unsigned i = 0; i < nb_votes; i++)
        if(best_score == -1 || (votes[i] > 0 && votes[i] < best_score))
            best_score = votes[i];
    
    /* choix du gagnant */
    unsigned winner = 0, ind = -1;
    if(best_score == -1) return -1;
    for(unsigned i = 0; i < nb_votes; i++) {
        if(votes[i] == best_score) {
            ind = i;
            winner++;
        }
    }
    /* si plus d'un gagnant, vote annulé */
    if(winner > 1) return -1;
    return ind;
}

/**
 * @name Ugo VALLAT
 * @date 30/11/2023
 * @brief Calcul les scores au second tour pour les candidats sélectionnés
 * 
 * @param b Ballot des scores 
 * @param winnersRound1 Gagnants du premier tour
 * @return Renvoie les tableau des scores (par ordre des gagnants du premier tour)
 */
int* voteCountSecondRound(Bale *b, List* winnersRound1){
    unsigned nb_winners = listSize(winnersRound1);
    unsigned nb_voters = baleNbVoter(b);
    int *count = malloc(sizeof(int)*nb_winners); /* tableau des scores */
    int votes[nb_winners]; /* tableau des votes d'une personne */
    int ind;

    /* initialisation du tableau des scores */
    for(unsigned i = 0; i < nb_winners; i++)
        count[i] = 0;

    /* pour chaque votant, recherche de son choix préféré */
    for(unsigned l = 0; l < nb_voters; l++) {
        /* récupération des votes du votant pour chaque candidat sélectionné */
        for(unsigned i = 0; i < nb_winners; i++) {
            votes[i] = baleGetValue(b, l, listGet(winnersRound1, i));
        }
        /* Ajout du gagnant aux scores si il existe */
        ind = preferredCandidate(votes, nb_winners);
        if(ind != -1)
            count[ind]++;
    }

    return count;
}

/**
 * @name Ugo VALLAT
 * @date 30/11/2023
 * @brief Renvoie la liste des positions du maximum dans le tableau des scores du deuxième tour
 * 
 * @param scores Scores des gagnants du premier tour
 * @param nb_winners Nombre de gagnants au premier tour
 * @return Liste des positions ddes maximums
 */
List* winnerOfsecondRound(int* scores, unsigned nb_winners) {
    List *winners_round_2 = createList(nb_winners);
    int max = scores[0];
    listAdd(winners_round_2, 0);
    
    /* récupération position des maximum */
    for(unsigned i = 1; i < nb_winners; i++) {
        if(scores[i] > max) {
            listClear(winners_round_2);
            listAdd(winners_round_2, i);
            max = scores[i];
        } else if(scores[i] == max) {
            listAdd(winners_round_2, i);
        }
    }
    return winners_round_2;
}

/**
 * @name Ugo VALLAT
 * @date 30/11/2023
 * @brief Récupère les information du candidat i est les stock dans une structure WinnerSingleTwo
 * 
 * @param b Ballot des votes 
 * @param id Indentifiant du gagnant (numéro de colonne dans le ballot)
 * @param scores Nombre de votes pour le gagnant
 * @param round Tour
 * @pre baleNbVoter(b) != 0
 * @return WinnerSingleTwo* 
 */
WinnerSingleTwo* createWinnerInfo(Bale* b, unsigned id, int score, unsigned round) {
    WinnerSingleTwo *winner;
    char* winner_name;
    /* malloc du winner */
    winner = malloc(sizeof(WinnerSingleTwo));

    /* récupération du nom */
    winner_name = baleColumnToLabel(b, id);
    strncpy(winner->name, winner_name, MAX_LENGHT_LABEL);
    free(winner_name);

    /* calcul du score en % */
    winner->score = ((float)score / baleNbVoter(b))*100;

    /* ajout du tour */
    winner->round = round;
    return winner;
}



GenList* theWinnerTwoRounds(Bale* bale){
    /* liste des vainqueurs (tour 1 et 2) */
    GenList *winners = createGenList(1);
    
    /* dimensions du ballot */
    unsigned nb_candidat = baleNbCandidat(bale);
    unsigned nb_voters = baleNbVoter(bale);

    /* si aucun candidat, retourne liste vide */
    if(nb_candidat == 0) return winners;

    /* ### debut du premier tour ### */
    unsigned* round_1_scores = voteCountFirstRound(bale);
    List* round_1_winners_id = winnersOffirstRound(round_1_scores, nb_candidat, nb_voters,2);


    unsigned nb_winners_round_1 = listSize(round_1_winners_id);

    /* On ajoute les gagnants du tour 1 à la liste des gagnants */
    WinnerSingleTwo *winner1;
    unsigned id_winner1;
    for(unsigned i = 0; i < nb_winners_round_1; i++) {
        id_winner1 = listGet(round_1_winners_id, i);
        /* récupération des informations du gagnant */
        winner1 = createWinnerInfo(bale, id_winner1, round_1_scores[id_winner1], 1);
        /* ajout à la liste des gagnants */
        genListAdd(winners, winner1);
    }

    /* si un seul gagant (1 seul candidat), gagne directement */
    if(nb_winners_round_1 <= 1) {
        deleteList(&round_1_winners_id);
        free(round_1_scores);
        return winners;
    }

    /* ### debut du second tour ### */
    int* round_2_scores = voteCountSecondRound(bale,round_1_winners_id);
    List* round_2_winners_pos = winnerOfsecondRound(round_2_scores , nb_winners_round_1);

    /* ajout de tous les gagnants du second tour à la liste */
    WinnerSingleTwo *winner2;
    unsigned id_winner2;
    while(!listEmpty(round_2_winners_pos)) {
        /* récupération des informations du gagnant */
        id_winner2 = listGet(round_1_winners_id, listGet(round_2_winners_pos, 0));
        winner2 = createWinnerInfo(bale, id_winner2, round_2_scores[listRemove(round_2_winners_pos, 0)], 2);
        /* ajout à la liste des gagnants */
        genListAdd(winners, winner2);
    }

    /* libération de la mémoire */
    deleteList(&round_1_winners_id);
    deleteList(&round_2_winners_pos);
    free(round_1_scores);
    free(round_2_scores);

    return winners;
}