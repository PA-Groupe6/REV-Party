/**
 * @file condorcet_schulze.c
 * @author IVANOVA ALina 
 * @date 14/12/2023
 *
 * @brief Fichier d'implementation de la méthode Condorcet Schulze
 *
 * Ce module implémente la méthode de Condorcet Schulze pour la structure de données duel.h.
 *
 * La méthode de Condorcet Schulze est utilisée pour déterminer les préférences collectives d'un ensemble de choix en se basant sur les préférences individuelles des votants.
 * Les duels entre choix sont évalués, et le gagnant est déterminé en respectant les relations de préférence les plus fortes dans un ordre de classement.
 * Cette méthode offre une approche précise pour refléter les préférences des votants.
 *
 * Ce fichier d'en-tête fournit les fonctions et les structures nécessaires pour calculer les résultats selon la méthode de Condorcet Schulze pour les données contenues dans duel.h.
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


int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}



/**
 * @date 16/12/2023
 * @author Alina IVANOVA
 * @brief creation des arcs à partir des duel, le gagnant a le valeur de duel comme un weight et son adversaire a 0
 */
GenList* arcsCreate(Duel* duel){
    //creating a graph
    int nb_cand = duelNbCandidat(duel) ;
    GenList* arcs = createGenList(nb_cand*(nb_cand-1));

    for ( int i = 0; i< nb_cand ; i++){
        for ( int j = i+1; j<nb_cand; j++){  
            //adding the arc or duel to the graph
            Arc* arc = malloc(sizeof(Arc));
            Arc* arc_inverse = malloc(sizeof(Arc));
            int cand1_vs = duelGetValue(duel, i, j);
            int cand2_vs = duelGetValue(duel, j, i);

            if(cand1_vs>cand2_vs) {
                arc->id_dest = j;
                arc->id_src = i;
                arc->weight = cand1_vs;

                arc_inverse->id_dest = i;
                arc_inverse->id_src = j;
                arc_inverse->weight = 0;
            }
            else {
                arc->id_dest = i;
                arc->id_src = j;
                arc->weight = cand2_vs;

                arc_inverse->id_dest = j;
                arc_inverse->id_src = i;
                arc_inverse->weight = 0;
            } 
            genListAdd(arcs, arc);
            genListAdd(arcs, arc_inverse);
        }    
    }

    return arcs;
}



/**
 * @date 16/12/2023
 * @author Alina IVANOVA
 * @brief creation d'un graphe à partir de duel
 */

Graph* graphCreate(Duel* duel){
    int nb_cand = duelNbCandidat(duel) ;
    GenList* labels_graph = createGenList(sizeof(char)*nb_cand);
    for(int i = 0; i < nb_cand; i++){
        genListAdd(labels_graph, duelIndexToLabel(duel, i));
    }

    Graph* winner_graph = createGraph(nb_cand, labels_graph);

    GenList* arcs_list = arcsCreate(duel);

    for (int i = 0; i< nb_cand*(nb_cand-1); i++){
        Arc* arc_current = genListGet(arcs_list, i);
        graphAdd(winner_graph,arc_current->id_src ,arc_current->id_dest , arc_current->weight);
    }

    while(!genListEmpty(labels_graph))
        free(genListPop(labels_graph));
    deleteGenList(&labels_graph);

    while(!genListEmpty(arcs_list))
        free(genListPop(arcs_list));
    deleteGenList(&arcs_list);

    return winner_graph;
}




/**
 * @date 16/12/2023
 * @author Alina IVANOVA
 * @brief modification de graph pour que le weight = le chemin entre source et destination
 */

Graph* findingPaths(Duel* duel){
    Graph* graph_for_search = graphCreate(duel);
    int nb_cand = duelNbCandidat(duel);
    int cand_2vs1, cand_3vs1, cand_2vs3;
    Arc* current_arc;

    for(int cand_3 = 0; cand_3<nb_cand; cand_3++){
        for(int cand_2 = 0;cand_2<nb_cand; cand_2++){
            
            if (cand_2!=cand_3){
                for(int cand_1 = 0; cand_1 < nb_cand; cand_1++){
                    
                    if(cand_3!=cand_1 && cand_1!=cand_2){

                        cand_2vs1 = graphGetWeight(graph_for_search, cand_2, cand_1);
                        cand_2vs3 = graphGetWeight(graph_for_search, cand_2, cand_3);
                        cand_3vs1 = graphGetWeight(graph_for_search, cand_3, cand_1);
                        
                        current_arc = graphGetArc(graph_for_search, cand_2, cand_1);

                        int weight = max(cand_2vs1, min(cand_2vs3, cand_3vs1));
                        current_arc->weight = weight; 

                        graphSetWeight(graph_for_search, cand_2, cand_1, weight);
                    }
                }
            } 
        }
    }
    return graph_for_search;
}



/**
 * @date 16/12/2023
 * @author Alina IVANOVA
 * @brief creation d'un duel à partir d'une graphe des chemins
 */

Duel* createDuelfromGraph(Graph* graph){
    int nb_cand = graphNbVertex(graph);
    GenList* labels = createGenList(nb_cand);
    for (int i = 0 ; i<nb_cand; i++){
        genListAdd(labels, graphGetLabel(graph, i));
    }

    Duel* duel = createDuel(nb_cand, labels);
    for(int cand = 0; cand< nb_cand; cand++){
        for(int cand_2 = cand+1 ; cand_2 < nb_cand; cand_2++){
            duelSetValue(duel, cand, cand_2, graphGetWeight(graph, cand, cand_2));
            duelSetValue(duel, cand_2, cand , graphGetWeight(graph, cand_2, cand));
        }
    }

    while(!genListEmpty(labels))
        free(genListPop(labels));
    deleteGenList(&labels);

    return duel;
}


/**
 * @date 16/12/2023
 * @author Alina IVANOVA
 * @brief creation d'un genlist de(s) gagnat(s) à partir d'un duel des chemins
 */
GenList* findWinnerGraph(Duel* duel){
    Graph* graph_path = findingPaths(duel);
    Duel* duel_path = createDuelfromGraph(graph_path);

    deleteGraph(&graph_path);

    int nb_cand= duelNbCandidat(duel);

    GenList* candidates = createGenList(1);
    int max_wins = 0;
    for (int cand_1 = 0; cand_1<nb_cand;cand_1++){
        int winsCandidate = 0;
        
        for (int cand_2 = 0; cand_2 < nb_cand; cand_2++){
            int win = duelGetValue(duel_path, cand_1, cand_2); 
            int loss = duelGetValue(duel_path, cand_2, cand_1); 
            if(win > loss)winsCandidate++;
        }

        if (max_wins<winsCandidate){
           while(genListSize(candidates)!=0) free((WinnerCondorcet*)genListPop(candidates));
            max_wins = winsCandidate;
            
            WinnerCondorcet* cand_possible = malloc(sizeof(WinnerCondorcet));
            
            cand_possible->score = winsCandidate;
            char* winner_name =  duelIndexToLabel(duel_path, cand_1);
            strncpy(cand_possible->name, winner_name, MAX_LENGHT_LABEL);
            
            free(winner_name);
            genListAdd(candidates, cand_possible);
        }
        else if (max_wins == winsCandidate) {
            WinnerCondorcet* cand_possible = malloc(sizeof(WinnerCondorcet));
            
            cand_possible->score = winsCandidate;
            char* winner_name =  duelIndexToLabel(duel_path, cand_1);
            strncpy(cand_possible->name, winner_name, MAX_LENGHT_LABEL);
            
            free(winner_name);
            genListAdd(candidates, cand_possible);
        }
    }

    deleteDuel(&duel_path);

    return candidates;

}


/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on utilise la méthode de Schulze pour trouver le candidat gagnant
 *
 * @param[in] duel matrice des duels entre tous le candidats
 *
 * @return Le gagnant en utilisant la structure WinnerCondorcet.
*/
GenList* theWinnerSchulze(Duel* duel){
    GenList* winners;
    WinnerCondorcet* winner_condorcet = CondorcetWinnerCriterion(duel);
    if (winner_condorcet==NULL) {
        winners = findWinnerGraph(duel);
    }
    else {
        winners = createGenList(1);
        genListAdd(winners,winner_condorcet);
    }
    return winners;
}