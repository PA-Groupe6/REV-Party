/**
 * @file condorcet_ranked_pairs.c
 * @author IVANOVA ALina 
 * @date 20/11/2023
 *
 * @brief Fichier d'implementation de la méthode Condorcet avec le rangement des paires par ordre décroissant  
 *
 * Ce module implémente la méthode de Condorcet pour la structure de données duel.h
 *
 change!!!!!!!!!!!!
 * La méthode de Condorcet Minimax est utilisée pour déterminer les préférences collectives d'un ensemble de choix en se basant sur les préférences individuelles des votants. 
 * Le gagnant est celui qui minimise le nombre maximum de défaites.
 * Cette méthode est particulièrement utile dans les processus de vote et de décision.
 *
 * Ce fichier d'en-tête fournit les fonctions et les structures nécessaires pour calculer les résultats selon la méthode de Condorcet pour les données contenues dans duel.h.
 *
 *
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée, il est 
 * fortement recomandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur
 */
