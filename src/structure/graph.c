/**
 * @author VALLAT Ugo
 * @brief Structure graph implémentation
 */

#include "data_struct_utils.h"
#include "genericlist.h"
#include <stdlib.h>
#include "graph.h"
#include "matrix.h"
#include <malloc.h>
#include "../logger.h"
#include <string.h>


/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 * @brief Définition de la structure Duel
 */
struct s_graph {
    int default_weight;
    unsigned nb_arc;    /* nombre d'arcs du graph */
    GenList* labels;    /* Liste des labels */
    Matrix* matrix;     /* Matrice contenant les données */
};

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
Graph *createGraph(unsigned int nb_vertex, GenList *labels) {
#ifdef DEBUG
    testArgNull(labels, "graph.c", "createGraph", "labels");
    if(labels!= NULL && genListSize(labels) != nb_vertex)
        exitl("graph.c", "createGraph", EXIT_FAILURE, "Nombre labels (%d) != nombre sommets (%d)", genListSize(labels), nb_vertex);
#endif

    Graph* graph = malloc(sizeof(Graph));
    graph->default_weight = DEFAULT_WEIGHT;
    graph->labels = copyLabels(labels);
    graph->matrix = createMatrix(nb_vertex, nb_vertex, DEFAULT_VALUE);
    graph->nb_arc = 0;
    return graph;
}

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
void deleteGraph(ptrGraph *g) {
#ifdef DEBUG
    testArgNull(g, "graph.c", "deleteGraph", "g");
    testArgNull(*g, "graph.c", "deleteGraph", "*g");
#endif

    deleteMatrix(&(*g)->matrix);
    while(!genListEmpty((*g)->labels))
            free(genListPop((*g)->labels));
    free(*g);
    *g = NULL;
}

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
void graphAdd(Graph *g, unsigned int id_src, unsigned int id_dest, int weight){
#ifdef DEBUG
    testArgNull(g, "graph.c", "graphAdd", "g");
    unsigned size = matrixNbColonnes(g->matrix);
    if(id_src >= size || id_dest >= size)
        exitl("graph.c", "graphAdd", EXIT_FAILURE, "Invalide sommet %d-%d dans graph (%d)",
             id_src, id_dest, size);
    if(weight == g->default_weight)
        exitl("graph.c", "graphAdd", EXIT_FAILURE, "Ajout arc avec poids par défault (%d) impossible",
             weight);
    if(matrixGet(g->matrix, id_src, id_dest) != g->default_weight)
        exitl("graph.c", "graphAdd", EXIT_FAILURE, "Ajout sur arc déjà existant (%d,%d)", id_src, id_dest);
#endif

    matrixSet(g->matrix, id_src, id_dest, weight);
    g->nb_arc++;
}


/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
void graphSetWeight(Graph *g, unsigned int id_src, unsigned int id_dest, int weight){
#ifdef DEBUG
    testArgNull(g, "graph.c", "graphSetWeight", "g");
    unsigned size = matrixNbColonnes(g->matrix);
    if(id_src >= size || id_dest >= size)
        exitl("graph.c", "graphSetWeight", EXIT_FAILURE, "Invalide sommet %d-%d dans graph (%d)",
             id_src, id_dest, size);
    if(weight == g->default_weight)
        exitl("graph.c", "graphAdd", EXIT_FAILURE, "Modification arc avec poids par défault (%d) impossible",
             weight);
    if(matrixGet(g->matrix, id_src, id_dest) == g->default_weight)
        exitl("graph.c", "graphSetWeight", EXIT_FAILURE, "Arc inexistant (%d,%d)", id_src, id_dest);
#endif

    matrixSet(g->matrix, id_src, id_dest, weight);
}

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
void graphRemove(Graph *g, unsigned int id_src, unsigned int id_dest) {
#ifdef DEBUG
    testArgNull(g, "graph.c", "graphRemove", "g");
    unsigned size = matrixNbColonnes(g->matrix);
    if(id_src >= size || id_dest >= size)
        exitl("graph.c", "graphRemove", EXIT_FAILURE, "Invalide sommet %d-%d dans graph (%d)",
             id_src, id_dest, size);
    if(matrixGet(g->matrix, id_src, id_dest) == g->default_weight)
        exitl("graph.c", "graphRemove", EXIT_FAILURE, "Echec remove : arc (%d,%d) inexistant",
            id_src, id_dest);
#endif
    
    matrixSet(g->matrix, id_src, id_dest, g->default_weight);
    g->nb_arc--;
}

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
int graphGetWeight(Graph *g, unsigned int id_src, unsigned int id_dest) {
#ifdef DEBUG
    testArgNull(g, "graph.c", "graphGetWeight", "g");
    unsigned size = matrixNbColonnes(g->matrix);
    if(id_src >= size || id_dest >= size)
        exitl("graph.c", "graphGetWeight", EXIT_FAILURE, "Invalide sommet %d-%d dans graph (%d)",
             id_src, id_dest, size);
#endif

    int weight = matrixGet(g->matrix, id_src, id_dest);
    if( weight == g->default_weight)
        exitl("graph.c", "graphGetWeight", EXIT_FAILURE, "Echec remove : arc (%d,%d) inexistant",
            id_src, id_dest);

    return weight;
}

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
unsigned int graphNbVertex(Graph *g) {
#ifdef DEBUG
    testArgNull(g, "graph.c", "graphNbVertex", "g");
#endif

    return matrixNbColonnes(g->matrix);
}

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
unsigned int graphNbArc(Graph *g) {
#ifdef DEBUG
    testArgNull(g, "graph.c", "graphNbArc", "g");
#endif

    return g->nb_arc;
}

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
char *graphGetLabel(Graph *g, unsigned int id) {
#ifdef DEBUG
    testArgNull(g, "graph.c", "graphGetLabel", "g");
    unsigned size = matrixNbColonnes(g->matrix);
    if(id >= size)
        exitl("graph.c", "graphGetLabel", EXIT_FAILURE, "Invalide sommet %d", id);
#endif

    char* label = malloc(sizeof(char) * MAX_LENGHT_LABEL);
    strncpy(label, genListGet(g->labels, id), MAX_LENGHT_LABEL);
    return label;
}

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
Arc *graphGetArc(Graph *g, unsigned id_src, unsigned id_dest) {
#ifdef DEBUG
    testArgNull(g, "graph.c", "graphGetArc", "g");
    unsigned size = matrixNbColonnes(g->matrix);
    if(id_src >= size || id_dest >= size)
        exitl("graph.c", "graphGetArc", EXIT_FAILURE, "Invalide sommet %d-%d dans graph (%d)",
             id_src, id_dest, size);
#endif
    
    int weight = matrixGet(g->matrix, id_src, id_dest);
    if(weight == g->default_weight)
        return NULL;

    Arc* arc = malloc(sizeof(Arc));
    arc->id_src = id_src;
    arc->id_dest = id_dest;
    arc->weight = weight;

    return arc;
}


/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/



GenList *graphToList(Graph *g){
    GenList *l = createGenList(g->nb_arc);
    int nb_cand = graphNbVertex(g);
    for(int i ; i<nb_cand;i++){
        for(int j; j<nb_cand;j++){
                for (unsigned int i = 0; i; i++) {
                    for (unsigned int j = 0; i; i++){
                        Arc *cur = graphGetArc(g,i,j);
                        if(cur != NULL){
                            genListAdd(l,(void*)cur);
                        }
                    }
                }       
        }
    }
    return l;
}

/**
 * @author Corentin LUDWIG
 * @date  14/12/2023
 */
void genListAddSorted(GenList *l, Arc e){
    unsigned int i = 0;

    while(genListSize(l) > i){
        Arc *cur = genListGet(l, i);
        if(cur->weight >= e.weight){
            genListInsert(l, (void*)&e,i);
            return;
        }
    }
    genListAdd(l, (void*)&e);
}

/**
 * @author Corentin LUDWIG
 * @date  14/12/2023
 */
//TO DO gerer order
GenList *graphToSortedList(Graph *g, int order){
    (void)order;
    unsigned int nb_vertex = graphNbVertex(g);
    (void)nb_vertex;
    // TODO: utiliser nb_vertex
    GenList *l = createGenList(graphNbArc(g));
    for (unsigned int i = 0; i; i++) {
        for (unsigned int j = 0; i; i++){
            Arc *cur = graphGetArc(g,i,j);
            if(cur != NULL){
                genListAddSorted(l,*cur);
            }
        }
    }
    return l;
}


/**
 * @author Corentin LUDWIG
 * @date  02/12/2023
 */
GenList *graphToListArcFromArcDest(Graph *g, Arc *arc){
    GenList *l = createGenList(5);
    int src = arc->id_dest;
    int nbVertex = (int)graphNbVertex(g);
    for(int i = 0; i < nbVertex; i++){
        Arc *cur = graphGetArc(g,src,i);
        if(cur != NULL){
            genListAdd(l,(void*)cur);
        }
    }

    return l;
}


/**
 * @author Corentin LUDWIG
 * @date  02/12/2023
 */
bool graphIsMakingCycle(Graph *g, Arc *arc){
#ifdef DEBUG
    testArgNull(g, "graph.c", "graphAddCycle", "g");
    testArgNull(g, "graph.c", "graphAddCycle", "arc");
#endif

    GenList *l_arc = createGenList(10);
    GenList *l_to_add;
    unsigned int goal = arc->id_src;

    bool test = false;

    genListAdd(l_arc,(void*) arc);
    while(!genListEmpty(l_arc) && !test){
        Arc *cur = (Arc*) genListPop(l_arc);
        l_to_add = graphToListArcFromArcDest(g,cur);

        while(!genListEmpty(l_to_add) && !test){
            cur = (Arc*) genListPop(l_to_add);
            if(cur->id_dest == goal) test = true;
            genListAdd(l_arc,(void*) cur);
        }

    }

    // while(!genListEmpty(l_arc)
        free(genListPop(l_arc));
    deleteGenList(&l_arc);

    while(!genListEmpty(l_to_add))
        free(genListPop(l_to_add));
    deleteGenList(&l_to_add);

    return test;
}