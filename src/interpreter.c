
#include <getopt.h>
#include <string.h>
#include "logger.h"
#include <stdio.h>
#include <unistd.h>
#include "interpreter.h"

//@autor Corentin
//@date 10/25/2023
Module stringToModule(char *nom)
{
    if (strcmp(nom, "uni1") == 0)
        return UNI1;
    if (strcmp(nom, "uni2") == 0)
        return UNI2;
    if (strcmp(nom, "cm") == 0)
        return MINIMAX;
    if (strcmp(nom, "cp") == 0)
        return RANGEMENT;
    if (strcmp(nom, "cs") == 0)
        return SCHULZE;
    if (strcmp(nom, "jm") == 0)
        return JUGEMENT_MAJORITAIRE;
    if (strcmp(nom, "all") == 0)
        return ALL;
    exitl("interpreter", "StringToModule", "methode inconnue",3);
}

//@autor Corentin
//@date 22/11/2023

Command intreprete(int argc, char *argv[])
{
    Command command;
    int c;
    command.file_type = 0;
    command.module = 0;
    command.has_log_file = false;
    while ((c = getopt(argc, argv, "-i:-d:-j:-o:-m:")) != -1)
    {
        switch (c)
        {
        case 'i':
            if (command.module == 0)
            {
                command.file_type = BALE;
                command.file_name = optarg;
            }
            else
            {
                exitl("interpreter", "intrepreter", "Les balises i j et d sont incompatible\n",2);
            }
            break;

        case 'd':
            if (command.module == 0)
            {
                command.file_type = DUEL;
                command.file_name = optarg;
            }
            else
            {
                exitl("interpreter", "intrepreter", "Les balises i j et d sont incompatible\n",2);
            }
            break;

        case 'j':
            if(command.module == 0)
            {
                command.file_type = JUDGMENT;
                command.file_name = optarg;
            }
            else
            {
                exitl("interpreter", "intrepreter", "Les balises i j et d sont incompatible\n",2);
            }
            break;

        case 'o':
        if (!command.has_log_file){
            command.log_file = optarg;
        } else {
            exitl("interpreter", "intrepreter", "il ne peut avoir qu'un fichier de log\n",2); // TO DO
        }
            break;

        case 'm':
        if (command.module == 0){
            command.module = stringToModule(optarg);
        } else {
            exitl("interpreter", "intrepreter", "il ne peut avoir qu'une seul balise de module\n",2); // TO DO
        }
            break;

        case '?':
            exitl("interpreter", "intrepreter", "balise non reconnu ou argument manquant\n",2); // TO DO
            break;
        }
    }

    if (!access(command.file_name, F_OK) != -1) {
        pexitl("interpreter", "intrepreter", "fichier d'entree inexistant\n",2);
    }


    if(command.module == 0 ){
        exitl("interpreter", "intrepreter", "la commande doit avoir un -m\n",1);
    }

    if(command.file_type == 0 ){
        exitl("interpreter", "intrepreter", "la commande doit avoir un -i, -d ou -j\n",1);
    }


    // if(command.module!=JUGEMENT_MAJORITAIRE && command.file_type == JUDGMENT ){
    //     exitl("interpreter", "intreprete", "la balise j ne peut etre utiliser que par pour un methode de jugement majoritaire\n",1);
    // }
    

    if((command.module==UNI1 || command.module==UNI2 || command.module==JUGEMENT_MAJORITAIRE) && command.file_type==BALE){
        exitl("interpreter", "intrepreter", "les methodes uninominals et jugement majoritaire ne peuvent etre appeler avec la balise -i\n",1); // TO DO
    }


    return command;
}