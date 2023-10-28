#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/sha256/sha256_utils.h"

#define KEY_MAX_LENGTH 64

// free the return value
BYTE* hexToBin(char* hex) {
    size_t sizeb = sizeof(BYTE); /* taille de la structure réceptrice */
    size_t hex_len = strlen(hex); 
    size_t data_len = hex_len/2;
    if (hex_len%2!=0) data_len+=1;

    BYTE* data = malloc(data_len*sizeb + 1);

    /* construction buffer pour appel strtoul */
    char buffer[sizeb+1];
    buffer[sizeb] = '\0';

    int offset;

    for (int i = 0; i<data_len; i++) {
        offset = i * (sizeb * 2);
        for (int j = 0; j<(sizeb * 2); j++) {
            if (offset+j<hex_len) {
                buffer[j] = hex[offset+j];
            } else buffer[j] = '\0'; /* rempli le buffer de \0 */
        }
        data[i] = strtoul(buffer, NULL, 16);
    }

    data[data_len] = 0;
    return data;
}

// free the return value
char* computeHash(char* name, char* key) {
    size_t name_len = strlen(name);
    size_t key_len = strlen(key);
    size_t total_len = name_len + key_len + 1; /* +1 pour caractère \0 */

    char* to_hash = (char*)malloc(total_len);
    if (to_hash == NULL) {
        perror("Memory allocation");
        exit(EXIT_FAILURE);
    }

    /* concaténation des chaînes */
    strcpy(to_hash, name);
    strcat(to_hash, key);

    char* hash_result = malloc((SHA256_BLOCK_SIZE*2 + 1) * sizeof(char));
    sha256ofString((BYTE*) to_hash, hash_result);

    free(to_hash);
    return hash_result;
}

#define LINE_MAX_LENGTH 256

// récupère les noms des colonnes d'un fichier ouvert csv
char* getColumnLabel(FILE* file) {
    char* line = malloc(LINE_MAX_LENGTH*sizeof(char)); /* Buffer pour stocker la ligne */
    fgets(line, LINE_MAX_LENGTH, file);
    int line_len = strlen(line);
    line[line_len-1] = '\0';
    return line;
}

char* search(char* hash, FILE* file) {
    int found = 0;
    char* line = malloc(LINE_MAX_LENGTH*sizeof(char)); /* Buffer pour stocker la ligne */

    while (!found && (fgets(line, LINE_MAX_LENGTH, file) != NULL)) {
        if (strstr(line, hash)) {
            found = 1;
        }
    }

    if (found == 0) {
        free(line);
        return NULL;
    } return line;
}

unsigned int splitLine(char*** desti, char* line, char* spliter) {
    unsigned int size = 0;
    char* token;
    char ** dest = NULL;
    /* découpage en tokens */
    token = strtok(line, spliter);
    
    /* récupération des tokens */
    while(token != NULL) {
        size++;
        /* redimensionnement du tableau */
        dest = (char**)realloc(dest, size*sizeof(char*));

        /* ajout du token dans le tableau */
        dest[size-1] = (char*)malloc(strlen(token) + 1);
        strcpy(dest[size-1], token);

        /* récupération du prochian token */
        token = strtok(NULL, spliter);
    }

    /* renvoie des données */
    *desti = dest;
    return size;
}

void displayVote(char* name, char** vote, char** labels, unsigned int nb_columns) {
    printf("\n <+>--- Bonjour %s, votre vote était :\n\n", name);
    for (int i = 0; i < nb_columns; i++) {
        printf("%-35s -> %5s\n", labels[i], vote[i]);
    }
    printf("\n <+>--- Bonne journée ---<+>\n\n");
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <your_name> <vote_file_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* récupération de la clé */
    char key[KEY_MAX_LENGTH];
    printf("Veuillez rentrer votre clé privée de vote:\n");
    scanf("%s", key);

    /* convertion de la clé en binaire */
    BYTE* bin_key = hexToBin(key);

    /* calcul du hash */
    char* hash = computeHash(argv[1], bin_key);

    free(bin_key);

    FILE* file = fopen(argv[2], "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    /* Récupération des noms de colonnes */
    char* str_labels = getColumnLabel(file);

    /* Récupération de la ligne du votant */
    char* str_result = search("Hash", file);

    free(hash);

    fclose(file);

    if (str_result == NULL) {
        free(str_labels);
        fprintf(stderr, "Votre vote n'as pas été trouvé dans le csv fourni\n");
        exit(EXIT_FAILURE);
    }

    /* séparer les colonnes */
    char** split_result;
    char** split_labels;
    int nb_column_result, nb_column_labels;

    nb_column_result = splitLine(&split_result, str_result, ",");
    nb_column_labels = splitLine(&split_labels, str_labels, ",");

    free(str_labels);
    free(str_result);

    if(nb_column_result != nb_column_labels){
        fprintf(stderr, "Echec séparation des colonnes\n");
        exit(EXIT_FAILURE);
    }

    /* affichage du résultat */
    displayVote(argv[1], split_result, split_labels, nb_column_result);

    for (int i = 0; split_labels[i] != NULL; i++) free(split_labels[i]);
    free(split_labels);
    for (int i = 0; split_result[i] != NULL; i++) free(split_result[i]);
    free(split_result);

    return EXIT_SUCCESS;
}