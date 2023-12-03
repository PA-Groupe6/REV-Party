
#include <getopt.h>
#include <string.h>
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "interpreter.h"

/**
 * @author LUDWIG Corentin
 * @date 02/12/2023
 */
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
    exitl("interpreter", "StringToModule", EUNKWMTH, "methode inconnue");
}

/**
 * @author LUDWIG Corentin
 * @date 02/12/2023
 */

Command interprete(int argc, char *argv[])
{
    Command command;
    int c;
    command.file_type = 0;
    command.module = 0;
    command.has_log_file = false;
    command.file_name = malloc(MAX_FILE_NAME);
    command.log_file = malloc(MAX_FILE_NAME);
    while ((c = getopt(argc, argv, "-i:-d:-j:-o:-m:")) != -1)
    {
        printf("%c\n",c);
        switch (c)
        {
        case 'i':
            if (command.file_type == 0)
            {
                command.file_type = BALE;
                strcpy(command.file_name,optarg);
            }
            else
            {
                exitl("interpreter", "intrepreter", EINCMPTB, "Les balises i j et d sont incompatible\n");
            }
            break;

        case 'd':
            if (command.file_type == 0)
            {
                command.file_type = DUEL;
                strcpy(command.file_name,optarg);
            }
            else
            {
                exitl("interpreter", "intrepreter", EINCMPTB, "Les balises i j et d sont incompatible\n");
            }
            break;

        case 'j':
            if(command.file_type == 0)
            {
                command.file_type = JUDGMENT;
                strcpy(command.file_name,optarg);
            }
            else
            {
                exitl("interpreter", "intrepreter", EINCMPTB, "Les balises i j et d sont incompatible\n");
            }
            break;

        case 'o':
        if (!command.has_log_file){
            strcpy(command.log_file,optarg);
        } else {
            exitl("interpreter", "intrepreter", EINCMPTB, "il ne peut avoir qu'un fichier de log\n");
        }
            break;

        case 'm':
        if (command.module == 0){
            command.module = stringToModule(optarg);
            printf("%d\n",command.module);
        } else {
            exitl("interpreter", "intrepreter", EINCMPTB, "il ne peut avoir qu'une seul balise de module\n");
        }
            break;

        case '?':
            exitl("interpreter", "intrepreter", EUNKWARG, "balise non reconnu ou argument manquant\n");
            break;
        }
    }



    if(command.module == 0 ){
        exitl("interpreter", "intrepreter", EMISSARG, "la commande doit avoir un -m\n");
    }

    if(command.file_type == 0 ){
        exitl("interpreter", "intrepreter", EMISSARG, "la commande doit avoir un -i, -d ou -j\n");
    }

    if (access(command.file_name, F_OK) == -1) {
        exitl("interpreter", "intrepreter", EINVLARG, "fichier d'entree inexistant\n");
    }

    if((command.module==UNI1 || command.module==UNI2 || command.module==JUGEMENT_MAJORITAIRE) && command.file_type==DUEL){
        exitl("interpreter", "intrepreter", EINCMPTB, "les methodes uninominals et jugement majoritaire ne peuvent etre appeler avec la balise -d\n");
    }

    return command;
}