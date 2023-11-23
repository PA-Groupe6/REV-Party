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

void freeGenList(GenList *list);



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



void fillBale(Bale *bale,FILE *file){
    int i = 0;

    while(!feof(file)){
        i++;
        if(baleNbVoter(bale)<i){
            
        }
        fillLigne(bale,file,i);
    }

}



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
