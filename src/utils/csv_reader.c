#include "csv_reader.h"
#include <stdio.h>
#include "../structure/bale.h"
#include "../structure/duel.h"
#include "../structure/list.h"
#include "../structure/genericlist.h"
#include <string.h> 

#define USLESS_COLUM 4
#define USLESS_CHAR 14
#define NB_CADIDATES 10

void freeGenList(GenList *list);

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


void fillLigne(Bale *bale,FILE *file,int ligne){
    char buffer[256];
    char* token;
    int n;


    fscanf(file, "%s\n", buffer);

    token = strtok(buffer,",");

    for(int i = 0; i<USLESS_COLUM;i++){ //passe les lignes inutiles
        token = strtok(NULL,",");
    }
    
   for (int i = 0; i < NB_CADIDATES; i++)
   {
        
        token = strtok(NULL,",");
   }

}



Bale* csvToBale(char *file){
    FILE *file_pointer;
    file_pointer = fopen(file,'r');
    GenList *label = createGenList(10);



    Bale *bale = createBale(10,NB_CADIDATES,label);



}
