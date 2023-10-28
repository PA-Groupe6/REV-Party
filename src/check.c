#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/sha256/sha256_utils.h"

#define KEY_MAX_LENGTH 64

#define LINE_MAX_LENGTH 256

/* Réinitialise la couleur de l'affichage */
#define RSTC "\033[0m"

#define RED "\033[38;5;124m"

#define YELLOW "\033[38;5;184m"

#define GREEN "\033[38;5;40m"



/*
    Calcul le hash à partir de nom et de la clé
*/
char* computeHash(char* name, char* key) {
    size_t name_len = strlen(name);
    size_t key_len = strlen(key);
    size_t total_len = name_len + key_len + 1; /* +1 pour caractère \0 */

    char* to_hash = (char*)malloc(sizeof(char)*total_len);
    if (to_hash == NULL) {
        perror("Memory allocation");
        exit(EXIT_FAILURE);
    }

    /* concaténation des chaînes */
    strcpy(to_hash, name);
    strcat(to_hash, key);

    /* calcul du hash */
    char* hash_result = malloc((SHA256_BLOCK_SIZE*2 + 1) * sizeof(char));
    sha256ofString((BYTE*) to_hash, hash_result);

    free(to_hash);
    return hash_result;
}


/* 
    Renvoie une chaine de caractère contenant les noms des colonnes
*/
char* getColumnLabel(FILE* file) {
    /* Buffer pour stocker la ligne */
    char* line = malloc(LINE_MAX_LENGTH*sizeof(char));

    /* Récupération de la première ligne */
    if(fgets(line, LINE_MAX_LENGTH, file) == NULL) {
        fprintf(stderr," <getColumnLabel> Echec lecture colonnes");
        exit(EXIT_FAILURE);
    }

    /* enlever le '\n' à la fin de la ligne */
    int line_len = strlen(line);
    line[line_len-1] = '\0';
    return line;
}


/* 
    Cheche le hash dans le csv 
*/
char* search(char* hash, FILE* file) {
    int found = 0;
    char* line = malloc(LINE_MAX_LENGTH*sizeof(char)); /* Buffer pour stocker la ligne */

    /* boucle de recherche */
    while (!found && (fgets(line, LINE_MAX_LENGTH, file) != NULL)) {
        if (strstr(line, hash)) {
            found = 1;
        }
    }
    line[strlen(line)-1] = '\0';

    if (found == 0) {
        free(line);
        return NULL;
    } return line;
}


/* 
Sépare chaques éléments d'une chaîne de caractère séparés par le spliter
*/
unsigned int splitLine(char*** desti, char* line, char* spliter) {
    unsigned int size = 0;
    unsigned int memory_size = 0;
    char* token;
    char ** dest = NULL;

    /* découpage en tokens */
    token = strtok(line, spliter);
    
    /* récupération des tokens */
    while(token != NULL) {
        /* redimensionnement du tableau */
        if(memory_size <= size) {
            memory_size += 5;
            dest = (char**)realloc(dest, sizeof(char*)*memory_size);
        }
        

        /* ajout du token dans le tableau */
        dest[size] = (char*)malloc(strlen(token) + 1);
        strcpy(dest[size], token);

        /* récupération du prochian token */
        token = strtok(NULL, spliter);
        size++;
    }

    /* renvoie des données */
    *desti = dest;
    return size;
}


/*
    Affiche le vote de l'utilisateur dans la console
*/
void displayVote(char* name, char** vote, char** labels, unsigned int nb_columns) {
    printf("\n%s<+>%s--- Bonjour%s %s %s, votre vote était :\n %s|%s\n",YELLOW, RSTC, RED, name, RSTC, YELLOW, RSTC);
    for (int i = 0; i < nb_columns; i++) {
        printf(" %s|%s %-35s %s->%s %5s\n", YELLOW, RSTC, labels[i], GREEN, RSTC, vote[i]);
    }
   printf(" %s|\n<+>%s--- Bonne journée ---%s<+>%s\n\n", YELLOW, RSTC, YELLOW, RSTC);
        
}



int main(int argc, char* argv[]) {
    /* Vérification des arguments */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <\"FIRST_NAME Last_name\"> <vote_file_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* récupération de la clé */
    char key[KEY_MAX_LENGTH];
    printf("\n\n%s<+>%s Veuillez rentrer votre clé privée de vote:\n%s |\n<+>%s >> %s",YELLOW,RSTC, YELLOW, GREEN, RSTC);
    scanf("%s", key);


    /* calcul du hash */
    char* hash = computeHash(argv[1], key);

    /* ouverture du fichier */
    FILE* file = fopen(argv[2], "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    /* Récupération des noms de colonnes */
    char* str_labels = getColumnLabel(file);

    /* Récupération de la ligne du votant */
    char* str_result = search(hash, file);

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

    for (int i = 0; i < nb_column_labels ; i++) free(split_labels[i]);
    free(split_labels);
    for (int i = 0; i < nb_column_labels; i++) free(split_result[i]);
    free(split_result);

    return EXIT_SUCCESS;
}