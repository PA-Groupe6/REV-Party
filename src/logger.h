/** \file logger.h
 * \brief header du logger
 * \author Laforge Mateo
 * \note la sortie par défaut du logger est stdout
*/
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <stdlib.h>

/** \date 13 octobre 2023
 * \brief écrit dans la sortie du logger
 * \param[in] msg le méssage à écrire
*/
void printl(char* msg);

/** \date 13 octobre 2023
 * \brief définit la sortie appropriée du logger
 * \param[in] out la nouvelle sortie du logger
 * \remark en mode DEBUG cette fonction ne fait rien
*/
void redirect(FILE* out);

/**
 * @date 20/10/2023
 * @brief Exit en affichant un message et la valeur de errno dans le logger
 * 
 * @param message message à afficher
 * @param exit_value valeur de retour
 */
void exitl(char* file_name, char* fun_name, char* message , int exit_value);


/**
 * @date 20/10/2023
 * @brief Affiche un message d'erreur en rouge dans le logger
 * 
 * @param message message à afficher
 */
void warnl(char* file_name, char* fun_name, char* message );

#endif