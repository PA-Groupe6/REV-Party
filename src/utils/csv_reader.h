/**
 * @file csv_reader.h
 * @brief Header du lecteur de csv
 * @author LUDWIG Corentin
*/
#ifndef __CSV_READER__H__
#define __CSV_READER__H__

#include "duel.h"
#include "bale.h"


/**
 * @date 30/10/2023
 * @brief Remplit une matrice de duel appartir d'un fichier csv
 * 
 * @param[in] nom_file nom/path du fichier csv
 * 
 * @return une matrice @ref Duel
*/
Duel csvToDuel(char *file);


/**
 * @date 30/10/2023
 * @brief Remplit un ballot appartir d'un fichier csv
 * 
 * @param[in] nom_file nom/path du fichier csv
 * 
 * @return une matrice @ref Bale
*/
Bale csvToBale(char *file);


#endif