/**
 * @file logger.h
 * @author LAFORGE Mateo
 * @brief Header de l'afficheur
 * 
 * L'afficheur a pour rôle de fournir une interface
 * d'affichage formaté généralisée l'ensemble du projet
 * 
 * @remark La sortie par défaut du logger est stdout
*/
#ifndef __LOGGER__H__
#define __LOGGER__H__

/**
 * @date 29/10/2023
 * @brief Initialise le logger en définissant sa sortie sur un chemin
 * @remark en mode DEBUG la sortie sera toujours la valeur par défaut
 * 
 * @param[in] file_name Le chemin du fichier où écrira le logger
 * @note mettre NULL pour laisser la valeur par défaut (stdout)
*/
void init_logger(char* file_path);

/**
 * @date 27/10/2023
 * @brief Écrit dans la sortie du logger
 * 
 * @param[in] msg Le méssage à écrire
*/
void printl(char* msg);

/**
 * @date 27/10/2023
 * @brief Affiche un message d'erreur dans le logger
 * @remarks le message d'erreur pourra prendre une forme dépendante
 *  de la sortie actuelle du logger e.g. message en rouge pour stdout
 *  caractères particulier dans un fichier
 * 
 * @param[in] file_name Nom du fichier appelant
 * @param[in] fun_name Nom de la fonction appelante
 * @param[in] message Message à afficher
 */
void warnl(char* file_name, char* fun_name, char* message );

/**
 * @date 27/10/2023
 * @brief Exit en affichant un message et la valeur de retour dans le logger
 * 
 * @param[in] file_name Nom du fichier appelant
 * @param[in] fun_name Nom de la fonction appelante
 * @param[in] message Message à afficher
 * @param[in] exit_value Valeur de retour du programme
 */
void exitl(char* file_name, char* fun_name, char* message , int exit_value);

/**
 * @date 29/10/2023
 * @brief Ferme le logger
 * @remark Ne fais rien si la sortie est stdout
*/
void close_logger();

#endif