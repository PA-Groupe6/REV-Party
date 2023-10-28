#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>
#include "test_utils.h"

#define OKC "\033[38;5;120m"
#define KOC "\033[38;5;124m"

StringBuilder* createStringBuilder() {
    StringBuilder* string_builder = malloc(sizeof(struct s_string_builder));
    string_builder->lastLine = 0;
    return string_builder;
}

void emptyStringBuilder(StringBuilder* string_builder) {
    string_builder->lastLine = 0;
    string_builder->output[0] = '\0';
}

void addLineStringBuilder(StringBuilder* string_builder, char* line) {
    strcpy(string_builder->output + string_builder->lastLine, line);
    string_builder->lastLine+=strlen(line);
}

void printStringBuilder(StringBuilder* string_builder) {
    printf("%s\n%s[>>>]: here%s\n", string_builder->output, KOC, RSTC);
}

#define STATUS_LENGTH 67
#define STATUS_PAD_CHAR '>'

/**
 * @date 27/10/2023
 * @brief Affiche un statue formaté contenant un message
 * 
 * @param[in] message Message du statue
 */
void printStatus(const char* message) {
    char status[STATUS_LENGTH];
    int i = 0;
    /* copie de message dans status */
    for (; message[i] != '\0'; i++)
        status[i] = message[i];
    /* ajout d'un caractère de remplissage */
    for (; i<STATUS_LENGTH-1; i++)
        status[i] = STATUS_PAD_CHAR;
    /* fin de la chaîne */
    status[STATUS_LENGTH-1] = '\0';
    printf("\n%s\n", status);
}

void printSuccess(char* test_name) {
    /* taille du message sans test_name + taille test_name + '\0' */
    int msg_size = 29 + strlen(test_name) + 1;
    char message[msg_size];
    sprintf(message, "(%sOK%s) %s passed ", OKC, RSTC, test_name);
    printStatus(message);
}

void printFailure(char* test_name) {
    /* taille du message sans test_name + taille test_name + '\0' */
    int msg_size = 29 + strlen(test_name) + 1;
    char message[msg_size];
    sprintf(message, "(%sKO%s) %s passed ", KOC, RSTC, test_name);
    printStatus(message);
}