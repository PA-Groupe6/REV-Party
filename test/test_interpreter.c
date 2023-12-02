#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include "../src/interpreter.h"
#include "../src/logger.h"
#include "test_utils.h"

#define printsb(msg) addLineStringBuilder(string_builder, msg)
#define LOGGER_OUPTUT "test/ressource/unit/interpreter.log"

StringBuilder* string_builder;
int return_value;

void beforeAll() {
    string_builder = createStringBuilder();
}

void afterAll() {
    deleteStringBuilder(&string_builder);
    if (return_value == 0) {
        remove(LOGGER_OUPTUT);
    }
}

void beforeEach() {
    emptyStringBuilder(string_builder);
}

void afterEach() {
}

void printCommand(Command* cmd) {
    char cmdPrint[256];
    memset(cmd, 0, sizeof(cmdPrint));
    sprintf(cmdPrint, "\tCommand:\n\t\tModule:%d\n\t\tFileType:%d\n\t\tfile_name:\"%s\"\n\t\thas_log_file:%d\n\t\tlog_file:\"%s\""
                                       , cmd->module,     cmd->file_type,    cmd->file_name,       cmd->has_log_file,   cmd->log_file
        );
    printsb(cmdPrint);
}

Command* try(int argc, char* argv[]) {
    int tube[2];
    if (pipe(tube) == -1) {
        perror("pipe");
        afterAll();
        exit(EXIT_FAILURE);
    }
    switch (fork()) {
        case -1:
            perror("fork");
            close(tube[0]);
            close(tube[1]);
            afterAll();
            exit(EXIT_FAILURE);
        case 0: /* fils */
            close(tube[0]);
            deleteStringBuilder(&string_builder);
            init_logger(LOGGER_OUPTUT);
            for (int i = 0; i < argc; i++) {
                printl("%s ", argv[i]);
            }
            printl(":\n");
            Command result = interprete(argc, argv);
            write(tube[1], &result, sizeof(Command));
            close(tube[1]);
            free(result.file_name);
            exit(EXIT_SUCCESS);
        default: /* père */
            close(tube[1]);

            int xcode;

            wait(&xcode);
            Command* cmd;
            if (WEXITSTATUS(xcode) == 0) {
                cmd = malloc(sizeof(Command));
                read(tube[0], cmd, sizeof(Command));
                close(tube[0]);
            } else {
                close(tube[0]);
                cmd = NULL;
            }
            return cmd;
    }
}

bool testInterprete() {
    beforeEach();
    
    // ### constantes de test
    // drapeaux
    char* dflag = "-d";
    char* iflag = "-i";
    char* jflag = "-j";
    char* oflag = "-o";
    char* mflag = "-m";
    // donnée supplémentaire
    char* bale_src_file = "test/ressource/unit/bale_1.csv";
    char* duel_src_file = "test/ressource/unit/duel_1.csv";
    char* dest_file = "any"; // no-OP surs destination

    // sans arguments
    
    printsb("\ntest sur \"interprete\"");
    Command* cmd1 = try(0, NULL);
    if (cmd1 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd1);
        free(cmd1);
        return false;
    }

    // sans corps pour méthode
    printsb("\ntest sur \"interprete -m\"");
    char* argv2[] = {mflag};
    Command* cmd2 = try(1, argv2);
    if (cmd2 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd2);
        return false;
    }

    // méthode valide sans source
    printsb("\ntest sur \"interprete -m uni1\"");
    char* argv3[] = {mflag, "uni1"};
    Command* cmd3 = try(2, argv3);
    if (cmd3 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd3);
        return false;
    }

    // méthode non valide
    printsb("\ntest sur \"interprete -m unknown\"");
    char* argv4[] = {mflag, "unknown"};
    Command* cmd4 = try(2, argv4);
    if (cmd4 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd4);
        return false;
    }

    // sans méthode
    printsb("\ntest sur \"interprete -d test/ressource/unit/duel_1.csv\"");
    char* argv5[] = {dflag, duel_src_file};
    Command* cmd5 = try(2, argv5);
    if (cmd5 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd5);
        return false;
    }

    // sans source spécifiée
    printsb("\ntest sur \"interprete -m uni1 -i\"");
    char* argv6[] = {mflag, "uni1", iflag};
    Command* cmd6 = try(3, argv6);
    if (cmd6 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd6);
        return false;
    }

    // méthode all reconnue
    printsb("\ntest sur \"interprete -m jm -j test/ressource/unit/bale_1.csv\"");
    char* argv7[] = {mflag, "jm", jflag, bale_src_file};
    Command* cmd7 = try(4, argv7);
    if (cmd7 == NULL
        || cmd7->module != JUGEMENT_MAJORITAIRE
        || cmd7->has_log_file
        || cmd7->file_type != JUDGMENT
        || strcmp(cmd7->file_name, bale_src_file) != 0
    ) {
        if (cmd7 != NULL) {
            printsb("\n\tCommand extracted:\n");
            printCommand(cmd7);
        } else {
            printsb("\n\tInterpreter gave NULL pointer\ntry looking in the log file in test/ressource/unit/");
        }
        return false;
    }

    // sans ordre
    printsb("\ntest sur \"interprete -j test/ressource/unit/bale_1.csv -m jm\"");
    char* argv8[] = {jflag, bale_src_file, mflag, "jm"};
    Command* cmd8 = try(4, argv8);
    if (cmd8 == NULL
        || cmd8->module != JUGEMENT_MAJORITAIRE
        || cmd8->has_log_file
        || cmd8->file_type != JUDGMENT
        || strcmp(cmd8->file_name, bale_src_file) != 0
    ) {
        if (cmd8 != NULL) {
            printsb("\n\tCommand extracted:\n");
            printCommand(cmd8);
        } else {
            printsb("\n\tInterpreter gave NULL pointer\ntry looking in the log file in test/ressource/unit/");
        }
        return false;
    }

    // mauvais type de source pour méthode
    printsb("\ntest sur \"interprete -d test/ressource/unit/duel_1.csv -m jm\"");
    char* argv9[] = {dflag, duel_src_file, mflag, "jm"};
    Command* cmd9 = try(4, argv9);
    if (cmd9 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd9);
        return false;
    }

    // sortie optionnelle non spécifié
    printsb("\ntest sur \"interprete -m jm -j test/ressource/unit/bale_1.csv -o\"");
    char* argv10[] = {mflag, "jm", jflag, bale_src_file, oflag};
    Command* cmd10 = try(5, argv10);
    if (cmd10 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd10);
        return false;
    }

    // sortie optionnelle valide
    printsb("\ntest sur \"interprete -m jm -j test/ressource/unit/bale_1.csv -o any\"");
    char* argv11[] = {mflag, "jm", jflag, bale_src_file, oflag, dest_file};
    Command* cmd11 = try(4, argv11);
    if (cmd11 == NULL
        || cmd11->module != JUGEMENT_MAJORITAIRE
        || !cmd11->has_log_file
        || strcmp(cmd11->log_file, dest_file) != 0
        || cmd11->file_type != JUDGMENT
        || strcmp(cmd11->file_name, bale_src_file) != 0
    ) {
        if (cmd11 != NULL) {
            printsb("\n\tCommand extracted:\n");
            printCommand(cmd11);
        } else {
            printsb("\n\tInterpreter gave NULL pointer\ntry looking in the log file in test/ressource/unit/");
        }
        return false;
    }
    
    return true;
}

void test_fun(bool(*f)(), int fnb, char* fname) {
    bool test_success = f();
    afterEach();
    if (!test_success) {
        return_value += fnb;
        printFailure(fname);
        printStringBuilder(string_builder);
    } else printSuccess(fname);
}

int main() {
    beforeAll();

    test_fun(testInterprete, 1, "testInterprete");

    afterAll();
    return return_value;
}