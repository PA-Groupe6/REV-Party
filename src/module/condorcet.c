#include <string.h>
#include <malloc.h>
#include "condorcet.h"
#include "../structure/graph.h"
#include "../structure/data_struct_utils.h"

/****************
*   VAINQUEUR   *
*****************/


/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on utilise la méthode vainqueur de Condorcet pour le trouver ou savoir si on doit implementer un autre méthode
 * 
 * @param[in] duel matrice des duels entre tous le candidats
 *
 * @return Le gagnant en utilisant la structure WinnerCondorcet ou NULL s'il n'y a pas de vaiqueur
*/
WinnerCondorcet* CondorcetWinnerCriterion(Duel* duel) {
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
    strncpy(vainqueur->name, winner_name, MAX_LENGHT_LABEL);
    free(winner_name);
    vainqueur->score = maxWins;
    return vainqueur;
}



/**************
*   MINIMAX   *
***************/

/**
 * @author Alina IVANOVA
 * @date 20/11/2023 
 * @brief creation d'une liste avec le(s) gagnant(s) en utilisant la méthode Minimax
 */
GenList* miniMaxCandidat(Duel* duel){
    int nbCandidats= duelNbCandidat(duel);
    int miniDifference;
    GenList* candidates = createGenList(1);
    for (int cand1 = 0; cand1<nbCandidats;cand1++){
        int maxDiffCandidat = 0;
        for (int cand2 = 0; cand2 < nbCandidats; cand2++){
            int diff = duelGetValue(duel, cand1, cand2); 
            if(diff > maxDiffCandidat){
                maxDiffCandidat = diff;
            }
        }
        
        if (cand1 == 0 || miniDifference>maxDiffCandidat){
            while(genListSize(candidates)!=0) free((WinnerCondorcet*)genListPop(candidates));

            miniDifference = maxDiffCandidat;
            
            WinnerCondorcet* cand_possible = malloc(sizeof(WinnerCondorcet));
            cand_possible->score = maxDiffCandidat;
            char* winner_name =  duelIndexToLabel(duel, cand1);
            strncpy(cand_possible->name, winner_name, MAX_LENGHT_LABEL);
            free(winner_name);
            genListAdd(candidates, cand_possible);
        }
        else if(miniDifference == maxDiffCandidat){
            WinnerCondorcet* another_cand = malloc(sizeof(WinnerCondorcet));
            another_cand->score = maxDiffCandidat;
            char* winner_name =  duelIndexToLabel(duel, cand1);
            strncpy(another_cand->name, winner_name, MAX_LENGHT_LABEL);
            free(winner_name);
            genListAdd(candidates, another_cand);
        }
    }
    return candidates;
}

/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on utilise la méthode condorcet minimax pour trouver le candidat gagnant
 * 
 * @param[in] duel matrice des duels entre tous le candidats
 *
 * @return Le gagnant en utilisant la structure WinnerCondorcet.
*/
GenList* theWinnerMinimax(Duel* duel){
    GenList* winners;
    WinnerCondorcet* wtmp;

    wtmp = CondorcetWinnerCriterion(duel);
    if (wtmp==NULL) {
        winners = miniMaxCandidat(duel);
    }
    else {
        winners = createGenList(1);
        genListAdd(winners,wtmp);
    }
    return winners;
}

/*******************
*   RANKED PAIRS   *
*******************/

/**
 * @date 11/12/2023
 * @author Corentin LUDWIG
 * @brief trie la liste generique
 * @remark utilise le trie a bulle
 */
void sortGenList(GenList* arcs){
    int i, j;
    Arc *a1,*a2;
    int size = genListSize(arcs);
    for (i = 0; i < size; i++) {
        for (j = 0; j < size - i-1; j++) {
            a1 = (Arc*)genListGet(arcs, j);
            a2 = (Arc*)genListGet(arcs, j+1);
            if (a1->weight < a2->weight) {
                genListSet(arcs, (void*) a1, j+1);
                genListSet(arcs, (void*) a2, j);
            }
        }
    }
}


/**
 * @date 08/12/2023
 * @author Alina IVANOVA
 * @brief creation de list des arcs triés
 */
GenList* sortedArcsCreate(Duel* duel){
    //creating a graph
    int nb_cand = duelNbCandidat(duel) ;
    GenList* arcs = createGenList(nb_cand*2);

    for (int i = 0; i< nb_cand ; i++){
        for (int j = i+1; j<nb_cand; j++){  
            //adding the arc or duel to the graph
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
    sortGenList(arcs);
    return arcs;
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

    GenList* arcs_list = sortedArcsCreate(duel);
    int nb_arcs = genListSize(arcs_list);

    Graph* winner_graph = createGraph(nb_cand, labels_graph);

    while(!genListEmpty(labels_graph))
        free(genListPop(labels_graph));
    deleteGenList(&labels_graph);

    for (int i = 0; i< nb_arcs; i++){
        Arc* arc_current = genListGet(arcs_list, i);
        if(!graphIsMakingCycle(winner_graph, arc_current)){
            int src = arc_current->id_src;
            int dest = arc_current->id_dest;
            int weiht = arc_current->weight;
            graphAdd(winner_graph,src ,dest , weiht);
        }
    }
    while(!genListEmpty(arcs_list))
        free(genListPop(arcs_list));
    deleteGenList(&arcs_list);

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


    int nb_cand = duelNbCandidat(duel) ;
    int nb_arcs = graphNbArc(graph);
    int wins_arcs[nb_cand];

    deleteGraph(&graph);

    for(int i = 0; i<nb_cand; i++) wins_arcs[i] = 0;
    Arc* arc_current;
    int src_id;
    for(int i = 0; i<nb_arcs; i++){
        arc_current =(Arc*) genListGet(arcs_sorted, i);
        src_id = arc_current->id_src;
        wins_arcs[src_id]++;
    }
    while(!genListEmpty(arcs_sorted))
        free(genListPop(arcs_sorted));
    deleteGenList(&arcs_sorted);
    
    int max_winnings = 0;
    
    GenList* winners = createGenList(nb_cand);

    for(int i = 0; i < nb_cand; i++){
        if(wins_arcs[i]>max_winnings){
            while(genListSize(winners)!=0) free((WinnerCondorcet*)genListPop(winners));
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
    GenList* winners;

    WinnerCondorcet* winner = CondorcetWinnerCriterion(duel);

    if (winner==NULL) {
        free(winner);
        winners = findWinnerGraph(duel);
    }
    else {
        winners = createGenList(1);
        genListAdd(winners,winner);
    }
    return winners;

}


/**************
*   SCHULZE   *
**************/


/**
 * @date 16/12/2023
 * @author Alina IVANOVA
 * @brief retourne la valeur maximale
 */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/**
 * @date 16/12/2023
 * @author Alina IVANOVA
 * @brief retourne la valeur minimale
 */
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

    for(int cand_3 = 0; cand_3<nb_cand; cand_3++){
        for(int cand_2 = 0;cand_2<nb_cand; cand_2++){
            
            if (cand_2!=cand_3){
                for(int cand_1 = 0; cand_1 < nb_cand; cand_1++){
                    
                    if(cand_3!=cand_1 && cand_1!=cand_2){

                        cand_2vs1 = graphGetWeight(graph_for_search, cand_2, cand_1);
                        cand_2vs3 = graphGetWeight(graph_for_search, cand_2, cand_3);
                        cand_3vs1 = graphGetWeight(graph_for_search, cand_3, cand_1);
                        
                        int weight = max(cand_2vs1, min(cand_2vs3, cand_3vs1));

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
GenList* findWinnerGraphPath(Duel* duel){
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
            if(win >= loss)winsCandidate++;
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
        winners = findWinnerGraphPath(duel);
    }
    else {
        winners = createGenList(1);
        genListAdd(winners,winner_condorcet);
    }
    return winners;
}