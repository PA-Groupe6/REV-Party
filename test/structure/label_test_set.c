

/**
 * @author Ugo VALLAT
 * @brief Jeu de test de label et fonction permettant de les charger
 * 
 */

#include <string.h>
#include "label_test_set.h"
#include "../../src/structure/genericlist.h"


/*
    ====================
    === JEUX DE TEST ===
    ====================
*/

char labels1[NB_LABELS][256] = {
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
    "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD",
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
};

char labels2[NB_LABELS][256] = {
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "CCCCCCCCCCCCCC",
    "D",
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
};

char labels3[NB_LABELS][256] = {
    "jbazHAHZjciehbnASIhb",
    "bHUVviuVUAPGugauGPYZVpyv",
    "pnnjOIBNnaibdoAOIPbnvwxCBLH",
    "jbjbiFAUUZYMPAJNojnamAOPOjo",
    "NVVAUEoIyiuIUAHGhAB"
};

char labels4[NB_LABELS][256] = {
    "azertyui op qsdfghjklmwxcvbn",
    "azerty     uiopqs  dfghjklmwxcvbn",
    "a z e r t y uiop  q s d fghj   klm wx cvbn",
    "a         zer t  y ui op q s d fg h j k  l m w x c v bn",
    "   az e rtyui     opqsdfghjklmwxc v bn",
};

char labels5[NB_LABELS][256] = {
    "Ajjabij IH AH hj A H H ihb açus h uA_\"\'(é\"=)ç(&é'\"çHI H uh A ",
    "  osnovu sbsoud s vcou hàuvc spiu&égbpshvcoiuusv piusbsd;:à\"éçviusdvud vu",
    "102760 DUG07GDgèGU 27°èg<>>>>µ£P+£°§/.?ljk onzax",
    "*/+/sl jo spo jlq,:n,;c uipzb_hi97 T3672R F5mlk nln",
    "@\\#~#|`[[{@]@\\~{`{|]~}`¤}}]]"
};

/**
 * @date 25/11/2023
 * @brief Charge le tableau dans la liste
 * 
 * @param labels tableau de labels
 * @return Liste des labels
 */
GenList* addToGenList(char labels[NB_LABELS][256]) {
    GenList* l = createGenList(5);
    for(unsigned i = 0; i < NB_LABELS; i++)
        genListAdd(l, labels[i]);
    return l;
}

GenList* loadLabelsInList(unsigned id_label) {
    switch(id_label) {
        case 1:
            return addToGenList(labels1);
        case 2:
            return addToGenList(labels2);
        case 3:
            return addToGenList(labels3);
        case 4:
            return addToGenList(labels4);
        case 5:
            return addToGenList(labels5);
    }
    return NULL;
}
