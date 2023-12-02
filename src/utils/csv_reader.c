/**
 * @author Ugo VALLAT, Corentin LUDWIG
 * @brief Chargement du csv
 * @date 2023-11-26
 * 
 */


#include "csv_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "../structure/bale.h"
#include "../structure/duel.h"
#include "../structure/genericlist.h"
#include "../logger.h"

#define USLESS_COLUM_BALE 4
#define USLESS_CHAR 14
#define SIZE_BUFF_LINE 512


/**
 * @date 23/11/2023
 * @author LUDWIG Corentin
 * @brief Supprime et libere la memoire de la genList et de tout les pointeur quel contients
 *
 * @param[in] la list a supprimer
 * 
 * @return <explications>
*/
void freeListLabel(GenList *list){
    while(!genListEmpty(list))
        free(genListPop(list));
    deleteGenList(&list);
}


/**
 * @date 23/11/2023
 * @author LUDWIG Corentin
 * @brief compte le nombre de ligne du fihier passer en parametre
 *
 * @param[in] nom/path du fichier
 * 
 * @return nombre de ligne du fichier
*/
int nbLigne(FILE *file){
    int nb_line = 0;
    char buff[SIZE_BUFF_LINE];
    rewind(file);
    while(fgets(buff, SIZE_BUFF_LINE, file)) {
        if(strlen(buff) > 10) nb_line++;
    }
    rewind(file);
    return nb_line;
}


/**
 * @date 26/11/2023
 * @author Ugo VALLAT, Corentin LUDWIG
 * @brief Format les labels
 * 
 * @param[in] token Token d'entrée du csv
 * @param[out] label Pointeur vers le label à remplir
 */
void tokenToLabel(char* token, char* label) {
    int i = 0;
    int size_tok = strlen(token);
    if (size_tok > MAX_LENGHT_LABEL)  
        size_tok = MAX_LENGHT_LABEL;
    while(token[i] == ' ') {
        size_tok--;
        token++;
    }
    while(i<size_tok && token[i] != '\n') i++;
        token[i] = '\0';
    strncpy(label,token, MAX_LENGHT_LABEL);
}


/**
 * @date 23/11/2023
 * @author LUDWIG Corentin
 * @brief remplit la liste label avec les noms des candidat present de le fichier file
 *
 * @param[in] nom/path du fichier
 * @param[out] list des labels remplit apres l'execution de la fonction
 *
 * @pre len(label) >= nb lable dans le fichier
*/
void readLabel(FILE *file,GenList *label, unsigned skiped_column){
    char buffer[SIZE_BUFF_LINE];
    char* token;
    char* label_name;
    fgets(buffer, sizeof(buffer), file);

    token = strtok(buffer,",");

    for(int i = 0; i<skiped_column;i++){ //passe les lignes inutiles
        token = strtok(NULL,",");
    }
    
   for (int i = 0; token; i++)
   {
        label_name = malloc(MAX_LENGHT_LABEL);
        tokenToLabel(token, label_name);
        genListInsert(label,(void*)label_name,i);
        token = strtok(NULL,",");
   }
}


/**
 * @date 23/11/2023
 * @author LUDWIG Corentin
 * @brief remplit le ballot passé en parametre avec les information contenu dans le fichier passé en entrée
 *
 * @param[out] bale ballot a remplir
 * @param[in] file fichier dans le quelle lire
 * 
*/
void fillBale(FILE *file, Bale *bale, int nbl, unsigned skiped_column) {
    char buffer[SIZE_BUFF_LINE];
    char* token;
    int n;

    /* ignorer première ligne */
    strncpy(buffer, "", SIZE_BUFF_LINE);
    fgets(buffer, sizeof(buffer), file);

    for(int l = 0; l < nbl; l++) {
        /* lecture ligne */
        if(!fgets(buffer, sizeof(buffer), file))
            exitl("csv_reader.c", "fillBale", EXIT_FAILURE, "Echec lecture ligne");
        
        /* convertion en token */
        token = strtok(buffer,",");

        for(int i = 0; i<skiped_column;i++){ //passe les colonnes inutiles
            token = strtok(NULL,",");
        }

        /* récupération des tokens */
        for (unsigned c = 0; token; c++) {
            sscanf(token,"%d",&n);
            bale = baleSetValue(bale, l, c, n);
            token = strtok(NULL,",");
        }
    }
}


/**
 * @date 23/11/2023
 * @author LUDWIG Corentin
*/
Bale* csvToBale(char *file){
    /* ouverture du csv */
    FILE *file_pointer;
    file_pointer = fopen(file,"r");
    if(!file_pointer)
        exitl("csv_reader.c", "csvToBale", EXIT_FAILURE, "Echec ouverture fichier");

    /* récupération des labels */
    GenList *label = createGenList(10);
    readLabel(file_pointer, label,USLESS_COLUM_BALE);

    /* compte nombre de lignes */
    int nbl_file = nbLigne(file_pointer);
    int nbc = genListSize(label);

    /* création ballot */
    Bale *bale = createBale(nbl_file-1,nbc,label);
    fillBale(file_pointer, bale, nbl_file-1,USLESS_COLUM_BALE);
    freeListLabel(label);

    fclose(file_pointer);
    return bale;
}


/**
 * @date 23/11/2023
 * @author Ugo VALLAT
 * @brief remplit le duel passé en parametre avec les information contenu dans le fichier passé en entrée
 *
 * @param[out] duel Duel a remplir
 * @param[in] file fichier dans le quelle lire
*/
void fillDuel(FILE *file, Duel *duel) {
    char buffer[SIZE_BUFF_LINE];
    char* token;
    int n;
    unsigned nb_candidat = duelNbCandidat(duel);

    /* ignorer première ligne */
    strncpy(buffer, "", SIZE_BUFF_LINE);
    fgets(buffer, sizeof(buffer), file);

    for(int l = 0; l < nb_candidat; l++) {
        /* lecture ligne */
        if(!fgets(buffer, sizeof(buffer), file))
            exitl("csv_reader.c", "fillDuel", EXIT_FAILURE, "Echec lecture ligne");
        
        /* convertion en token */
        token = strtok(buffer,",");

        /* récupération des tokens */
        for (unsigned c = 0; token; c++) {
            sscanf(token,"%d",&n);
            duel = duelSetValue(duel, l, c, n);
            token = strtok(NULL,",");
        }
    }
}


/**
 * @date 1/12/2023
 * @author Ugo VALLAT
*/
Duel* csvToDuel(char *file){
    /* ouverture du csv */
    FILE *file_pointer;
    file_pointer = fopen(file,"r");
    if(!file_pointer)
        exitl("csv_reader.c", "csvToDuel", EXIT_FAILURE, "Echec ouverture fichier");

    /* récupération des labels */
    GenList *label = createGenList(10);
    readLabel(file_pointer, label,0);

    /* création duel */
    unsigned nb_candidat = genListSize(label);
    Duel *duel = createDuel(nb_candidat,label);
    fillDuel(file_pointer, duel);
    freeListLabel(label);

    fclose(file_pointer);
    return duel;
}