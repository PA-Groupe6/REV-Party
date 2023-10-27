/**
 * @file logger.h
 * @author LAFORGE Mateo
 * @brief Header de l'afficheur
 * @note La sortie par défaut du logger est stdout
*/
#ifndef __LOGGER_H__
#define __LOGGER_H__
#include <stdio.h>

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
 * @date 27/10/2023
 * @brief Définit la sortie appropriée du logger
 * @remark en mode DEBUG cette fonction ne fait rien
 * 
 * @param[in] out La nouvelle sortie du logger
*/
void redirect(FILE* out);

#endif