/**
 * @file label_test_set.h
 * @author Ugo VALLAT
 * @brief Jeu de test de label et fonction permettant de les charger
 * 
 */

#ifndef __SET_LABEL__
#define __SET_LABEL__

#include "../../src/structure/genericlist.h"


#define NB_LABELS 5     /* Nombre de labels / colonnes */


/**
 * @date 25/11/2023
 * @brief Génère la liste générique contenant les labels du jeu de test id_label
 * 
 * @param id_label identifiant du jeu de test
 * @pre 1<= id_label <= 5
 * @return Liste des labels
 * @note Ne pas free les labels (uniquement durant les tests)
 */
GenList* loadLabelsInList(unsigned id_label);








#endif