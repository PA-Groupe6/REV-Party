
#include <getopt.h>
#include <string.h>
#include "logger.h"
#include "interpreter.h"

//@autor Corentin
//@date 10/25/2023
Module stringToModule(char *nom)
{
    if (strcmp(nom, "uni1"))
        return UNI1;
    if (strcmp(nom, "uni2"))
        return UNI2;
    if (strcmp(nom, "cm"))
        return MINIMAX;
    if (strcmp(nom, "cp"))
        return RANGEMENT;
    if (strcmp(nom, "cs"))
        return SCHULZE;
    if (strcmp(nom, "jm"))
        return JUGEMENT_MAJORITAIRE;
    if (strcmp(nom, "all"))
        return ALL;
    exitl("interpreter", "StringToModule", "methode inconnue",3);
}

//@autor Corentin
//@date 10/25/2023

Command intreprete(int argc, char *argv[])
{
    Command command;
    int c;
    while ((c = getopt(argc, argv, "-i:-d:-j:-o:-m:")) != -1)
    {
        switch (c)
        {
        case 'i':
            if (command.module)
            {
                command.file_type = BALE;
                command.file_name = optarg;
            }
            else
            {
                exitl("interpreter", "StringToModule", "options -i et -d incompatible",2);
            }
            break;

        case 'd':
            if (command.module)
            {
                command.file_type = DUEL;
                command.file_name = optarg;
            }
            else
            {
                exitl("interpreter", "StringToModule", "options -i et -d incompatible",2);
            }
            break;

        case 'j':
            if(command.module)
            {
                command.file_type = JUDGMENT;
                command.file_name = optarg;
            }

        case 'o':
        if (command.log_file){
            command.log_file = optarg;
        } else {
            exitl("interpreter", "StringToModule", "erreur dans la commande",2); // TO DO
        }
            break;

        case 'm':
        if (command.log_file){
            command.module = stringToModule(optarg);
        } else {
            exitl("interpreter", "StringToModule", "erreur dans la commande",2); // TO DO
        }
            break;

        case '?':
            exitl("interpreter", "StringToModule", "erreur dans la commande",2); // TO DO
            break;
        }
    }



    if(command.module!=JUGEMENT_MAJORITAIRE && command.file_type == JUDGMENT ){
        exitl("interpreter", "StringToModule", "erreur dans la commande",1);
    }
    

    if((command.module==UNI1 || command.module==UNI2) && command.file_type==BALE){
        exitl("interpreter", "StringToModule", "erreur dans la commande",1); // TO DO
    }


    return command;
}