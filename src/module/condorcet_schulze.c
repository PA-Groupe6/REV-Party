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
#include <errno.h>
#include "../structure/duel.h"
#include <string.h>
#include <malloc.h>
#include "../structure/genericlist.h"
#include "../structure/graph.h"
#include "condorcet.h"
#include "../structure/data_struct_utils.h"






/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on utilise la méthode de Schulze pour trouver le candidat gagnant
 *
 * @param[in] duel matrice des duels entre tous le candidats
 *
 * @return Le gagnant en utilisant la structure WinnerCondorcet.
*/
GenList* theWinnerSchulze(Duel* duel);