/**
 * @file condorcet_criterion.c
 * @author IVANOVA ALina 
 * @date 20/11/2023
 *
 * @brief Fichier d'implementation de la méthode Vainqueur de Condorcet 
 *
 * Ce module implémente la méthode de Vainqueur de Condorcet pour la structure de données duel.h
 *
 * La méthode de Vainqueur de Condorcet est utilisée pour déterminer s'il existe le candidat gagnant d'un ensemble de choix en se basant sur les préférences individuelles des votants. 
 * Cette méthode est particulièrement utile dans les processus de vote et de décision.
 *
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée, il est 
 * fortement recomandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include "../structure/duel.h"
#include <string.h>
#include <malloc.h>
#include "../structure/genericlist.h"
#include "condorcet.h"


/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on utilise la méthode vainqueur de Condorcet pour le trouver ou savoir si on doit implementer un autre méthode
 * 
 * @param[in] duel matrice des duels entre tous le candidats
 *
 * @return Le gagnant en utilisant la structure WinnerCondorcet ou NULL s'il n'y a pas de vaiqueur
*/

WinnerCondorcet* CondorcetWinnerCriterion(Duel* duel){
    int nbCandidats= duelNbCandidat(duel);
    int maxWins = 0;
    int winner = -1;
    for (int cand1 = 0; cand1<nbCandidats;cand1++){
        int winsCandidate = 0;
        for (int cand2 = 0; cand2 < nbCandidats; cand2++){
            int won = duelGetValue(duel, cand1, cand2); 
            int lost = duelGetValue(duel, cand2, cand1); 
            if(won > lost){
                winsCandidate++;
            }
        }
        if (maxWins<winsCandidate){
            maxWins = winsCandidate;
            winner = cand1;
        }
        else if (maxWins == winsCandidate) {
            winner = -1;
        }
    }
    if (winner == -1) return NULL;
    WinnerCondorcet* vainqueur = malloc(sizeof(WinnerCondorcet));
    char* winner_name =  duelIndexToLabel(duel, winner);
    strncpy(vainqueur->name, winner_name, 256);
    free(winner_name);
    vainqueur->score = maxWins;
    return vainqueur;
}