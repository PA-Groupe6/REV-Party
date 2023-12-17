#include <stdio.h>
#include <stdlib.h>
#include "module/condorcet.h"
#include "module/majority_judgment.h"
#include "module/single_member.h"
#include "interpreter.h"
#include "utils/csv_reader.h"
#include "logger.h"

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief libère la mémoire prise par une liste générique de candidats gagnants (le retour type d'une fonction module)
 * 
 * @param winners la liste à libérer
 */
void deleteWinners(ptrGenList* winners) {
    while (!genListEmpty(*winners))
        free(genListPop(*winners));
    deleteGenList(winners);
}

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief applique la méthode uni1 sur le fichier source_file et affiche son résultat pour l'utilisateur
 * 
 * @param source_file le fichier d'entrée fournit par l'utilisateur
 */
void uni1(char* source_file) {
    Bale* bale = csvToBale(source_file);
    GenList* winners = theWinnerOneRound(bale);
    deleteBale(&bale);
    displayListWinnerSingle(winners);
    deleteWinners(&winners);
}

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief applique la méthode uni2 sur le fichier source_file et affiche son résultat pour l'utilisateur
 * 
 * @param source_file le fichier d'entrée fournit par l'utilisateur
 */
void uni2(char* source_file) {
    Bale* bale = csvToBale(source_file);
    GenList* winners = theWinnerTwoRounds(bale);
    deleteBale(&bale);
    displayListWinnerSingleTwo(winners);
    deleteWinners(&winners);
}

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief applique la méthode minimax sur le fichier source_file et affiche son résultat pour l'utilisateur
 * 
 * @param source_file le fichier d'entrée fournit par l'utilisateur
 */
void minimax(char* source_file) {
    Duel* duel = csvToDuel(source_file);
    GenList* winners = theWinnerMinimax(duel);
    deleteDuel(&duel);
    displayListWinnerCondorcet(winners, "minimax");
    deleteWinners(&winners);
}

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief applique la méthode de rangement des pairs sur le fichier source_file et affiche son résultat pour l'utilisateur
 * 
 * @param source_file le fichier d'entrée fournit par l'utilisateur
 */
void rankedPairs(char* source_file) {
    Duel* duel = csvToDuel(source_file);
    GenList* winners = theWinnerRankedPairs(duel);
    deleteDuel(&duel);
    displayListWinnerCondorcet(winners, "rangement des pairs");
    deleteWinners(&winners);
}

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief applique la méthode de schulze sur le fichier source_file et affiche son résultat pour l'utilisateur
 * 
 * @param source_file le fichier d'entrée fournit par l'utilisateur
 */
void schulze(char* source_file) {
    Duel* duel = csvToDuel(source_file);
    GenList* winners = theWinnerSchulze(duel);
    deleteDuel(&duel);
    displayListWinnerCondorcet(winners, "schulze");
    deleteWinners(&winners);
}

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief applique la méthode du jugement majoritaire sur le fichier source_file et affiche son résultat pour l'utilisateur
 * 
 * @param source_file le fichier d'entrée fournit par l'utilisateur
 */
void majorityJudgment(char* source_file) {
    Bale* bale = csvToBale(source_file);
    GenList* winners = theWinnerMajorityJudgment(bale);
    deleteBale(&bale);
    deleteWinners(&winners);
    //displayListWinnerMajorityJudgment(winners);
}

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief applique dans l'ordre de définition toutes les méthodes de scrutins une à une en affichant à chaque fois le résultat
 * 
 * @param source_file le fichier d'entrée fournit par l'utilisateur
 */
void all(char* source_file) {
    printl("\n### uni1:\n\n");
    uni1(source_file);
    printl("\n### uni2:\n\n");
    uni2(source_file);
    printl("\n### minimax:\n\n");
    minimax(source_file);
    printl("\n### ranked pairs:\n\n");
    rankedPairs(source_file);
    printl("\n### schulze:\n\n");
    schulze(source_file);
    printl("\n### majority judgement:\n\n");
    majorityJudgment(source_file);
}

int main(int argc, char* argv[]) {
    init_logger(NULL);
    Command* cmd = interprete(argc, argv);
    close_logger();

    if (cmd->has_log_file) {
        init_logger(cmd->log_file);
    } else {
        init_logger(NULL);
    }

    switch(cmd->module) {
        case UNI1:
            uni1(cmd->file_name);
            break;
        case UNI2:
            uni2(cmd->file_name);
            break;
        case MINIMAX:
            minimax(cmd->file_name);
            break;
        case RANGEMENT:
            rankedPairs(cmd->file_name);
            break;
        case SCHULZE:
            schulze(cmd->file_name);
            break;
        case JUGEMENT_MAJORITAIRE:
            majorityJudgment(cmd->file_name);
            break;
        case ALL:
            all(cmd->file_name);
            break;
        default:
            exitl("main", "main", 1, "le Module renvoyé par l'interpréteur est invalide");
    }

    close_logger();

    free(cmd);

    return 0;
}