#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include "../src/interpreter.h"
#include "../src/logger.h"
#include "test_utils.h"

#define printsb(msg) addLineStringBuilder(string_builder, msg)
#define LOGGER_OUPTUT "test/ressource/interpreter.log"

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
    if (cmd == NULL) return;
    char cmdPrint[1024];
    memset(cmdPrint, 0, sizeof(cmdPrint));
    sprintf(cmdPrint, "\t\tModule:%d\n\t\tFileType:%d\n\t\tfile_name:\"%s\"\n\t\thas_log_file:%d\n\t\tlog_file:\"%s\"\n"
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
            Command* result = interprete(argc, argv);
            printl("SUCESS\n");
            write(tube[1], result, sizeof(struct command_t));
            close(tube[1]);
            free(result);
            close_logger();
            exit(EXIT_SUCCESS);
        default: /* père */
            close(tube[1]);

            int xcode;

            wait(&xcode);
            Command* cmd;
            if (WEXITSTATUS(xcode) == 0) {
                cmd = malloc(sizeof(struct command_t));
                read(tube[0], cmd, sizeof(struct command_t));
            } else {
                cmd = NULL;
            }
            close(tube[0]);
            return cmd;
    }
}

bool testInterprete() {
    beforeEach();
    
    // ### constantes de test
    char* cmd = "interprete";
    // drapeaux
    char* dflag = "-d";
    char* iflag = "-i";
    char* jflag = "-j";
    char* oflag = "-o";
    char* mflag = "-m";
    // donnée supplémentaire
    char* bale_src_file = "test/ressource/bale_1.csv";
    char* duel_src_file = "test/ressource/duel_1.csv";
    char* dest_file = "any"; // no-OP surs destination

    // sans arguments
    printsb("\ntest sur \"interprete\"");
    char* argv1[] = {cmd};
    Command* cmd1 = try(1, argv1);
    if (cmd1 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd1);
        free(cmd1);
        return false;
    }

    // sans corps pour méthode
    printsb("\n\ntest sur \"interprete -m\"");
    char* argv2[] = {cmd, mflag};
    Command* cmd2 = try(2, argv2);
    if (cmd2 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd2);
        return false;
    }

    // méthode valide sans source
    printsb("\n\ntest sur \"interprete -m uni1\"");
    char* argv3[] = {cmd, mflag, "uni1"};
    Command* cmd3 = try(3, argv3);
    if (cmd3 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd3);
        return false;
    }

    // méthode non valide
    printsb("\n\ntest sur \"interprete -m unknown\"");
    char* argv4[] = {cmd, mflag, "unknown"};
    Command* cmd4 = try(3, argv4);
    if (cmd4 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd4);
        return false;
    }

    // sans méthode
    printsb("\n\ntest sur \"interprete -d test/ressource/duel_1.csv\"");
    char* argv5[] = {cmd, dflag, duel_src_file};
    Command* cmd5 = try(3, argv5);
    if (cmd5 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd5);
        return false;
    }

    // sans source spécifiée
    printsb("\n\ntest sur \"interprete -m uni1 -i\"");
    char* argv6[] = {cmd, mflag, "uni1", iflag};
    Command* cmd6 = try(4, argv6);
    if (cmd6 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd6);
        return false;
    }

    // méthode all reconnue
    printsb("\n\ntest sur \"interprete -m jm -j test/ressource/bale_1.csv\"");
    char* argv7[] = {cmd, mflag, "jm", jflag, bale_src_file};
    Command* cmd7 = try(5, argv7);
    if (cmd7 == NULL
        || cmd7->module != JUGEMENT_MAJORITAIRE
        || cmd7->has_log_file
        || cmd7->file_type != JUDGMENT
        || strcmp(cmd7->file_name, bale_src_file) != 0
    ) {
        if (cmd7 != NULL) {
            printsb("\n\tCommand extracted:\n");
            printCommand(cmd7);
            free(cmd7);
        } else {
            printsb("\n\tInterpreter gave NULL pointer\ntry looking in the log file in test/ressource/");
        }
        return false;
    }
    if (cmd7 != NULL)
        free(cmd7);

    // sans ordre
    printsb("\n\ntest sur \"interprete -j test/ressource/bale_1.csv -m jm\"");
    char* argv8[] = {cmd, jflag, bale_src_file, mflag, "jm"};
    Command* cmd8 = try(5, argv8);
    if (cmd8 == NULL
        || cmd8->module != JUGEMENT_MAJORITAIRE
        || cmd8->has_log_file
        || cmd8->file_type != JUDGMENT
        || strcmp(cmd8->file_name, bale_src_file) != 0
    ) {
        if (cmd8 != NULL) {
            printsb("\n\tCommand extracted:\n");
            printCommand(cmd8);
            free(cmd8);
        } else {
            printsb("\n\tInterpreter gave NULL pointer\ntry looking in the log file in test/ressource/");
        }
        return false;
    }
    if (cmd8 != NULL)
        free(cmd8);

    // mauvais type de source pour méthode
    printsb("\n\ntest sur \"interprete -d test/ressource/duel_1.csv -m jm\"");
    char* argv9[] = {cmd, dflag, duel_src_file, mflag, "jm"};
    Command* cmd9 = try(5, argv9);
    if (cmd9 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd9);
        return false;
    }

    // sortie optionnelle non spécifié
    printsb("\n\ntest sur \"interprete -m jm -j test/ressource/bale_1.csv -o\"");
    char* argv10[] = {cmd, mflag, "jm", jflag, bale_src_file, oflag};
    Command* cmd10 = try(6, argv10);
    if (cmd10 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd10);
        return false;
    }

    // sortie optionnelle valide
    printsb("\n\ntest sur \"interprete -m jm -j test/ressource/bale_1.csv -o any\"");
    char* argv11[] = {cmd, mflag, "jm", jflag, bale_src_file, oflag, dest_file};
    Command* cmd11 = try(7, argv11);
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
            free(cmd11);
        } else {
            printsb("\n\tInterpreter gave NULL pointer\ntry looking in the log file in test/ressource/");
        }
        return false;
    }
    if (cmd11 != NULL)
        free(cmd11);
    
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