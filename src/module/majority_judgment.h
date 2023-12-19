/**
 * @file majority_judgment.h
 * @author IVANOVA ALina 
 * @date 16/12/2023
 *
 * @brief Fichier d'en-tête pour le module de Jugement Majoritaire
 *
 * Ce module implémente jugement majoritaire pour la structure de données bale.h
 *
 * La méthode du Jugement Majoritaire est un système de vote conçu pour l'évaluation des candidats sur une échelle de préférence.
 *
 * Cette méthode est particulièrement utile dans les processus de vote et de décision.
 *
 * Ce fichier d'en-tête fournit les fonctions et les structures nécessaires pour calculer les résultats selon la méthode de Jugement Majoritaire pour les données contenues dans un ballot donné
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
#include "../structure/bale.h"
#include "../structure/data_struct_utils.h"


/*------------------------------------------------------------------*/
/*                        MODULE MAJORIY JUDGMENT                   */
/*------------------------------------------------------------------*/



typedef struct s_winner_MajorityJudgment {
    char name[MAX_LENGHT_LABEL];
    int median;                    /* la médiane avec laquelle le vainqueur a gagné */
    float percent_inf;             /* pourcentage d'opposants */
    float percent_sup;             /* pourcentage de partisans */
} WinnerMajorityJudgment;


/**
 * @author IVANOVA ALina 
 * @date 04/11/2023
 * @brief on cherche le candidat gagnant à partir des votes concetant les préférence d'électeurs
 *
 * @param[in] bale matrice des ballots fournis pour chaque candidat
 *
 * @return Le nom du candidat gagnant
*/
GenList* theWinnerMajorityJudgment(Bale* bale, bool is_bale_judgment);



#endif


