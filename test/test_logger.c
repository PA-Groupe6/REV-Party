#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "test_utils.h"
#include "../src/logger.h"

#define LOGGER_EXPECTED_LOG_FILE "test/ressource/unit/logger_expected.log"
#define LOGGER_LOG_FILE "test/ressource/unit/logger.log"

#define printsb(msg) addLineStringBuilder(string_builder, msg)

#define BLOCK_SIZE 512

StringBuilder* string_builder;
int return_value;
FILE* original_stdout;
FILE* expected_log_file = NULL;
FILE* log_file = NULL;

void exitTest(char* error_msg) {
    if (expected_log_file != NULL) fclose(expected_log_file);
    perror(error_msg);
    exit(EXIT_FAILURE);
}

bool checkFiles() {
    if (expected_log_file == NULL || log_file == NULL) exitTest("Invalid State");
    rewind(expected_log_file);
    rewind(log_file);

    int celg;
    int clg;

    do {
        celg = fgetc(expected_log_file);
        clg = fgetc(log_file);
    } while (celg == clg && clg != EOF && celg != EOF);

    rewind(expected_log_file);
    rewind(log_file);

    return celg == clg;
}

void beforeAll() {
    string_builder = createStringBuilder();
    return_value = 0;
    original_stdout = stdout;

    expected_log_file = fopen(LOGGER_EXPECTED_LOG_FILE, "r");
    if (expected_log_file == NULL) {
        exitTest(LOGGER_EXPECTED_LOG_FILE);
    }
}

void afterAll() {
    deleteStringBuilder(&string_builder);
    fclose(expected_log_file);
}

void beforeEach() {
    emptyStringBuilder(string_builder);
}

void afterEach() {
}

void redirectStandardOutput() {
    log_file = fopen(LOGGER_LOG_FILE, "w+");
    if (log_file == NULL) {
        exitTest(LOGGER_LOG_FILE);
    }
    stdout = log_file;
}

void revertStandardOutputRedirection() {
    stdout = original_stdout;
}

bool testPrintl() {
    beforeEach();

    char data[BLOCK_SIZE];

    // ### test sur stdout
    // redirection de stdout vers LOGGER_LOG_FILE
    redirectStandardOutput();
    // affichage par printl du fichier LOGGER_EXPECTED_LOG_FILE
    printsb("\ntest sur stdout\n");
    init_logger(NULL);
    while (fread(data, sizeof(char), sizeof(data), expected_log_file) > 0) {
        printl(data);
    }
    close_logger();
    // restauration de stdout
    revertStandardOutputRedirection();
    // comparaison de LOGGER_LOG_FILE à LOGGER_EXPECTED_LOG_FILE
    if (!checkFiles()) return false;


    // ### test sur sortie explicite
    printsb("\ntest sur sortie explicite\n");
    init_logger(LOGGER_LOG_FILE);
    while (fread(data, sizeof(char), sizeof(data), expected_log_file) > 0) {
        printl(data);
    }
    close_logger();
    if (!checkFiles()) return false;

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

    test_fun(testPrintl, 1, "testPrintl");

    afterAll();
    return return_value;
}