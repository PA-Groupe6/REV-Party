/**
 * @file majority_judgment.h
 * @author IVANOVA ALina 
 * @date 4/11/2023
 *
 * @brief Fichier d'en-tête pour le module Single-Member(Uninominal)
 *
 * Ce module implémente jugement majoritaire pour la structure de données bale.h
 *
 * La méthode du Jugement Majoritaire est un système de vote conçu pour l'évaluation des candidats sur une échelle de préférence.
 *
 * Cette méthode est particulièrement utile dans les processus de vote et de décision.
 *
 * Ce fichier d'en-tête fournit les fonctions et les structures nécessaires pour calculer les résultats selon la méthode de Jugement Majoritaire pour les données contenues dans bale.h.
 *
 *
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée, il est 
 * fortement recomandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur
 */


#ifndef __MAJORITY_JUDGEMENT_H__
#define __MAJORITY_JUDGEMENT_H__
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <bale.h>


/*------------------------------------------------------------------*/
/*                        MODULE MAJORIY JUDGMENT                   */
/*------------------------------------------------------------------*/



/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on cherche le candidat gagnant à partir des votes concetant les préférence d'électeurs
 *
 * @param[in] bale matrice des ballots fournis pour chaque candidat
 *
 * @return Le nom du candidat gagnant
*/
char theWinnerMajorityJudgment(Bale* bale);



#endif


