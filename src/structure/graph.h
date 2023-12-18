/**
 * @file graph.h
 * @author VALLAT Ugo
 * @brief Structure graph header
 *
 * @brief Un graph est une structure de données contenant tous les arcs (flèches) entres les
 * candidats pondérés par un poids (positif).
 *
 * @note les sommets du graph sont représentés par un indentifiant ( unsigned int in [0, nb_vertex[ ) et un label
 *
 * @note Taille maximum des labels = @ref MAX_LENGHT_LABEL
 *
 * @remark Les sommets son étiquetés avec les labels.
 *
 * @remark En cas d'erreur, toutes les fonctions du graph exit le progamme avec un
 * message d'erreur
 *
 * @remark Un arc (src,dest) est unique
 */

#ifndef __GRAPH_H__
#define __GRAPH_H__
#include "data_struct_utils.h"
#include "genericlist.h"
#include "matrix.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Valeur par défaut du poids dans la structure graph,
 * ne doit pas correspondre à un poids valide
 * 
 */
#define DEFAULT_WEIGHT -2


/**
 * @brief Structure de données contenant les informations relatives à un arc
 *
 */
typedef struct s_arc {
    unsigned int id_src;               /* numéro sommet origine */
    unsigned int id_dest;              /* numéro sommet destination */
    int weight;                        /* poids de l'arc */
} Arc;


/*------------------------------------------------------------------*/
/*                         STRUCTURE GRAPH                           */
/*------------------------------------------------------------------*/

/**
 * @date 5/11/2023
 * @brief Définition opaque de la structure Graph
 */
typedef struct s_graph Graph;
typedef Graph *ptrGraph;

/**
 * @date 5/11/2023
 * @brief Crée un graph dont les sommets sont appartiennent à [0, nb_vertex[
 *
 * @param[in] nb_vertex Nombre de sommets
 * @param[in] labels Liste des noms des sommets (taille maximale des etiquette : @ref
 * MAX_LENGHT_LABEL)
 *
 * @pre genericListSize(labels) == nb_vertex
 * @pre Forall x in labels, typeof(x) == char*
 *
 * @return pointeur vers le graph
 */
Graph *createGraph(unsigned int nb_vertex, GenList *labels);

/**
 * @date 5/11/2023
 * @brief Supprime le graph et ses données en libérant la mémoire
 *
 * @param[in] g pointeur vers le graph à supprimer
 * @pre g != NULL
 */
void deleteGraph(ptrGraph *g);

/**
 * @date 5/11/2023
 * @brief Ajoute un arc (origine, destination, poids)
 *
 * @param[in] g graph
 * @param[in] id_src identifiant origine arc
 * @param[in] id_dest identifiant destination de l'arc
 * @param[in] weight poids de l'arc
 * @pre id_src < nb_sommets && id_dest < nb_sommets
 * @pre g != NULL
 *
 */
void graphAdd(Graph *g, unsigned int id_src, unsigned int id_dest, int weight);


/**
 * @date 30/11/2023
 * @brief Change la valeur du poids d'un arc
 *
 * @param[in] g graph
 * @param[in] id_src identifiant origine arc
 * @param[in] id_dest identifiant destination de l'arc
 * @param[in] weight nouveau poids de l'arc
 * @pre id_src < nb_sommets && id_dest < nb_sommets
 * @pre g != NULL
 * @pre Exist (origine, destination)
 *
 */
void graphSetWeight(Graph *g, unsigned int id_src, unsigned int id_dest, int weight);

/**
 * @date 5/11/2023
 * @brief Supprime un arc (origine, destination) si il existe
 *
 * @param[in] g graph
 * @param[in] id_src identifiant origine arc
 * @param[in] id_dest identifiant destination de l'arc
 * @pre id_src < nb_sommets && id_dest < nb_sommets
 * @pre g != NULL
 *
 */
void graphRemove(Graph *g, unsigned int id_src, unsigned int id_dest);

/**
 * @date 5/11/2023
 * @brief Renvoie la valeur à la position (l,c) dans le graph
 *
 * @param[in] g graph
 * @param[in] id_src identifiant origine arc
 * @param[in] id_dest identifiant destination de l'arc
 * @pre id_src < nb_sommets && id_dest < nb_sommets
 * @pre g != NULL
 *
 * @return poids de l'arc, 0 si n'existe pas
 */
int graphGetWeight(Graph *g, unsigned int id_src, unsigned int id_dest);

/**
 * @date 5/11/2023
 * @brief Renvoie le nombre de sommets
 *
 * @param[in] g graph
 * @pre g != NULL
 *
 * @return nombre de sommets
 */
unsigned int graphNbVertex(Graph *g);

/**
 * @date 5/11/2023
 * @brief Renvoie le nombre d'arcs
 *
 * @param[in] g graph
 * @pre g != NULL
 *
 * @return nombre d'arcs
 */
unsigned int graphNbArc(Graph *g);

/**
 * @date 5/11/2023
 * @brief Ranvoie l'étiquette d'un sommet
 *
 * @param[in] g graph
 * @param[in] id identifiant du sommet
 * @pre g != NULL
 *
 * @return etiquette du sommet
 */
char *graphGetLabel(Graph *g, unsigned int id);


/**
 * @date 5/11/2023
 * @brief Renvoie les information d'un arc (id_src, id_dest) dans une structure
 ( @ref Arc )
 *
 * @param[in] g graph à lire
 * @param[in] id_src identifiant source
 * @param[in] id_dest identifiant destination
 * @pre id_src < nb_sommets && id_dest < nb_sommets
 * @pre g != NULL
 *
 * @return pointeur vers copie de l'arc, NULL si l'arc n'existe pas
 */
Arc *graphGetArc(Graph *g, unsigned id_src, unsigned id_dest);




/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/

/**
 * @date 5/11/2023
 * @brief Copie un graph
 *
 * @param[in] g Pointeur vers le graph source (à copier)
 * @pre g != NULL
 * @return Pointeur vers la copie
 */

Graph *graphCopy(Graph *g);

/**
 * @brief Transforme le graphe en une liste d'arc ( @ref Arc )
 *
 * @param g Graph
 * @pre g != NULL
 *
 * @return Liste générique des Arcs
 */
GenList *graphToList(Graph *g);

#define DESCENDING 0
#define ASCENDING 1

/**
 * @brief Transforme le graphe en une liste d'arc ( @ref Arc ) triée par orde de poids
 * croissant ( @ref ASCENDING ) ou décroissant ( @ref DESCENDING )
 *
 * @param g Graph
 * @param order Ordre de tri
 * @pre g != NULL
 * @pre order == @ref ASCENDING || order == @ref DESCENDING
 *
 * @return Liste générique des Arcs
 */
GenList *graphToSortedList(Graph *g, int order);

/**
 * @brief Test si l'ajoue d'un arc crée un cycle dans le graph
 *
 * @param g Graph
 * @param arc Arc à tester
 * @pre g != NULL
 * @pre arc != NULL
 * @return true si crée un cycle, false sinon
 */
bool graphIsMakingCycle(Graph *g, Arc *arc);


#endif