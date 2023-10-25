/** @file logger.h
 * @brief header du logger
 * @author Laforge Mateo
 * @note la sortie par défaut du logger est stdout
*/
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>

/** @date 13 octobre 2023
 * @brief écrit dans la sortie du logger
 * @param[in] msg le méssage à écrire
*/
void printl(char* msg);

/**
 * @date 20/10/2023
 * @brief Affiche un message d'erreur dans le logger
 * 
 * @param[in] file_name nom du fichier appelant
 * @param[in] fun_name nom de la fonction appelante
 * @param[in] message message à afficher
 * @remarks le message d'erreur pourra prendre une forme dépendante
 *  de la sortie actuelle du logger e.g. message en rouge pour stdout
 *  caractères particulier dans un fichier
 */
void warnl(char* file_name, char* fun_name, char* message );

/**
 * @date 20/10/2023
 * @brief Exit en affichant un message et la valeur de retour dans le logger
 * 
 * @param[in] file_name nom du fichier appelant
 * @param[in] fun_name nom de la fonction appelante
 * @param[in] message message à afficher
 * @param[in] exit_value valeur de retour
 */
void exitl(char* file_name, char* fun_name, char* message , int exit_value);

/** @date 13 octobre 2023
 * @brief définit la sortie appropriée du logger
 * @param[in] out la nouvelle sortie du logger
 * @remark en mode DEBUG cette fonction ne fait rien
*/
void redirect(FILE* out);

#endif