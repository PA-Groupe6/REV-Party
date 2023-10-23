/** \file interpreteur.h
 * \brief header de l'interpreteur de commande
 * \author Ludwig Corentin
*/


typedef struct commande
{
    int module;         /*module appeler, cf voir dessous*/
    int type_fichier;   /*type du fichier de vote, cf voir dessous*/
    char *fichier;      /*nom du fichier de vote*/
    char *log;          /*potentiel nom du fichier de log*/
};

/********
* module *
**********/

#define UNI1 1
#define UNI2 2
#define MINIMAX 3
#define RANGEMENT 4
#define SCHULZE 5
#define JUGEMENT_MAJORITAIRE 6
#define ALL 7


/*******************
 * type de fichier *
********************/

#define BALLOT 9
#define MATRICE_DUEL 10

/************
 * fonction *
*************/
/** \brief Crée une structure commande
    \param[in] argv Commande de lancement du programme
    \return une structure commande
    \date  23/10/2023
*/
commande intreprete(char* argv[]);