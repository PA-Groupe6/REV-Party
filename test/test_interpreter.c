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
    remove(LOGGER_OUPTUT);
}

void afterEach() {
    emptyStringBuilder(string_builder);
}

void printCommand(Command* cmd) {
    char cmdPrint[256];
    memset(cmd, 0, sizeof(cmdPrint));
    sprintf(cmdPrint, "\tCommand:\n\t\tModule:%d\n\t\tFileType:%d\n\t\tfile_name:\"%s\"\n\t\thas_log_file:%d\n\t\tlog_file:\"%s\"\n"
                                       , cmd->module,     cmd->file_type,    cmd->file_name,       cmd->has_log_file,   cmd->log_file
        );
    printsb(cmdPrint);
}

Command* try(int argc, char* argv[]) {
    int tube[2];
    if (pipe(tube) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    switch (fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0: /* fils */
            close(tube[0]);
            init_logger(LOGGER_OUPTUT);
            Command result = interprete(argc, argv);
            write(tube[1], &result, sizeof(Command));
            exit(EXIT_SUCCESS);
        default: /* père */
            close(tube[1]);

            int xcode;
            Command* cmd = malloc(sizeof(Command));

            wait(&xcode);
            if (WEXITSTATUS(xcode) == 0) {
                read(tube[0], cmd, sizeof(Command));
            } else {
                cmd = NULL;
            }

            close(tube[0]);
            return cmd;
    }
}

bool testInterprete() {
    // ### constantes de test
    char* cmd = "interprete";
    // drapeaux
    char* dflag = "-d";
    char* iflag = "-i";
    char* jflag = "-j";
    char* oflag = "-o";
    char* mflag = "-m";
    // donnée supplémentaire
    char* bale_src_file = "test/ressource/unit/bale_1.csv";
    char* duel_src_file = "test/ressource/unit/duel_1.csv";
    char* destfile = "any"; // no-OP surs destination

    printsb("\ntest sur \"interprete -d test/ressource/unit/bale_1.csv\"");
    char* argv1[] = {cmd, dflag, bale_src_file};
    Command* cmd1 = try(3, argv1);
    if (cmd1 != NULL) {
        printsb("Command extracted:\n");
        printCommand(cmd1);
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