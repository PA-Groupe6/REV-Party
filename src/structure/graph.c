/**
 * @author VALLAT Ugo
 * @brief Structure graph implémentation
 */

#include "bale.h"
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
    testArgNull(labels, "graph.c", "createGraph", "labels");
    if(labels!= NULL && genListSize(labels) != nb_vertex)
        exitl("graph.c", "createGraph", EXIT_FAILURE, "Nombre labels (%d) != nombre sommets (%d)", genListSize(labels), nb_vertex);

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
    testArgNull(g, "graph.c", "deleteGraph", "g");
    testArgNull(*g, "graph.c", "deleteGraph", "*g");

    deleteMatrix(&(*g)->matrix);
    char* label;
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
    
    matrixSet(g->matrix, id_src, id_dest, weight);
    g->nb_arc++;
}


/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
void graphSetWeight(Graph *g, unsigned int id_src, unsigned int id_dest, int weight){
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
    
    matrixSet(g->matrix, id_src, id_dest, weight);
}

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
void graphRemove(Graph *g, unsigned int id_src, unsigned int id_dest) {
    testArgNull(g, "graph.c", "graphRemove", "g");
    unsigned size = matrixNbColonnes(g->matrix);
    if(id_src >= size || id_dest >= size)
        exitl("graph.c", "graphRemove", EXIT_FAILURE, "Invalide sommet %d-%d dans graph (%d)",
             id_src, id_dest, size);
    if(matrixGet(g->matrix, id_src, id_dest) == g->default_weight)
        exitl("graph.c", "graphRemove", EXIT_FAILURE, "Echec remove : arc (%d,%d) inexistant",
            id_src, id_dest);
    
    matrixSet(g->matrix, id_src, id_dest, g->default_weight);
    g->nb_arc--;
}

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
int graphGetWeight(Graph *g, unsigned int id_src, unsigned int id_dest) {
    testArgNull(g, "graph.c", "graphGetWeight", "g");
    unsigned size = matrixNbColonnes(g->matrix);
    if(id_src >= size || id_dest >= size)
        exitl("graph.c", "graphGetWeight", EXIT_FAILURE, "Invalide sommet %d-%d dans graph (%d)",
             id_src, id_dest, size);
    
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
    testArgNull(g, "graph.c", "graphNbVertex", "g");
    return matrixNbColonnes(g->matrix);
}

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
unsigned int graphNbArc(Graph *g) {
    testArgNull(g, "graph.c", "graphNbArc", "g");
    return g->nb_arc;
}

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
char *graphGetLabel(Graph *g, unsigned int id) {
    testArgNull(g, "graph.c", "graphGetLabel", "g");
    unsigned size = matrixNbColonnes(g->matrix);
    if(id >= size)
        exitl("graph.c", "graphGetLabel", EXIT_FAILURE, "Invalide sommet %d", id);

    char* label;
    strncpy(label, genListGet(g->labels, id), MAX_LENGHT_LABEL);
    return label;
}

/**
 * @author Ugo VALLAT
 * @date  13/11/2023
 */
Arc *graphGetArc(Graph *g, unsigned id_src, unsigned id_dest) {
    testArgNull(g, "graph.c", "graphGetArc", "g");
    unsigned size = matrixNbColonnes(g->matrix);
    if(id_src >= size || id_dest >= size)
        exitl("graph.c", "graphGetArc", EXIT_FAILURE, "Invalide sommet %d-%d dans graph (%d)",
             id_src, id_dest, size);
    
    int weight = matrixGet(g->matrix, id_src, id_dest);
    if(weight == g->default_weight)
        return NULL;

    Arc* arc = malloc(sizeof(Arc));
    arc->id_src = id_src;
    arc->id_dest = id_dest;
    arc->weight = weight;

    return arc;
}