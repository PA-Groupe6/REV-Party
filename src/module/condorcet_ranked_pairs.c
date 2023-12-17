/**
 * @file condorcet_ranked_pairs.c
 * @author IVANOVA ALina 
 * @date 20/11/2023
 *
 * @brief Fichier d'implementation de la méthode Condorcet avec le rangement des paires par ordre décroissant  
 *
 * Ce module implémente la méthode de Condorcet avec le rangement des paires par ordre décroissant pour les structures de données duel.h et graph.h
 *
 * La méthode de Condorcet Ranked Pairs est utilisée pour déterminer les préférences collectives d'un ensemble de choix en se basant sur les préférences individuelles des votants. 
 * Les paires de choix sont classées par ordre décroissant en fonction de leur force relative, et le gagnant est déterminé en respectant ces classements.
 * Cette méthode offre une approche précise pour refléter les préférences des votants.
 *
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée. Il est 
 * fortement recommandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur.
 */


#include <stdio.h>
#include <stdbool.h>
#include "../structure/duel.h"
#include <string.h>
#include <malloc.h>
#include "../structure/genericlist.h"
#include "../structure/graph.h"
#include "condorcet.h"
#include "../structure/data_struct_utils.h"


/**
 * @date 11/12/2023
 * @author Corentin LUDWIG
 * @brief trie la liste generique
 * @remark utilise le trie a bulle
 */
GenList* sortGenList(GenList* arcs){
    int i, j;
    Arc *a1,*a2;
    int size = genListSize(arcs);
    for (i = 0; i < size; i++) {
        for (j = 0; j < size - i; j++) {
            a1 = (Arc*)genListGet(arcs, j);
            a2 = (Arc*)genListGet(arcs, j+1);
            if (a1->weight< a2->weight) {
                genListSet(arcs, (void*) a1, j+1);
                genListSet(arcs, (void*) a2, j);
            }
        }
    }
    return arcs;
}


/**
 * @date 08/12/2023
 * @author Alina IVANOVA
 * @brief creation de list des arcs triés
 */
GenList* sortedArcsCreate(Duel* duel){
    //creating a graph
    int nb_cand = duelNbCandidat(duel) ;
    GenList* arcs = createGenList(sizeof(Arc)*nb_cand*2);

    for (int i = 0; i< nb_cand ; i++){
        for (int j = i+1; j<nb_cand-1; j++){  
            //adding the arc or duel to the graph
            int weight;
            Arc* arc = malloc(sizeof(Arc));
            int cand1_vs = duelGetValue(duel, i, j);
            int cand2_vs = duelGetValue(duel, j, i);
            if(cand1_vs>cand2_vs) {
                arc->id_dest = j;
                arc->id_src = i;
                arc->weight = cand1_vs;
                genListAdd(arcs, arc);
            }
            else if (cand2_vs > cand1_vs){
                arc->id_dest = i;
                arc->id_src = j;
                arc->weight = cand2_vs;
                genListAdd(arcs, arc);
            } 
        }    
    }
    GenList* sorted_arcs = sortGenList(arcs);
    free(arcs);
    return sorted_arcs;
}

/**
 * @date 11/12/2023
 * @author Alina IVANOVA
 * @brief creation d'un graphe sans cycles
 */
Graph* creatingGraph(Duel* duel){
    int nb_cand = duelNbCandidat(duel) ;
    GenList* labels_graph = createGenList(sizeof(char)*nb_cand);
    for(int i = 0; i < nb_cand; i++){
        genListAdd(labels_graph, duelIndexToLabel(duel, i));
    }

    Graph* winner_graph = createGraph(nb_cand, labels_graph);

    GenList* arcs_list = sortedArcsCreate(duel);

    for (int i = 0; i< nb_cand; i++){
        Arc* arc_current = genListGet(arcs_list, i);
        if(!graphIsMakingCycle(winner_graph, arc_current)){
            graphAdd(winner_graph,arc_current->id_src ,arc_current->id_dest , arc_current->weight);
        }
    }
    return winner_graph;
}


/**
 * @date 11/12/2023
 * @author Alina IVANOVA
 * @brief creation d'un list des candidats gagnants
 */
GenList* findWinnerGraph(Duel* duel){
    Graph* graph = creatingGraph(duel);

    GenList* arcs_sorted = graphToSortedList(graph, DESCENDING);

    free(graph);

    int nb_cand = duelNbCandidat(duel) ;
    int nb_arcs = graphNbArc(graph);
    int* wins_arcs = malloc(sizeof(int)*nb_cand);

    GenList* winners = createGenList(sizeof(WinnerCondorcet)*nb_cand);

    for(int i = 0; i<nb_cand; i++) wins_arcs[i] = 0;

    for(int i = 0; i<nb_arcs; i++){
        Arc* arc_current =(Arc*) genListGet(arcs_sorted, i);
        int src_id = arc_current->id_src;
        wins_arcs[src_id]++;
    }
    
    int max_winnings = 0;//max_role_src

    for(int i = 0; i < nb_cand; i++){
        if(wins_arcs[i]>max_winnings){
            while(!winners) free((WinnerCondorcet*)genListPop(winners));

            max_winnings = wins_arcs[i];
            
            WinnerCondorcet* cand_possible = malloc(sizeof(WinnerCondorcet));
            cand_possible->score = max_winnings;
            char* winner_name =  duelIndexToLabel(duel, i);
            strncpy(cand_possible->name, winner_name, MAX_LENGHT_LABEL);
            
            free(winner_name);
            genListAdd(winners, cand_possible);
        }
        else if(wins_arcs[i]==max_winnings){
            WinnerCondorcet* another_cand = malloc(sizeof(WinnerCondorcet));
            
            another_cand->score = max_winnings;
            char* winner_name =  duelIndexToLabel(duel, i);
            strncpy(another_cand->name, winner_name, MAX_LENGHT_LABEL);
            
            free(winner_name);
            genListAdd(winners, another_cand);
        }
    }

    return winners;

}




/**
 * @author IVANOVA ALina 
 * @date 11/12/2023
 * @brief on utilise la méthode condorcet avec le rangement des paires par ordre décroissant pour trouver le candidat gagnant
 *
 * @param[in] duel matrice des duels entre tous le candidats
 *
 * @return Le gagnant en utilisant la structure WinnerCondorcet.
*/
GenList* theWinnerRankedPairs(Duel* duel){
    int nb_cand = duelNbCandidat(duel) ;

    GenList* winners;

    if (CondorcetWinnerCriterion(duel)==NULL) {
        winners = findWinnerGraph(duel);
    }
    else {
        winners = createGenList(1);
        genListAdd(winners,CondorcetWinnerCriterion(duel));
    }
    return winners;

}
