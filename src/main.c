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
 * @param[in] winners la liste à libérer
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
 * @param[in] bale le ballot fournit par l'utilisateur en entrée
 */
void uni1(Bale* bale) {
    GenList* winners = theWinnerOneRound(bale);
    displayListWinnerSingle(winners);
    deleteWinners(&winners);
}

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief applique la méthode uni2 sur le fichier source_file et affiche son résultat pour l'utilisateur
 * 
 * @param[in] bale le ballot fournit par l'utilisateur en entrée
 */
void uni2(Bale* bale) {
    GenList* winners = theWinnerTwoRounds(bale);
    displayListWinnerSingleTwo(winners);
    deleteWinners(&winners);
}

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief applique la méthode minimax sur le fichier source_file et affiche son résultat pour l'utilisateur
 * 
 * @param[in] duel la matrice de duels fournie par l'utilisateur en entrée
 * @param[in] nb_voters nombre de votants (si il a été possible de l'extraire, NULL sinon)
 */
void minimax(Duel* duel) {
    GenList* winners = theWinnerMinimax(duel);
    displayListWinnerCondorcet(winners, "minimax");
    deleteWinners(&winners);
}

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief applique la méthode de rangement des pairs sur le fichier source_file et affiche son résultat pour l'utilisateur
 * 
 * @param[in] duel la matrice de duels fournie par l'utilisateur en entrée
 * @param[in] nb_voters nombre de votants (si il a été possible de l'extraire, NULL sinon)
 */
void rankedPairs(Duel* duel) {
    GenList* winners = theWinnerRankedPairs(duel);
    displayListWinnerCondorcet(winners, "rangement des pairs");
    deleteWinners(&winners);
}

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief applique la méthode de schulze sur le fichier source_file et affiche son résultat pour l'utilisateur
 * 
 * @param[in] duel la matrice de duels fournie par l'utilisateur en entrée
 * @param[in] nb_voters nombre de votants (si il a été possible de l'extraire, NULL sinon)
 */
void schulze(Duel* duel) {
    GenList* winners = theWinnerSchulze(duel);
    displayListWinnerCondorcet(winners, "schulze");
    deleteWinners(&winners);
}

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief applique la méthode du jugement majoritaire sur le fichier source_file et affiche son résultat pour l'utilisateur
 * 
 * @param[in] bale le ballot fournit par l'utilisateur en entrée
 */
void majorityJudgment(Bale* bale) {
    GenList* winners = theWinnerMajorityJudgment(bale);
    displayListWinnerMajorityJudgment(winners);
    deleteWinners(&winners);
}

/**
 * @date 18/12/2023
 * @author LAFORGE Mateo
 * @brief affiche les nombres de votants et de candidats à partir d'un ballot
 * 
 * @param[in] bale ballot source
 */
void printNumbersFromBale(Bale* bale) {
    printl("Nombre Votants: %d   -   Nombre Candidats: %d\n", baleNbVoter(bale), baleNbCandidat(bale));
}

/**
 * @date 18/12/2023
 * @author LAFORGE Mateo
 * @brief affiche les nombres de candidats à partir d'une matrice de duel et le nombre de votant si nb_voters != NULL
 * 
 * @param[in] duel matrice de duel source
 * @param[in] nb_voters nombre de votant extrait d'une conversion (peux être NULL si l'utilisateur a donné une matrice de duel)
 */
void printNumbersFromDuel(Duel* duel, unsigned* nb_voters) {
    if (nb_voters == NULL) {
        printl("Nombre Candidats: %d\n", duelNbCandidat(duel));
    } else {
        printl("Nombre Votants: %d   -   Nombre Candidats: %d\n", *nb_voters, duelNbCandidat(duel));
    }
}

/**
 * @date 15/12/2023
 * @author LAFORGE Mateo
 * @brief applique dans l'ordre de définition toutes les méthodes de scrutins une à une en affichant à chaque fois le résultat
 * 
 * @param[in] cmd la commande interprétée de l'utilisateur
 */
void all(Command* cmd) {
    switch (cmd->file_type) {
        case DUEL: {
            warnl("main", "all", "Une Matrice de duel à été passée en paramètre -> exécution des méthodes de Condorcet\n");
            Duel* duel = csvToDuel(cmd->file_name);

            printl(" -= Minimax =-\n");
            minimax(duel);
            printl(" -= Rangement Des Pairs =-\n");
            rankedPairs(duel);
            printl(" -= Schulze =-\n");
            schulze(duel);

            printNumbersFromDuel(duel, NULL);
            deleteDuel(&duel);
            break;
        }
        case BALE: {
            Bale* bale = csvToBale(cmd->file_name);

            printl(" -= Uni1 =-\n");
            uni1(bale);
            printl(" -= Uni2 =-\n");
            uni2(bale);

            Duel* duel = duelFromBale(bale);

            printl(" -= Minimax =-\n");
            minimax(duel);
            printl(" -= Rangement Des Pairs =-\n");
            rankedPairs(duel);
            printl(" -= Schulze =-\n");
            schulze(duel);

            printl(" -= Jugment Majoritaire =-\n");
            majorityJudgment(bale);

            deleteDuel(&duel);
            printNumbersFromBale(bale);
            deleteBale(&bale);
            break;
        }
        default:
            exitl("main", "main", 1, "le type de fichier renvoyé par l'interpréteur est invalide\n");
    }
}

/**
 * @date 17/12/2023
 * @author LAFORGE Mateo
 * @brief récupère une structure Duel soit à partir d'une matrice de duel soit à partir d'un ballot
 * 
 * @param[in] cmd commande permettant la récupération des informations pour la construction
 * @param[out] nb_voters nombre de votants modifier si il à été possible de l'extraire (cas où cmd donne un ballot)
 * 
 * @return la matrice de duel construite
 */
Duel* getDuel(Command* cmd, unsigned* nb_voters) {
    Duel* duel;
    switch (cmd->file_type) {
        case DUEL:
            duel = csvToDuel(cmd->file_name);
            break;
        case BALE: {
            warnl("main.c", "getDuel",
                "conversion d'une matrice de duel en ballot\n");
            Bale* bale = csvToBale(cmd->file_name);
            duel = duelFromBale(bale);
            // assigne nb_voters si possible
            if (nb_voters != NULL) *nb_voters = baleNbVoter(bale);
            deleteBale(&bale);
            break;
        }
        default:
            exitl("main", "main", 1, "le type de fichier renvoyé par l'interpréteur est invalide\n");
    }
    return duel;
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
        case UNI1: {
            Bale* bale = csvToBale(cmd->file_name);
            uni1(bale);
            printNumbersFromBale(bale);
            deleteBale(&bale);
            break;
        }
        case UNI2: {
            Bale* bale = csvToBale(cmd->file_name); 
            uni2(bale);
            printNumbersFromBale(bale);
            deleteBale(&bale);
            break;
        }
        case MINIMAX: {
            unsigned nb_voters;
            Duel* duel = getDuel(cmd, &nb_voters);
            minimax(duel);
            printNumbersFromDuel(duel, &nb_voters);
            deleteDuel(&duel);
            break;
        }
        case RANGEMENT: {
            unsigned nb_voters;
            Duel* duel = getDuel(cmd, &nb_voters);
            rankedPairs(duel);
            printNumbersFromDuel(duel, &nb_voters);
            deleteDuel(&duel);
            break;
        }
        case SCHULZE: {
            unsigned nb_voters;
            Duel* duel = getDuel(cmd, &nb_voters);
            schulze(duel);
            printNumbersFromDuel(duel, &nb_voters);
            deleteDuel(&duel);
            break;
        }
        case JUGEMENT_MAJORITAIRE: {
            Bale* bale = csvToBale(cmd->file_name);
            majorityJudgment(bale);
            printNumbersFromBale(bale);
            deleteBale(&bale);
            break;
        }
        case ALL:
            all(cmd);
            break;
        default:
            exitl("main", "main", 1, "le Module renvoyé par l'interpréteur est invalide\n");
    }

    close_logger();

    free(cmd);

    return 0;
}