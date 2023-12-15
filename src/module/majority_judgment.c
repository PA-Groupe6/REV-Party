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
    for (int voter = 0; voter<nbVotes; voter++)
        votes[voter] = baleGetValue(bale, voter, candidate);
    qsort(votes, nbVotes, sizeof(int), compare);
    return votes;
}


/**
 * @date 15/12/2023
 * @author Alina IVANOVA
 */
int medianeCandidate(Bale* bale, int candidate){
    int nbVotes = baleNbVoter(bale);
    int* votes = sortingVotesCandidate(bale,candidate);
    int a;
    if(nbVotes%2!=0) {
        a = votes[(nbVotes/2)];
    } else{
        a = (votes[nbVotes/2]+ votes[nbVotes/2-1])/2;
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
    for (int i = 0; i < baleNbVoter(bale); i++) {
        currentVote = sortedVotes[i];
        if (currentVote < median)
            nbInfVote++;
        else if (currentVote > median)
            nbSupVote++;
    }
    free(sortedVotes);
    *percentInf = (float) nbInfVote/nbVotes;
    *percentSup = (float) nbSupVote/nbVotes;
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
void severalWinners(Bale* bale, GenList* winners, int median, int nb_winners, int* indexCandidates) {
    // création de tout les candidats avec leurs pourcentages
    for (int i = 0; i < nb_winners; i++) {
        Candidate* winner = malloc(sizeof(struct s_winner_MajorityJudgment));
        winner->candIndex = indexCandidates[i];
        winner->mediane = median;
        computePercentagesCandidate(bale, winner->candIndex, median, &winner->percentInf, &winner->percentSup);
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
bool winnersContains(GenList* winners, Candidate* winner) {
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
    Candidate* current_cand;
    float current_perc;
    // pour chaque candidats dans la liste des gagnants
    for (unsigned int i = 0; i < genListSize(winners); i ++) {
        current_cand = (Candidate*) genListGet(winners, i);
        current_perc = current_cand->percentSup;
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
    Candidate* current_cand;
    float current_perc;
    // pour chaque candidats dans la liste des gagnants
    for (unsigned int i = 0; i < genListSize(winners); i ++) {
        current_cand = (Candidate*) genListGet(winners, i);
        current_perc = current_cand->percentInf;
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
    int medianes[nb_cand];
    int index_winners[nb_cand];
    int max_median = 0, nb_winners = 1;
    for(int cand = 0; cand < nb_cand; cand++){
        medianes[cand] = medianeCandidate(bale, cand);
        if (medianes[cand]>max_median){
            max_median = medianes[cand];
            nb_winners = 1;
            index_winners[0] = cand;
        }
        else if (medianes[cand] == max_median){
            index_winners[nb_winners] = cand;
            nb_winners++;
        }
    }

    // liste générique retournée
    GenList* winner_s = createGenList(nb_winners);

    if (nb_winners>1) { // si ex-aeqo utilise des techniques plus avancées
        // peupler la liste winner_s des candidats ex-aeqo
        severalWinners(bale, winner_s, max_median, nb_winners, index_winners);
        // parmis les ex-aeqo, calcul du vainqueur par moindre mal
        winner_s = leastHarmCand(winner_s);
        if(genListSize(winner_s) > 1) // si encore ex-aeqo
            // parmis les ex-aeqo, calcul du vainqueur par meilleur bien
            winner_s = bestCand(winner_s);
    } else {
        Candidate* winner = malloc(sizeof(Candidate));
        winner->candIndex = index_winners[0];
        winner->mediane = max_median;
        computePercentagesCandidate(bale, winner->candIndex, winner->mediane, &winner->percentInf, &winner->percentSup);
        genListAdd(winner_s, winner);
    }

    return winner_s;
}
