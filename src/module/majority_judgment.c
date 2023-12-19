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

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../structure/bale.h"
#include <string.h>
#include <malloc.h>
#include "../structure/genericlist.h"
#include "majority_judgment.h"
#include "../logger.h"

typedef struct candidate_s {
    int index;
    int* votes;
    float original_percent_inf;
    float current_percent_inf;
    int inf_shift;
    float original_percent_sup;
    float current_percent_sup;
    int sup_shift;
} Candidate;

Candidate* createCandidate(int index) {
    Candidate* candidate = malloc(sizeof(Candidate));
    candidate->index = index;
    candidate->votes = NULL;
    candidate->original_percent_inf = 0.0f;
    candidate->current_percent_inf = 0.0f;
    candidate->inf_shift = 0;
    candidate->original_percent_sup = 0.0f;
    candidate->current_percent_sup = 0.0f;
    candidate->sup_shift = 0;
    return candidate;
}

void deleteCandidate(Candidate** cand) {
    if (cand == NULL || *cand == NULL) return;
    if ((*cand)->votes != NULL) {
        free((*cand)->votes);
    }
    free(*cand);
    cand = NULL;
}

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
 * @brief creation d'une liste des votes dans un ordre décroissant 
 */
void sortingVotesCandidate(Bale* bale, Candidate* candidate) {
    int nb_votes = baleNbVoter(bale);
    int* votes = malloc(sizeof(int)*nb_votes);
    for (int voter = 0; voter<nb_votes; voter++){
        votes[voter] = baleGetValue(bale, voter, candidate->index);
        if(votes[voter] ==-1) votes[voter] = baleNbCandidat(bale);
    }
    qsort(votes, nb_votes, sizeof(int), compare);
    candidate->votes = votes;
}



/**
 * @date 15/12/2023
 * @author Alina IVANOVA
 * @brief on cherche la mediane d'un candidat
 */
int medianCandidate(Bale* bale, Candidate* candidate) {
    if (candidate->votes == NULL)
        sortingVotesCandidate(bale, candidate);
    int nb_votes = baleNbVoter(bale);
    int a;
    if(nb_votes%2!=0) {
        a = candidate->votes[(nb_votes/2)];
    } else{
        a = candidate->votes[nb_votes/2 - 1];
    }
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
void computePercentagesCandidate(Bale* bale, Candidate* candidate, int median, int shift) {
    if (candidate->votes == NULL)
        sortingVotesCandidate(bale, candidate);
    // calcul des pourcentages
    int nb_votes = baleNbVoter(bale);
    int nb_inf_vote = 0;
    int nb_sup_vote = 0;
    int current_vote;
    int new_median = median + shift;
    for (unsigned int i = 0; i < baleNbVoter(bale); i++) {
        current_vote = candidate->votes[i];
        if (current_vote < new_median)
            nb_sup_vote++;
        else if (current_vote > new_median)
            nb_inf_vote++;
    }
    // mise à jour sélective
    if (shift >= 0) {
        candidate->current_percent_inf = (float) nb_inf_vote / nb_votes;
        candidate->inf_shift = shift;
    }
    if (shift <= 0) {
        candidate->current_percent_sup = (float) nb_sup_vote / nb_votes;
        candidate->sup_shift = shift;
    }
}

WinnerMajorityJudgment* candidateToWinner(Bale* bale, Candidate* candidate, int median) {
    WinnerMajorityJudgment* winner = malloc(sizeof(WinnerMajorityJudgment));
    char* winner_name = baleColumnToLabel(bale, candidate->index);
    strncpy(winner->name, winner_name, MAX_LENGHT_LABEL);
    free(winner_name);
    winner->median = median;
    if (candidate->current_percent_inf == 0.0f || candidate->current_percent_inf == 0.0f)
        computePercentagesCandidate(bale, candidate, median, 0);
    winner->percent_inf = candidate->original_percent_inf;
    winner->percent_sup = candidate->original_percent_sup;
    return winner;
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
void initializeCandidates(Bale* bale, GenList* candidates, int median) {
    // création de tout les candidats avec leurs pourcentages
    for (unsigned i = 0; i < genListSize(candidates); i++) {
        Candidate* candidate = genListGet(candidates, i);
        computePercentagesCandidate(bale, candidate, median, 0);
        candidate->original_percent_inf = candidate->current_percent_inf;
        candidate->original_percent_sup = candidate->current_percent_sup;
    }
}



int min_value = 1;
int max_value = 6;

unsigned removeLosers(GenList* candidates, float max_percent) {
    List* removed = createList(genListSize(candidates));
    for (unsigned i = 0; i < genListSize(candidates); i++) {
        Candidate* candidate = genListGet(candidates, i);
        if (candidate->current_percent_inf == max_percent)
            listAdd(removed, i);
    }
    unsigned nb_removed = listSize(removed);
    Candidate *ctmp;
    if (nb_removed != genListSize(candidates)) {
        for (int i = (int) listSize(removed)-1; i >= 0; i--) {
            ctmp = (Candidate*) genListRemove(candidates, listGet(removed, i));
            deleteCandidate(&ctmp);
        }
    } else nb_removed = 0;
    deleteList(&removed);
    return nb_removed;
}

unsigned keepWinners(GenList* candidates, float max_percent) {
    List* removed = createList(genListSize(candidates));
    for (unsigned i = 0; i < genListSize(candidates); i++) {
        Candidate* candidate = genListGet(candidates, i);
        if (candidate->current_percent_sup != max_percent)
            listAdd(removed, i);
    }
    unsigned nb_removed = listSize(removed);
    Candidate *ctmp;
    if (nb_removed != genListSize(candidates)) {
        for (int i = (int) listSize(removed)-1; i >= 0; i--) {
            ctmp = (Candidate*)genListRemove(candidates, listGet(removed, i));
            deleteCandidate(&ctmp);
        }
    } else nb_removed = 0;
    deleteList(&removed);
    return nb_removed;
}

bool isShiftPossible(int median, int shift_value) {
    int shifted_median = median + shift_value;
    return min_value <= shifted_median && shifted_median <= max_value;
}

void filterWinners(Bale* bale, GenList* candidates, int median) {
    // cas d'arrêt systématique
    if (genListSize(candidates) == 1) return;
    // calcul du max percent
    float max_percent = 0.0f;
    bool max_is_inf = false;
    for (unsigned i = 0; i < genListSize(candidates); i++) {
        Candidate* cand = genListGet(candidates, i);
        if (cand->current_percent_inf > max_percent) {
            max_percent = cand->current_percent_inf;
            max_is_inf = true;
        } else if (cand->current_percent_inf == max_percent) {
            max_is_inf = true;
        }

        if (cand->current_percent_sup > max_percent) {
            max_percent = cand->current_percent_sup;
            max_is_inf = false;
        }
    }
    // traitement du max
    if (max_is_inf) { // priorité aux groupes d'opposants
        unsigned removed = removeLosers(candidates, max_percent);
        if (removed == 0) {
            for (unsigned i = 0; i < genListSize(candidates); i++) {
                Candidate* candidate = genListGet(candidates, i);
                int shift_value = candidate->inf_shift + 1;
                if (!isShiftPossible(median, shift_value))
                    return;
                computePercentagesCandidate(bale, candidate, median, shift_value);
            }
        }
        filterWinners(bale, candidates, median);
    } else {
        unsigned removed = keepWinners(candidates, max_percent);
        if (removed == 0) {
            for (unsigned i = 0; i < genListSize(candidates); i++) {
                Candidate* candidate = genListGet(candidates, i);
                int shift_value = candidate->sup_shift - 1;
                if (!isShiftPossible(median, shift_value))
                    return;
                computePercentagesCandidate(bale, candidate, median, shift_value);
            }
        }
        filterWinners(bale, candidates, median);
    }
}

void clearCandidates(GenList* candidates) {
    for (unsigned i = 0; i < genListSize(candidates); i++) {
        Candidate* candidate = genListPop(candidates);
        deleteCandidate(&candidate);
    }
}

void foreachCandidateToWinner(Bale* bale, GenList* dest, GenList* source, int median) {
    for (unsigned i = 0; i < genListSize(source); i++) {
        Candidate* candidate = genListGet(source, i);
        WinnerMajorityJudgment* winner = candidateToWinner(bale, candidate, median);
        genListAdd(dest, winner);
        deleteCandidate(&candidate);
    }
}


/**
 * @date 15/12/2023
 * @author Alina IVANOVA
 * @brief creation d'une liste de(s) gagnant(s)
 */
GenList* theWinnerMajorityJudgment(Bale* bale, bool is_bale_judgment){
#ifdef DEBUG
    if (baleNbCandidat(bale) < 1)
        exitl("majority_judgment.c", "theWinnerMajorityJudgement", 1, "Il n'y a pas assez de candidats pour déterminer un vainqueur\n");
#endif

    // Convertion de la donnée 
    Bale *bale_save_tmp;
    if(!is_bale_judgment) {
        min_value = 1;
        max_value = baleNbCandidat(bale);
        unsigned nb_voter = baleNbVoter(bale);

        /* récupération des labels */
        bale_save_tmp = bale;
        GenList *label = createGenList(nb_voter);
        for(int i = 0; i < max_value; i++)
            genListAdd(label, baleColumnToLabel(bale, i));

        /* création du nouveau ballot */
        bale = createBale(nb_voter, max_value, label);
        while (!genListEmpty(label))
            free(genListPop(label));
        deleteGenList(&label);

        /* ajout des valeurs au nouveau ballot */
        int v;
        for(unsigned l = 0; l < nb_voter; l++) {
            for(int c = 0; c < max_value; c++) {
                v = baleGetValue(bale_save_tmp, l, c);
                if(v < 1) {
                    baleSetValue(bale, l, c, max_value);
                } else {
                    baleSetValue(bale, l, c, v);
                }
            }
        }
    } else {
        min_value = 1;
        max_value = 6;
    }

    // calcule du vainqueur par médiane
    int nb_cand = baleNbCandidat(bale);
    int min_median, current_median;
    GenList* candidates = createGenList(nb_cand);

    // initialisation du min
    Candidate* first_cand = createCandidate(0);
    min_median = medianCandidate(bale, first_cand);
    genListAdd(candidates, first_cand);

    // pour chaques candidats
    for(int i = 1; i < nb_cand; i++){
        Candidate* candidate = createCandidate(i);
        current_median = medianCandidate(bale, candidate);
        if (current_median < min_median) {
            min_median = current_median;
            clearCandidates(candidates);
            genListAdd(candidates, candidate);
        }
        else if (current_median == min_median){
            genListAdd(candidates, candidate);
        } else {
            deleteCandidate(&candidate);
        }
    }

    unsigned nb_winners = genListSize(candidates);
    // liste générique retournée
    GenList* winner_s = createGenList(nb_winners);

    if (nb_winners>1) { // si ex-aequo utilise des techniques plus avancées
        warnl("majority_judgment.c", "theWinnerMajorityJudgment",
            "Ex-aequo dans le vainqueur par médiane -> filtrage des gagnants\n");
        // peupler la liste winner_s des candidats ex-aequo
        initializeCandidates(bale, candidates, min_median);
        filterWinners(bale, candidates, min_median);
        foreachCandidateToWinner(bale, winner_s, candidates, min_median);
    } else {
        Candidate* winner_cand = genListGet(candidates, 0);
        WinnerMajorityJudgment* winner = candidateToWinner(bale, winner_cand, min_median);
        genListAdd(winner_s, winner);
        deleteCandidate(&winner_cand);
    }
    deleteGenList(&candidates);
    if(!is_bale_judgment) {
        deleteBale(&bale);
        bale = bale_save_tmp;
    }
    return winner_s;
}
