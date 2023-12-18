#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
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
    if (return_value == 0)
        remove(LOGGER_LOG_FILE);
}

void beforeEach() {
    emptyStringBuilder(string_builder);
}

void afterEach() {
}

void resetLogFileContent() {
    FILE* file = fopen(LOGGER_LOG_FILE, "w");
    fclose(file);
}

void redirectStandardOutput() {
    log_file = fopen(LOGGER_LOG_FILE, "w");
    if (log_file == NULL) {
        exitTest(LOGGER_LOG_FILE);
    }
    stdout = log_file;
}

void revertStandardOutputRedirection() {
    fclose(log_file);
    stdout = original_stdout;
}

bool diffFiles() {
    char command[5 + strlen(LOGGER_EXPECTED_LOG_FILE) + 1 + strlen(LOGGER_LOG_FILE) + 1];
    sprintf(command, "diff %s %s", LOGGER_EXPECTED_LOG_FILE, LOGGER_LOG_FILE);
    int status = system(command);
    return WEXITSTATUS(status) == 0;
}

bool testPrintl() {
    beforeEach();

    char data[BLOCK_SIZE];
    memset(data, 0, BLOCK_SIZE);
    
    // ### test sur stdout
    // redirection de stdout vers LOGGER_LOG_FILE
    redirectStandardOutput();
    // affichage par printl du fichier LOGGER_EXPECTED_LOG_FILE
    printsb("\ntest sur stdout\n");
    resetLogFileContent();
    init_logger(NULL);
    rewind(expected_log_file);
    while (fread(data, sizeof(char), BLOCK_SIZE, expected_log_file) > 0) {
        printl(data);
    }
    close_logger();
    // restauration de stdout
    revertStandardOutputRedirection();
    // comparaison de LOGGER_LOG_FILE à LOGGER_EXPECTED_LOG_FILE
    if (!diffFiles()) return false;

    // ### test sur sortie explicite
    printsb("\ntest sur sortie explicite\n");
    resetLogFileContent();
    init_logger(LOGGER_LOG_FILE);
    rewind(expected_log_file);
    while (fread(data, sizeof(char), BLOCK_SIZE, expected_log_file) > 0) {
        printl(data);
    }
    close_logger();
    if (!diffFiles()) return false;

    return true;
}

bool contains(const char* file_name, const char* fun_name) {
    int file_name_size = strlen(file_name);
    int fun_name_size = strlen(fun_name);
    int greater_size = file_name_size;
    if (fun_name_size > greater_size)
        greater_size = fun_name_size;
    char command[10 + greater_size + 2 + strlen(LOGGER_LOG_FILE) + 1];
    int status;

    printsb("\tcontains file_name\n");
    sprintf(command, "grep -qF \"%s\" %s", file_name, LOGGER_LOG_FILE);
    status = system(command);
    if (WEXITSTATUS(status) != 0)
        return false;

    printsb("\tcontains fun_name\n");
    sprintf(command, "grep -qF \"%s\" %s", fun_name, LOGGER_LOG_FILE);
    status = system(command);
    if (WEXITSTATUS(status) != 0)
        return false;

    return true;
}

bool containsFile() {
    char command[10 + BLOCK_SIZE + 2 + strlen(LOGGER_LOG_FILE) + 1];
    char buffer[BLOCK_SIZE];
    int status;
    printsb("\t contains logger_expected.log");
    rewind(expected_log_file);
    while (fgets(buffer, BLOCK_SIZE, expected_log_file) != NULL) {
        sprintf(command, "grep -qF \"%s\" %s", buffer, LOGGER_LOG_FILE);
        status = system(command);
        if (WEXITSTATUS(status) != 0) {
            return false;
        }
    }
    return true;
}

#define FILE_NAME "test_logger.c"

bool testWarnl() {
    beforeEach();

    const char* fun_name = "testWarnl";

    char data[BLOCK_SIZE];
    memset(data, 0, BLOCK_SIZE);

    // ### test sur stdout (codes couleur)
    redirectStandardOutput();
    printsb("\ntest sur stdout\n");
    resetLogFileContent();
    init_logger(NULL);
    rewind(expected_log_file);
    while (fread(data, sizeof(char), sizeof(data), expected_log_file) > 0) {
        warnl(FILE_NAME, fun_name, data);
    }
    close_logger();
    revertStandardOutputRedirection();
    if (!containsFile()) return false;

    printsb("\ntest sur sortie explicite\n");
    resetLogFileContent();
    init_logger(LOGGER_LOG_FILE);
    rewind(expected_log_file);
    while (fread(data, sizeof(char), sizeof(data), expected_log_file) > 0) {
        warnl(FILE_NAME, fun_name, data);
    }
    close_logger();
    if (!contains(FILE_NAME, fun_name) || !containsFile()) return false;

    return true;
}

bool testExitl() {
    beforeEach();

    const char* fun_name = "testExitl";

    char data[BLOCK_SIZE];
    memset(data, 0, BLOCK_SIZE);

    int status;

    printsb("\ntest sur stdout\n");
    resetLogFileContent();
    switch (fork()) {
        case 0:
            deleteStringBuilder(&string_builder);
            redirectStandardOutput();
            init_logger(NULL);
            rewind(expected_log_file);
            fread(data, sizeof(char), sizeof(data), expected_log_file);
            fclose(expected_log_file);
            exitl(FILE_NAME, fun_name, 1, data); // fall through
        default:
            wait(&status);
            if (!contains(FILE_NAME, fun_name) || !containsFile() || WEXITSTATUS(status) != 1) return false;
    }

    printsb("\ntest sur sortie explicite\n");
    resetLogFileContent();
    switch (fork()) {
        case 0:
            deleteStringBuilder(&string_builder);
            init_logger(LOGGER_LOG_FILE);
            rewind(expected_log_file);
            fread(data, sizeof(char), sizeof(data), expected_log_file);
            fclose(expected_log_file);
            exitl(FILE_NAME, fun_name, 1, data); // fall through
        default:
            wait(&status);
            if (!contains(FILE_NAME, fun_name) || !containsFile() || WEXITSTATUS(status) != 1) return false;
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

    test_fun(testPrintl, 1, "testPrintl");
    test_fun(testWarnl, 2, "testWarnl");
    test_fun(testExitl, 4, "testExitl");

    afterAll();
    return return_value;
}