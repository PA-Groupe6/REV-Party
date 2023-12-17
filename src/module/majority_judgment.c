/**
 * @file majority_judgment.c
 * @author IVANOVA ALina 
 * @date 24/11/2023
 *
 * @brief Fichier d'implementation de la méthode Jugement majoritaire.  
 *
 * Ce module implémente la méthode de Jugement majoritaire pour la structure de données de ballot
 *
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée, il est 
 * fortement recomandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../structure/bale.h"
#include <string.h>
#include <malloc.h>
#include "../structure/genericlist.h"
#include "majority_judgment.h"

/**
 * @date 15/12/2023
 * @author Alina IVANOVA
 */
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}


/**
 * @date 15/12/2023
 * @author Alina IVANOVA
 */
int* sortingVotesCandidate(Bale* bale, int candidate){
    int nbVotes = baleNbVoter(bale);
    int* votes = malloc(sizeof(int)*nbVotes);
    for (int voter = 0; voter<nbVotes; voter++){
        votes[voter] = baleGetValue(bale, voter, candidate);
        if(votes[voter] ==-1) votes[voter] = baleNbCandidat(bale);
    }
    qsort(votes, nbVotes, sizeof(int), compare);
    return votes;
}



/**
 * @date 15/12/2023
 * @author Alina IVANOVA
 */
int medianCandidate(Bale* bale, int candidate){
    int nb_votes = baleNbVoter(bale);
    int* votes = sortingVotesCandidate(bale, candidate);
    int a;
    if(nb_votes%2!=0) {
        a = votes[(nb_votes/2)];
    } else{
        a = votes[nb_votes/2 - 1];
    }
    free(votes);
    return a;
}


/**
 * @date 15/12/2023
 * @author IVANOVA Alina, LAFORGE Mateo
 * @brief calcule le pourcentage inférieur et supérieurs des votes pour un candidat
 * 
 * @param[in] bale matrice ballot source
 * @param[in] candidate indice candidat
 * @param[out] precentInf pourcentage inférieur de vote du candidat
 * @param[out] percentSup pourcentage supérieur de vote du candidat
*/
void computePercentagesCandidate(Bale* bale, int candidate, int median, float* percentInf, float* percentSup) {
    int nbVotes = baleNbVoter(bale);
    int* sortedVotes = sortingVotesCandidate(bale, candidate);
    int nbInfVote = 0;
    int nbSupVote = 0;
    int currentVote;
    for (unsigned int i = 0; i < baleNbVoter(bale); i++) {
        currentVote = sortedVotes[i];
        if (currentVote < median)
            nbSupVote++;
        else if (currentVote > median)
            nbInfVote++;
    }
    free(sortedVotes);
    *percentInf = (float) nbInfVote/nbVotes;
    *percentSup = (float) nbSupVote/nbVotes;
    printf("%s percent Sup %f \t percent Inf %f \n", baleColumnToLabel(bale, candidate), (*(float*)percentSup), (*(float*)percentInf));
    printf("median %d \t nbSup %d \t nbInf %d\n",median, nbSupVote, nbInfVote);
}


/**
 * @date 15/12/2023
 * @author IVANOVA Alina, LAFORGE Mateo
 * @brief peuple la liste winners des candidats dans indexCandidates avec leurs pourcentages d'opposants et partisans
 * 
 * @param[in] bale le ballot source
 * @param[in-out] winners liste des candidats vainqueurs
 * @param[in] median la mediane avec laquelle les candidats ont gagnés
 * @param[in] nbWinners nombre de vainqueurs
 * @param[in] indexCandidates listes des indices des candidats vainqueurs
 */
void severalWinners(Bale* bale, GenList* winners, int median, int nb_winners, int* index_candidates) {
    // création de tout les candidats avec leurs pourcentages
    for (int i = 0; i < nb_winners; i++) {
        WinnerMajorityJudgment* winner = malloc(sizeof(WinnerMajorityJudgment));
        char* winner_name =  baleColumnToLabel(bale, index_candidates[i]);
        strncpy(winner->name, winner_name, MAX_LENGHT_LABEL);
        free(winner_name);
        winner->median = median;
        computePercentagesCandidate(bale, index_candidates[i], median, &winner->percent_inf, &winner->percent_sup);
        genListAdd(winners, winner);
    }
}


/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief regarde si la liste de vainqueurs contient un vainqueur spécifique
 * 
 * @param[in] winners la liste de vainqueurs
 * @param[in] winner le vainqueur à chercher
 * @pre winners est une liste de Candidats
 * 
 * @return vrai si winner est dans winners, faux sinon
*/
bool winnersContains(GenList* winners, WinnerMajorityJudgment* winner) {
    for (unsigned int i = 0; i < genListSize(winners); i++)
        if (genListGet(winners, i) == winner)
            return true;
    return false;
}


/**
 * @date 15/12/2023
 * @author IVANOVA Alina, LAFORGE Mateo
 * @brief applique la tactique du meilleur bien (récupère le candidat qui a le plus de partisans)
 * 
 * @param[in] winners liste des gagnants courant à départagés
 * @pre |winners| > 1
 * 
 * @return la liste des vainqueurs de cette tactique
 * @note il peux subsister des ex-aeqo dans la liste de retour
 */
GenList* bestCand(GenList* winners){
    GenList* best_winners = createGenList(genListSize(winners));
    float max_perc = 0.0f;
    WinnerMajorityJudgment* current_cand;
    float current_perc;
    // pour chaque candidats dans la liste des gagnants
    for (unsigned int i = 0; i < genListSize(winners); i ++) {
        current_cand = (WinnerMajorityJudgment*) genListGet(winners, i);
        current_perc = current_cand->percent_sup;
        if (current_perc > max_perc) {
            // mise à jour du min
            max_perc = current_perc;
            // vide la liste
            while(!genListEmpty(best_winners))
                genListPop(best_winners);
        }
        if (current_perc >= max_perc) {
            genListAdd(best_winners, current_cand);
        }
    }
    while (!genListEmpty(winners)) {
        void* winner = genListPop(winners);
        if (!winnersContains(best_winners, winner))
            free(winner);
    }
    deleteGenList(&winners);
    return best_winners;
}


/**
 * @date 15/12/2023
 * @author IVANOVA Alina, LAFORGE Mateo
 * @brief applique la tactique du moindre mal (récupère le candidat qui a le moins d'opposants)
 * 
 * @param[in] winners liste des gagnants courant à départagés
 * @pre |winners| > 1
 * 
 * @return la liste des vainqueurs de cette tactique
 * @note il peux subsister des ex-aeqo dans la liste de retour
 */
GenList* leastHarmCand(GenList* winners) {
    GenList* least_harm_winners = createGenList(genListSize(winners));
    float min_perc = 1.0f;
    WinnerMajorityJudgment* current_cand;
    float current_perc;
    // pour chaque candidats dans la liste des gagnants
    for (unsigned int i = 0; i < genListSize(winners); i ++) {
        current_cand = (WinnerMajorityJudgment*) genListGet(winners, i);
        current_perc = current_cand->percent_inf;
        if (current_perc < min_perc) {
            // mise à jour du min
            min_perc = current_perc;
            // vide la liste
            while(!genListEmpty(least_harm_winners))
                genListPop(least_harm_winners);
        }
        if (current_perc <= min_perc) {
            genListAdd(least_harm_winners, current_cand);
        }
    }
    while (!genListEmpty(winners)) {
        void* winner = genListPop(winners);
        if (!winnersContains(least_harm_winners, winner))
            free(winner);
    }
    deleteGenList(&winners);
    return least_harm_winners;
}


/**
 * @date 15/12/2023
 * @author Alina IVANOVA
 */
GenList* theWinnerMajorityJudgment(Bale* bale){
    // calcule du vainqueur par médiane
    int nb_cand = baleNbCandidat(bale);
    int medians[nb_cand];
    int index_winners[nb_cand];
    int min_median = nb_cand, nb_winners = 1;
    for(int cand = 0; cand < nb_cand; cand++){
        medians[cand] = medianCandidate(bale, cand);
        printf("%s median = %d\n", baleColumnToLabel(bale, cand),medians[cand]);
        if (medians[cand]<min_median){
            min_median = medians[cand];
            nb_winners = 1;
            index_winners[0] = cand;
        }
        else if (medians[cand] == min_median){
            index_winners[nb_winners] = cand;
            nb_winners++;
        }
    }

    // liste générique retournée
    GenList* winner_s = createGenList(nb_winners);

    if (nb_winners>1) { // si ex-aeqo utilise des techniques plus avancées
        // peupler la liste winner_s des candidats ex-aeqo
        severalWinners(bale, winner_s, min_median, nb_winners, index_winners);
        // parmis les ex-aeqo, calcul du vainqueur par moindre mal
        winner_s = leastHarmCand(winner_s);
        if(genListSize(winner_s) > 1) // si encore ex-aeqo
            // parmis les ex-aeqo, calcul du vainqueur par meilleur bien
            winner_s = bestCand(winner_s);
    } else {
        WinnerMajorityJudgment* winner = malloc(sizeof(WinnerMajorityJudgment));
        char* winner_name =  baleColumnToLabel(bale, index_winners[0]);
        strncpy(winner->name, winner_name, MAX_LENGHT_LABEL);
        free(winner_name);
        winner->median = min_median;
        computePercentagesCandidate(bale, index_winners[0], winner->median, &winner->percent_inf, &winner->percent_sup);
        genListAdd(winner_s, winner);
    }

    return winner_s;
}
