#include <stdio.h>
#include "module/condorcet.h"
#include "module/majority_judgment.h"
#include "module/single_member.h"
#include "interpreter.h"
#include "utils/csv_reader.h"
#include "logger.h"

void uni1(char* source_file) {
    Bale* bale = csvToBale(source_file);
    GenList* winners = theWinnerOneRound(bale);
    displayListWinnerSingle(winners);
}

void uni2(char* source_file) {
    Bale* bale = csvToBale(source_file);
    GenList* winners = theWinnerTwoRounds(bale);
    displayListWinnerSingleTwo(winners);
}

void minimax(char* source_file) {
    Duel* duel = csvToDuel(source_file);
    GenList* winners = theWinnerMinimax(duel);
    displayListWinnerCondorcet(winners, "minimax");
}

void rankedPairs(char* source_file) {
    Duel* duel = csvToDuel(source_file);
    GenList* winners = theWinnerRankedPairs(duel);
    displayListWinnerCondorcet(winners, "rangement des pairs");
}

void schulze(char* source_file) {
    Duel* duel = csvToDuel(source_file);
    GenList* winners = theWinnerSchulze(duel);
    displayListWinnerCondorcet(winners, "schulze");
}

void majorityJudgement(char* source_file) {
    Bale* bale = csvToBale(source_file);
    GenList* winners = theWinnerMajorityJudgement(bale);
    //displayListWinnerMajorityJudgement(winners);
}

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
    majorityJudgement(source_file);
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
            majorityJudgement(cmd->file_name);
            break;
        case ALL:
            all(cmd->file_name);
            break;
        default:
            exitl("main", "main", 1, "le Module renvoyé par l'interpréteur est invalide");
    }

    close_logger();

    return 0;
}