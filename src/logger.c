#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <execinfo.h>
#include "logger.h"
#include "module/condorcet.h"
#include "structure/bale.h"
#include "structure/duel.h"
#include "structure/list.h"
#include "structure/matrix.h"
#include <errno.h>
#include <string.h>
#include "module/majority_judgment.h"

#define YELLOW "\033[38;5;184m"
#define ORANGE "\033[38;5;208m"
#define RED "\033[38;5;160m"
#define RSTC "\033[0m"

/* nombre caractères pour affichage INT */
#define SIZE_INT_DISPLAY 3

FILE* output = NULL;
bool console;
char* c_yellow;
char* c_orange;
char* c_rstc;

/**
 * @date 04/11/2023
 * @author LAFORGE Mateo
 */
void init_logger(const char* file_path) {
    if (file_path) {
        output = fopen(file_path, "a");
        console = false;
        c_yellow = "";
        c_orange = "";
        c_rstc = "";
    } else {
        output = stdout;
        console = true;
        c_yellow = YELLOW;
        c_orange = ORANGE;
        c_rstc = RSTC;
    }
}

/**
 * @date 04/11/2023
 * @author LAFORGE Mateo
 */
void printl(const char* format, ...) {
#ifdef DEBUG
    if (output == NULL) {
        fprintf(stderr, "Warning: logger call to printl while being not initialized!");
        return;
    }
#endif
    va_list args;
    va_start(args, format);
    vfprintf(output, format, args);
    fflush(output); // intégrité des logs
    va_end(args);
}

/**
 * @date 04/11/2023
 * @author LAFORGE Mateo
 */
void warnl(const char* file_name, const char* fun_name, const char* format, ...) {
#ifdef DEBUG
    if (output == NULL) {
        fprintf(stderr, "Warning: logger call to warnl while being not initialized!\nlast call was from %s in %s\n", fun_name, file_name);
        return;
    }
#endif
    va_list args;
    va_start(args, format);
    // format de sortie dépendant
    if (console) {
        fprintf(output, YELLOW);
        vfprintf(output, format, args);
        fprintf(output, RSTC);
    } else {
        fprintf(output, "[warnl] %s > %s : ", file_name, fun_name);
        vfprintf(output, format, args);
    }
    fflush(output); // intégrité des logs
    va_end(args);
}


/**
 * @date 04/11/2023
 * @author LAFORGE Mateo
 * @brief Affiche la pile d'appel de fonction
 * @pre Drapeau de compilation -rdynamic
 */
void printStackTrace() {
    void* buffer[64];
    int nbv = backtrace(buffer, sizeof(buffer));
    char** strings = backtrace_symbols(buffer, nbv);
    for (int i = 1; i<nbv; i++) { // démarre à 1 pour ignorer l'appel de cette fonction
        fprintf(stderr, "%s\n", strings[i]);
    }
    free(strings);
}

/**
 * @date 04/11/2023
 * @author LAFORGE Mateo
 */
void exitl(const char* file_name, const char* fun_name, int exit_value, char* format, ...) {
#ifdef DEBUG
    if (output == NULL) {
        fprintf(stderr, "Warning: logger call to exitl while being not initialized!\n");
        printStackTrace();
        exit(EXIT_FAILURE);
    }
#endif
    va_list args;
    va_start(args, format);
    if (errno) perror("Exit with errno : ");
    // format de sortie dépendant
    if (console) {
        fprintf(output, RED);
        fprintf(output, "[exitl] %s > %s : ", file_name, fun_name);
        vfprintf(output, format, args);
        fprintf(output, RSTC);
    } else {
        fprintf(output, "[exitl] %s > %s : ", file_name, fun_name);
        vfprintf(output, format, args);
    }
    va_end(args);
    close_logger();
    exit(exit_value);
}

/**
 * @date 04/11/2023
 * @author LAFORGE Mateo
 */
void close_logger() {
    if (output != NULL && output != stdout)
        fclose(output);
}


/*
    =======================
    === Affichage utils ===
    =======================
*/

/**
 * @brief Affiche nb_fois la chaine de caractère s
 * 
 * @param s Chaine de caractères à afficher
 * @param nb Nombre de fois
 */
void printStringN(char* s, unsigned nb) {
    for(unsigned i = 0; i < nb; i++) {
        printf("%s",s);
    }
}



/*
    =======================================
    === Affichage structures de données ===
    =======================================
*/


/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void displayListLog(List *l) {
    /* vérification paramêtre */
    testArgNull(l, "logger.c", "displayListLog", "l");
    
    /* description */
    unsigned size = listSize(l);
    printl("%s ┌──────────────── list (%d)...\n │ %s[", c_yellow, size, c_rstc);
    
    /* affichage contenu */
    if(size > 0 ) 
        printl(" %*d", SIZE_INT_DISPLAY, listGet(l, 0));
    for (unsigned i = 1; i < size; i++)
        printl(", %*d",SIZE_INT_DISPLAY, listGet(l, i));
    printl(" ]\n");
    
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void displayGenListLog(GenList *l) {
    /* vérification paramêtre */
    testArgNull(l, "logger.c", "displayGenListLog", "l");
    
    /* description */
    unsigned size = genListSize(l);
    printl("%s ┌──────────────── generic list (%d)...\n │ %s[", c_yellow, size, c_rstc);
    
    /* affichage contenu */
    if(size > 0 ) 
        printl(" %p", genListGet(l, 0));
    for (unsigned i = 1; i < size; i++)
        printl(", %p", genListGet(l, i));
    printl(" ]\n");
}


/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void displayMatrixLog(Matrix *m) {
    testArgNull(m, "logger.c", "displayMatrixLog", "m");
    
    /* description */
    unsigned nbl = matrixNbLines(m);
    unsigned nbc = matrixNbColonnes(m);
    printl("%s ┌──────────────── matrix (%d,%d)...%s\n", c_yellow, nbl, nbc, c_rstc);
    
    /* aucune ligne */
    if(nbl == 0)
        printl(" %s│%s [ ]\n", c_yellow, c_rstc);

    /* affichage contenu */
    for(unsigned l = 0; l < nbl; l++) {
        /* aucune colonne */
        if(nbc == 0)
            printl(" %s│%s [ ]\n", c_yellow, c_rstc);
        /* affichage de la ligne */
        else {
            printl(" %s│%s [ %*d", c_yellow, c_rstc, SIZE_INT_DISPLAY, matrixGet(m, l, 0));
            for(unsigned c = 1; c < nbc; c++) {
                printl(", %*d", SIZE_INT_DISPLAY, matrixGet(m, l, c));
            }
            printl(" ]\n");
        }
    }
}


/**
 * @date 13/11/2023
 * @author Ugo VALLAT
 * @brief Affiche dans le logger toutes les informations sur le ballot
 */
void displayBaleLog(Bale *b) {
    unsigned nb_cand = baleNbCandidat(b);
    unsigned nb_voter = baleNbVoter(b);
    printl("\n %s┌────────────────────────────── BALLOT (%d,%d)... \n", c_yellow, nb_voter, nb_cand);
    printl(" │\n ├────────────── Liste des candidats :%s\n", c_rstc);

    /* candidats */
    char* label;
    for(unsigned i = 0; i < nb_cand; i++) {
        label = baleColumnToLabel(b, i);
        printl(" %s├─%s C%-2d : %s %s\n", c_yellow, c_orange, i+1, label, c_rstc);
        free(label);
    }
    printl(" %s│\n ├────────────── Votes :%s\n", c_yellow, c_rstc);

    /* bordure haute */
    printl(" %s│%s ┌", c_yellow, c_rstc);
    for(unsigned i = 0; i < nb_cand-1; i++) {
        printl("─────┬");
    }
    printl("─────┐\n");

    /* nom candidats */
    printl(" %s│%s │", c_yellow, c_rstc);
    for(unsigned i = 0; i < nb_cand; i++) {
        printl("%s C%-2d %s│",c_orange, i+1, c_rstc);
    }
    printl("\n");

    /* bordure basse candidats*/
    printl(" %s│%s ├", c_yellow, c_rstc);
    for(unsigned i = 0; i < nb_cand-1; i++) {
        printl("─────┼");
    }
    printl("─────┤\n");

    /* affichage données */
    for(unsigned l = 0; l < nb_voter-1; l++) {
        printl(" %s│%s │", c_yellow, c_rstc);
        for(unsigned c = 0; c < nb_cand; c++) {
            printl(" %3d │", baleGetValue(b, l, c));
        }
        printl("\n");
        printl(" %s│%s ├", c_yellow, c_rstc);
        for(unsigned c = 0; c < nb_cand-1; c++) {
            printl("─────┼");
        }
        printl("─────┤\n");
    }
    printl(" %s│%s │", c_yellow, c_rstc);
    for(unsigned c = 0; c < nb_cand && nb_voter > 0; c++) {
        printl(" %3d │", baleGetValue(b, nb_voter-1, c));
    }
    printl("\n");

    /* bordure basse */
    printl(" %s│%s └", c_yellow, c_rstc);
    for(unsigned i = 0; i < nb_cand-1; i++) {
        printl("─────┴");
    }
    printl("─────┘\n");
}


void displayDuelLog(Duel *d) {
    unsigned nb_cand = duelNbCandidat(d);

    printl("\n %s┌────────────────────────────── DUEL (%d,%d)... \n", c_yellow, nb_cand, nb_cand);
    printl(" │\n ├────────────── Liste des candidats :%s\n", c_rstc);

    /* candidats */
    char* label;
    for(unsigned i = 0; i < nb_cand; i++) {
        label = duelIndexToLabel(d, i);
        printl(" %s├─%s C%-2d : %s %s\n", c_yellow, c_orange, i+1, label, c_rstc);
        free(label);
    }
    printl(" %s│\n ├────────────── Votes :%s\n", c_yellow, c_rstc);

    /* bordure haute */
    printl(" %s│%s ┌─────┬", c_yellow, c_rstc);
    for(unsigned i = 0; i < nb_cand-1; i++) {
        printl("─────┬");
    }
    printl("─────┐\n");

    /* nom candidats */
    printl(" %s│%s │     │", c_yellow, c_rstc);
    for(unsigned i = 0; i < nb_cand; i++) {
        printl("%s C%-2d %s│",c_orange, i+1, c_rstc);
    }
    printl("\n");

    /* bordure basse candidats*/
    printl(" %s│%s ├─────┼", c_yellow, c_rstc);
    for(unsigned i = 0; i < nb_cand-1; i++) {
        printl("─────┼");
    }
    printl("─────┤\n");

    /* affichage données */
    for(unsigned l = 0; l < nb_cand-1; l++) {
        printl(" %s│%s │%s C%-2d %s│", c_yellow, c_rstc,c_orange,l+1,c_rstc);
        for(unsigned c = 0; c < nb_cand; c++) {
            printl(" %3d │", duelGetValue(d, l, c));
        }
        printl("\n");
        printl(" %s│%s ├─────┼", c_yellow, c_rstc);
        for(unsigned c = 0; c < nb_cand-1; c++) {
            printl("─────┼");
        }
        printl("─────┤\n");
    }
    printl(" %s│%s │%s C%-2d %s│", c_yellow, c_rstc,c_orange,nb_cand,c_rstc);
    for(unsigned c = 0; c < nb_cand; c++) {
        printl(" %3d │", duelGetValue(d, nb_cand-1, c));
    }
    printl("\n");

    /* bordure basse */
    printl(" %s│%s └─────┴", c_yellow, c_rstc);
    for(unsigned i = 0; i < nb_cand-1; i++) {
        printl("─────┴");
    }
    printl("─────┘\n");
}


/*
    ===========================
    === Affichage Résultats ===
    ===========================
*/

#define MAX(x, y) (x > y ? x : y)

unsigned maxLengthLabel(GenList* l, char*(label_function(void*))) {
    unsigned nb_winners = genListSize(l);
    unsigned max = 0;
    unsigned tmp;
    for(unsigned i = 0; i < nb_winners; i++) {
        tmp = strlen(label_function(genListGet(l, i)));
        if(tmp > max) max = tmp;
    }
    return max;
}

char* singleWinnerName(void* arg) {
    return ((WinnerSingle*) arg)->name;
}
char* singleTwoWinnerName(void* arg) {
    return ((WinnerSingleTwo*) arg)->name;
}
char* condorcetWinnerName(void* arg) {
    return ((WinnerSingleTwo*) arg)->name;
}
char* majorityJudgmentWinnerName(void* arg) {
    return ((WinnerSingleTwo*) arg)->name;
}

unsigned maxLengthLabelWinner(GenList *l) {
    return maxLengthLabel(l, singleWinnerName);
}
unsigned maxLengthLabelWinnerTwo(GenList *l) {
    return maxLengthLabel(l, singleTwoWinnerName);
}
unsigned maxLengthLabelCondorcet(GenList *l) {
    return maxLengthLabel(l, condorcetWinnerName);
}
unsigned maxLengthLabelWinnerMajorityJudgmeent(GenList *l) {
    return maxLengthLabel(l, majorityJudgmentWinnerName);
}

/* décalage horizontal entre le texte affiché et les bordures */
#define SPACE_BETWEEN_BORDER 2

/* phrase d'annonce */
#define UNI1_BANNER "RESULTATS UNI 1 :"
/* taille de la phrase d'affichage d'un candidat sans compter la taille prise par le nom de celui-ci */
#define UNI1_WINNER_SENTENCE_LENGTH 13

void displayListWinnerSingle(GenList *l) {
    unsigned nb_winners = genListSize(l);
    unsigned max_length_winner = maxLengthLabelWinner(l);
    unsigned max_length_case = MAX(strlen(UNI1_BANNER), max_length_winner + UNI1_WINNER_SENTENCE_LENGTH);

    /* bordure haute */
    printf("\t╔");
    printStringN("═", max_length_case + (2 * SPACE_BETWEEN_BORDER));
    printf("╗\n");

    /* titre */
    printf("\t║  %s", UNI1_BANNER);
    printStringN(" ", max_length_case - strlen(UNI1_BANNER) + SPACE_BETWEEN_BORDER);
    printf("║\n");

    /* vainqueurs */
    WinnerSingle *wtmp;
    for(unsigned i = 0; i < nb_winners; i++) {
        wtmp = genListGet(l, i);
        printf("\t║  • %-*s : %6.2f %%", max_length_winner, wtmp->name, wtmp->score);
        printStringN(" ", max_length_case - (UNI1_WINNER_SENTENCE_LENGTH + max_length_winner) + SPACE_BETWEEN_BORDER);
        printf("║\n");
    }

    /* bordure basse */
    printf("\t╚");
    printStringN("═", max_length_case + (2 * SPACE_BETWEEN_BORDER));
    printf("╝\n");
}

/* phrase d'annonce */
#define UNI2_BANNER "RESULTATS UNI 2 :"
/* taille de la phrase d'affichage d'un candidat sans compter la taille prise par le nom de celui-ci */
#define UNI2_WINNER_SENTENCE_LENGTH 17
/* phrase d'annonce du tour 1 */
#define UNI2_FIRST_ROUND_BANNER "Tour 1..."
/* phrase d'annonce du tour 2 */
#define UNI2_SECOND_ROUND_BANNER "Tour 2..."

void printWinnerSingleTwo(WinnerSingleTwo* winner, unsigned max_length_winner, unsigned max_length_case) {
    printf("\t║   │  • %-*s : %6.2f %%", max_length_winner, winner->name, winner->score);
    printStringN(" ", max_length_case - (UNI2_WINNER_SENTENCE_LENGTH + max_length_winner) + SPACE_BETWEEN_BORDER);
    printf("║\n");
}

void displayListWinnerSingleTwo(GenList *l) {
    unsigned nb_winners = genListSize(l);
    unsigned max_length_winner = maxLengthLabelWinnerTwo(l);
    unsigned max_length_case = MAX(strlen(UNI2_BANNER), max_length_winner + UNI2_WINNER_SENTENCE_LENGTH);

    /* bordure haute */
    printf("\t╔");
    printStringN("═", max_length_case + (2 * SPACE_BETWEEN_BORDER));
    printf("╗\n");
    /* ligne vide */
    printf("\t║");
    printStringN(" ", max_length_case + (2 * SPACE_BETWEEN_BORDER));
    printf("║\n");

    /* titre */
    printf("\t║  %s", UNI2_BANNER);
    printStringN(" ", max_length_case - strlen(UNI2_BANNER) + SPACE_BETWEEN_BORDER);
    printf("║\n");

    /* Tour 1 */
    printf("\t║  %s", UNI2_FIRST_ROUND_BANNER);
    printStringN(" ", max_length_case - strlen(UNI2_FIRST_ROUND_BANNER) + SPACE_BETWEEN_BORDER);
    printf("║\n");

    /* vainqueurs tour 1 */
    WinnerSingleTwo *wtmp;
    for(unsigned i = 0; i < nb_winners; i++) {
        wtmp = genListGet(l, i);
        if (wtmp->round == 1)
            printWinnerSingleTwo(wtmp, max_length_winner, max_length_case);
    }

    /* Tour 2 */
    printf("\t║  %s", UNI2_SECOND_ROUND_BANNER);
    printStringN(" ", max_length_case - strlen(UNI2_SECOND_ROUND_BANNER) + SPACE_BETWEEN_BORDER);
    printf("║\n");

    /* vainqueurs tour 1 */
    for(unsigned i = 0; i < nb_winners; i++) {
        wtmp = genListGet(l, i);
        if(wtmp->round == 2)
            printWinnerSingleTwo(wtmp, max_length_winner, max_length_case);
    }

    /* ligne vide */
    printf("\t║");
    printStringN(" ", max_length_case + (2 * SPACE_BETWEEN_BORDER));
    printf("║\n");
    /* bordure basse */
    printf("\t╚");
    printStringN("═", max_length_case + (2 * SPACE_BETWEEN_BORDER));
    printf("╝\n");
}


void displayListWinnerCondorcet(GenList *l, char* name_algo) {
    unsigned nb_winners = genListSize(l);
    unsigned max_length_winner = maxLengthLabelCondorcet(l);
    unsigned max_length_case = max_length_winner + 20;

    /* bordure haute */
    printf("\t╔");
    printStringN("═", max_length_case);
    printf("╗\n");

    /* titre */
    printf("\t║  RESULTATS %s :", name_algo);
    printStringN(" ", max_length_case-19);
    printf("║\n");

    /* vainqueurs */
    WinnerCondorcet *wtmp;
    for(unsigned i = 0; i < nb_winners; i++) {
        wtmp = genListGet(l, i);
        printf("\t║  • %-*s        ║\n", max_length_winner, wtmp->name);
    }

    /* bordure basse */
    printf("\t╚");
    printStringN("═", max_length_case);
    printf("╝\n");
}

/* phrase d'annonce */
#define MJ_BANNER "RESULTATS Jugement Majoritaire :"
/* taille de la phrase d'affichage d'un candidat sans compter la taille prise par le nom de celui-ci */
#define MJ_WINNER_SENTENCE_LENGTH 27

void displayListWinnerMajorityJudgment(GenList *l) {
    unsigned nb_winners = genListSize(l);
    unsigned max_length_winner = maxLengthLabelWinnerMajorityJudgmeent(l);
    unsigned max_length_case = MAX(strlen(MJ_BANNER), max_length_winner + MJ_WINNER_SENTENCE_LENGTH);

    /* bordure haute */
    printf("\t╔");
    printStringN("═", max_length_case + (2 * SPACE_BETWEEN_BORDER));
    printf("╗\n");

    /* titre */
    printf("\t║  %s", MJ_BANNER);
    printStringN(" ", max_length_case - strlen(MJ_BANNER) + SPACE_BETWEEN_BORDER);
    printf("║\n");

    /* vainqueurs */
    WinnerMajorityJudgment *wtmp;
    for(unsigned i = 0; i < nb_winners; i++) {
        wtmp = genListGet(l, i);
        printf("\t║  • %-*s : (%6.2f %%,%2d,%6.2f %%)", max_length_winner, wtmp->name, wtmp->percent_sup * 100, wtmp->median , wtmp->percent_inf * 100);
        printStringN(" ", max_length_case - (MJ_WINNER_SENTENCE_LENGTH + max_length_winner) + SPACE_BETWEEN_BORDER);
        printf("║\n");
    }

    /* bordure basse */
    printf("\t╚");
    printStringN("═", max_length_case + (2 * SPACE_BETWEEN_BORDER));
    printf("╝\n");
}




