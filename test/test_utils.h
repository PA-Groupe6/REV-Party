#ifndef __TEST_H__
#define __TEST_H__

/* Réinitialise la couleur de l'affichage */
#define RSTC "\033[0m"
/* Affiche en gras */
#define BOLD "\033[1m"
/* Couleur standard des fichiers sources */
#define SRCC "\033[38;5;189m"
/* Couleur standard des fichiers test */
#define TSTC "\033[38;5;228m"
/* Couleur d'erreur (même que gcc fatal error) */
#define ERRC "\033[38;5;160m"

/* Couleur standard d'annonce d'exécution */
#define EXECC "\033[38;5;117m"
/* Couleur de fond d'un succès */
#define SUCCC "\033[48;5;2m"
/* Couleur de fond d'un échec */
#define FAILC "\033[48;5;124m"

/**
 * @date 26/10/2023
 * @brief Construit une chaine de caractère multiligne par ajout successifs
 * @note Ne pas oublier de vidé avec `emptyStringBuilder`
 */
typedef struct s_string_builder {
    char output[2048];
    int lastLine;
} StringBuilder;

/**
 * @date 26/10/2023
 * @brief Créer un StringBuilder
 * @return Un pointer vers le StringBuilder créé
 */
StringBuilder* createStringBuilder();

/**
 * @date 26/10/2023
 * @brief vide la chaîne construite jusqu'ici par un StringBuilder
 * @param[in] string_builder Le StringBuilder à vider
 */
void emptyStringBuilder(StringBuilder* string_builder);

/**
 * @date 26/10/2023
 * @brief Ajoute une ligne à un StringBuilder
 * 
 * @param[in] string_builder Le StringBuilder sur lequel faire l'ajout
 * @param[in] line La ligne à ajouter
 */
void addLineStringBuilder(StringBuilder* string_builder, char* line);

/**
 * @date 26/10/2023
 * @brief Affiche un StringBuilder dans la sortie standard
 * @param string_builder Le StringBuilder à afficher
 */
void printStringBuilder(StringBuilder* string_builder);

/**
 * @date 26/10/2023
 * @brief Affiche un message formaté de succès contenant le nom tu test passé
*/
void printSuccess(char* test_name);

/**
 * @date 26/10/2023
 * @brief Affiche un message formaté d'échec contenant le nom tu test échoué
*/
void printFailure(char* test_name);

#endif