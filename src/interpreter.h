/** 
 * @file interpreter.h
 * @author LUDWIG Corentin
 * @brief Header de l'interpreteur de commande
*/
#ifndef __INTERPRETER__H__
#define __INTERPRETER__H__

#include <stdbool.h>

#define EUNKWMTH 1 /* méthode non reconnue */
#define EUNKWARG 2 /* argument non reconnu */
#define EINVLARG 3 /* argument invalide */
#define EMISSARG 4 /* argument manquant */
#define EINCMPTB 5 /* arguments incompatibles */

#define MAX_FILE_NAME 256

/**
 * @date 27/10/2023
 * @brief Modules de calcul du vaiqueur
 */
typedef enum e_module {
    UNI1=1, UNI2, MINIMAX, RANGEMENT, SCHULZE, JUGEMENT_MAJORITAIRE, ALL
} Module;


/**
 * @date 27/10/2023
 * @brief Types de fichier csv pouvant être renseigné par l'utilisateur
 */
typedef enum e_file_type {
    BALE=1, DUEL, JUDGMENT
} FileType;

/**
 * @date 19/11/2023
 * @brief Contient toutes les informations nécéssaires au calcul du vainqueur
 */
typedef struct command_t {
    Module module;        /* Module appelé, cf enum Module */
    FileType file_type;   /* Type du fichier de vote, cf enum FileType */
    char *file_name;      /* Nom du fichier de vote */
    bool has_log_file;    
    char *log_file;       /* Potentiel nom du fichier de log */
} Command;

/************
* fonctions *
************/

/**
 * @date 27/10/2023
 * @brief Interprète les arguments fournis
 * 
 * @param[in] argv Commande de lancement du programme
 * 
 * @return Informations extraites dans une structure @ref Command
*/
Command interprete(int argc,char* argv[]);

#endif