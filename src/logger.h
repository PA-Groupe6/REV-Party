/** \file logger.h
 * \brief header du logger
 * \author Laforge Mateo
 * \note la sortie par défaut du logger est stdout
*/
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>

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

#endif