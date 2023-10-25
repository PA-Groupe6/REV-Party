/** \file interpreter.h
 * \brief header de l'interpreteur de commande
 * \author Ludwig Corentin
*/

/** @date 25 Octobre 2023
 * @brief fait référence à un module de calcul de vaiqueur
 */
typedef enum module_e {
    UNI1, UNI2, MINIMAX, RANGEMENT, SCHULZE, JUGEMENT_MAJORITAIRE, ALL
} Module;


/** @date 25 Octobre 2023
 * @brief fait référence au type de fichier csv renseigné par l'utilisateur
 */
typedef enum file_type_e {
    BALE, DUEL
} FileType;

/** @date 25 Octobre 2023
 * @brief structure contenant toutes les informations nécéssaire au calcul du vainqueur
 */
typedef struct commande_t {
    Module module;         /*module appeler, cf voir dessous*/
    FileType type_fichier;   /*type du fichier de vote, cf voir dessous*/
    char *fichier;      /*nom du fichier de vote*/
    char *log;          /*potentiel nom du fichier de log*/
} Commande;

/************
* fonctions *
************/

/** \brief Crée une structure commande
    \param[in] argv Commande de lancement du programme
    \return une structure commande
    \date  23/10/2023
*/
Commande intreprete(int argc,char* argv[]);