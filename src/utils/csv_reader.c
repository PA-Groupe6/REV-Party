#include "csv_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "../structure/bale.h"
#include "../structure/duel.h"
#include "../structure/genericlist.h"

#define USLESS_COLUM 4
#define USLESS_CHAR 14
#define NB_CADIDATES 10


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
unsigned nbLigne(char *file){
    FILE *pipe;
    char commande[256];
    int nbLigne;
    strcat(commande,"wc -l");
    strcat(commande,file);

    pipe = popen(commande,"r");
    if (pipe == NULL) {
    }

    fscanf(pipe,"%d",&nbLigne);

    fclose(pipe);
    return nbLigne;
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
void readLabel(FILE *file,GenList *label){
    char buffer[256];
    char* token;
    char* tab_name[NB_CADIDATES];
    fscanf(file, "%s\n", buffer);

    token = strtok(buffer,",");

    for(int i = 0; i<USLESS_COLUM;i++){ //passe les lignes inutiles
        token = strtok(NULL,",");
    }
    
   for (int i = 0; i < NB_CADIDATES; i++)
   {
        tab_name[i] = malloc(MAX_LENGHT_LABEL);
        strcpy(tab_name[i],token+USLESS_CHAR);
        genListInsert(label,(void*)tab_name[i],i);
        token = strtok(NULL,",");
   }
}


/**
 * @date 23/11/2023
 * @author LUDWIG Corentin
 * @brief remplit la ligne du ballot passer en paramtre avec les information contenu dans le fichier passer en entree
 *
 * @param[out] bale ballot a remplir
 * @param[in] file fichier dans le quelle lire
 * @param[in] ligne numero de la colone a lire
 * 
 * @pre baleNBcandidat(bale) >= NB_CADIDATES
*/
void fillLigne(Bale *bale,FILE *file,unsigned ligne){
    char buffer[256];
    char* token;
    int n;


    fscanf(file, "%s\n", buffer);

    token = strtok(buffer,",");

    for(int i = 0; i<USLESS_COLUM;i++){ //passe les lignes inutiles
        token = strtok(NULL,",");
    }
    
   for (unsigned i = 0; i < NB_CADIDATES; i++)
   {
        sscanf("%d",&n);
        bale = baleSetValue(bale, ligne, i, n);
        token = strtok(NULL,",");
   }

}


/**
 * @date 23/11/2023
 * @author LUDWIG Corentin
 * @brief remplit le ballot passer en parametre avec les information contenu dans le fichier passer en entree
 *
 * @param[out] bale ballot a remplir
 * @param[in] file fichier dans le quelle lire
 * 
 * @pre baleNBcandidat(bale) >= NB_CADIDATES && baleNbVotant(bale) >= nbLigne(file)
*/
void fillBale(Bale *bale,FILE *file){
    int i = 0;

    while(!feof(file)){
        i++;
        fillLigne(bale,file,i);
    }

}


/**
 * @date 23/11/2023
 * @author LUDWIG Corentin
*/
Bale* csvToBale(char *file){
    FILE *file_pointer;
    file_pointer = fopen(file,"r");
    GenList *label = createGenList(10);

    int nbl = nbLigne(file);

    Bale *bale = createBale(nbl,NB_CADIDATES,label);
    freeListLabel(label);

    fclose(file_pointer);
    return bale;
}
