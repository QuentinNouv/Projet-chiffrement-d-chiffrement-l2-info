//
// Created by Quentin Nouvel on 06/12/2017.
//

#ifndef C_DICO_H
#define C_DICO_H

#include "types.h"
#include "valid.h"
#include "cipher.h"

void C3(int lenkey, int lentar, byte* tar);
byte** ParsingTar(int lentar, byte* tar, int* nb);
byte** parsing_prof(int lentar, byte* tar, int* nb);
#endif //C_DICO_H
