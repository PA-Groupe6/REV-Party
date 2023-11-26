/**
 * @file graph.h
 * @author VALLAT Ugo
 * @brief Structure graph header
 *
 * @brief Un graph est une structure de données contenant tous les arcs (flèches) entres les
 * candidats pondérés par un poids (positif).
 *
 * @note les sommets du graph sont représentés par un indentifiant (unsigned int) et un label
 *
 * @note Taille maximum des labels = @ref MAX_LENGHT_LABEL
 *
 * @remark Les lignes et les colonnes son étiquetée avec le nom du candidat.
 *
 * @remark En cas d'erreur, toutes les fonctions du graph exit le progamme avec un
 * message d'erreur
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
 * @brief Structure de données contenant les informations relatives à un arc
 *
 */
typedef struct s_arc {
    char label_src[MAX_LENGHT_LABEL];  /* etiquette du sommet origine */
    char label_dest[MAX_LENGHT_LABEL]; /* etiquette du sommet destination */
    unsigned int id_src;               /* numéro sommet origine */
    unsigned int id_dest;              /* numéro sommet destination */
    unsigned int weight;               /* poids de l'arc */
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
 * @brief Crée un graph
 *
 * @param[in] nb_vertex Nombre de sommets
 * @param[in] labels Liste des noms des sommets (taille maximale des etiquette : @ref
 * MAX_LENGHT_LABEL)
 *
 * @pre genericListSize(labels) == nbc && Forall x in labels, typeof(x) == char*
 *
 * @return pointeur vers le graph
 * @note labels vaut NULL si non définis
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
void graphAdd(Graph *g, unsigned int id_src, unsigned int id_dest, unsigned int weight);

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
unsigned int graphGetWeight(Graph *g, unsigned int id_src, unsigned int id_dest);

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
 * @brief Renvoie l'identifiant du sommet associé à l'étiquette
 *
 * @param[in] g graph
 * @param[in] label  Etiquette du sommet
 * @pre g != NULL
 * @pre exist label
 *
 * @return identifiant du sommet
 */
unsigned int graphGetId(Graph *g, char *label);

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
 * @return pointeur vers l'arc ( @ref Arc), NULL si l'arc n'existe pas
 */
Arc *graphGetArc(Graph *g, unsigned int id_src, char *id_dest);

/*------------------------------------------------------------------*/
/*                          ITERATEUR                               */
/*------------------------------------------------------------------*/

/**
 * @date 5/11/2023
 * @brief Définition opaque de la structure GraphIte
 */
typedef struct s_graphIte GraphIte;
typedef struct s_graphIte *ptrGraphIte;

/**
 * @date 5/11/2023
 * @brief Définie le type des fonctions prise en paramêtre de l'itérateur
 *
 * @param[in] wieght Poids de l'
 * @param[in] src sommet origine
 * @param[in] dest sommet destination
 * @param[in] buff pointeur vers un buffer pouvant stocker un résultat
 */
typedef unsigned int (*fun_ite_Graph)(unsigned int weight, unsigned int src, unsigned int dest,
                                      void *buff);

/**
 * @date 5/11/2023
 * @brief Crée un itérateur sur le graph passé en entrée et le positionne
 * avant le premier élément
 *
 * @param[in] g graph à parcourir
 * @param[in] fun Fonction à appliquer à chaque élément, si NULL aucun traitement
 * n'est appliqué et le graph n'est pas modifiée
 * @param[in] buff pointeur vers le buffer fourni à fun
 * @pre g != NULL
 *
 * @return Pointeur vers l'itérateur
 */
GraphIte *createGraphIte(Graph *g, fun_ite_Graph fun, void *buff);

/**
 * @date 5/11/2023
 * @brief Renvoie vrai si il reste des éléments à parcourir
 *
 * @param[in] ite Itérateur questionné
 * @pre ite != NULL
 *
 * @return true si il reste des éléments, sinon false
 */
bool graphIteHasNext(GraphIte *ite);

/**
 * @date 5/11/2023
 * @brief Décale l'itérateur sur le prochain élément, renvoie sa valeur, réalise
 *  le traitement de fun et remplace la valeur courante par le retour de fun
 *
 * @param[in] ite Itérateur à utiliser
 * @pre GraphIteHasNext(ite) == true
 * @pre ite != NULL
 *
 * @return Valeur courante
 */
unsigned int graphIteNext(GraphIte *ite);

/**
 * @date 5/11/2023
 * @brief Renvoie la valeur courrante
 *
 * @param[in] ite Itérateur à utiliser
 * @pre Appel à next avant
 * @pre ite != NULL
 *
 * @return Valeur de l'élément de courant
 */
unsigned int graphIteGetValue(GraphIte *ite);

/**
 * @date 5/11/2023
 * @brief Supprime l'itérateur et libère la mémoire
 *
 * @param[in] ite pointeur vers l'itérateur
 * @pre ite != NULL
 * @pre *ite != NULL
 * @return adresse du buffer
 */
void deleteGraphIte(ptrGraphIte *ite);

/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/

/**
 * @date 5/11/2023
 * @brief Applique la fonction fun à tous les éléments du graph
 *
 * @param[in] g graph à parcourir
 * @param[in] fun Fonction à appliquer à chaque élément, si NULL aucun traitement
 * n'est effectué
 * @param[in] buff pointeur vers le buffer fourni à fun
 * @pre g != NULL
 */
void graphMap(Graph *g, fun_ite_Graph fun, void *buff);

/**
 * @date 5/11/2023
 * @brief Renvoie le (ou les) arc(s) de poids maximum
 *
 * @param[in] g Pointeur vers le graph
 * @pre g != NULL
 *
 * @return Liste générique des arcs de poids maximum
 */
GenList *graphMax(Graph *g);

/**
 * @date 5/11/2023
 * @brief Renvoie le (ou les) arc(s) de poids minimum
 *
 * @param[in] g Pointeur vers le graph
 * @pre g != NULL
 *
 * @return Liste générique des arcs de poids minimum
 */
GenList *graphMin(Graph *g);

/**
 * @date 5/11/2023
 * @brief Définie le type des fonctions prise en paramêtre du filter
 */
typedef fun_filter_matrix fun_filter_graph;

/**
 * @date 5/11/2023
 * @brief Filtre le graph et renvoie une copie du graph avec uniquement
 * les arcs tel que fun(arc) = true
 *
 * @param[in] d Pointeur vers le graph
 * @param[in] fun Fonction de filtrage de type fun_filter
 * @param[in] buff Buffer pour fun_filter
 * @pre g != NULL
 *
 * @return Pointeur vers la copie filtrée
 */
Graph *graphFilter(Graph *g, fun_filter_graph fun, void *buff);

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
 * @brief Renvoie la liste des arcs sortants d'un sommet
 *
 * @param g Graph
 * @pre g != NULL
 * @return Liste générique des arcs sortants
 */
GenList *graphToListArcFromVertex(Graph *g);

/**
 * @brief Renvoie la liste des arcs entrants d'un sommet
 *
 * @param g Graph
 * @pre g != NULL
 * @return Liste générique des arcs entrants
 */
GenList *graphToListArcToVertex(Graph *g);

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

/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

void printGraphLog(Graph *g);

#endif

#endif