//
// Created by Quentin Nouvel on 28/11/2017.
//

#ifndef C_VALID_H
#define C_VALID_H

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool IskeyValid(byte* key, int lenkey);
byte** buildkey(int lenkey, int lentar, byte *tar,int* nb);
void libDoublePointeur(byte** pointeur, int lenPointeur);
void C1(int lenkey, int lentar, byte* tar);
bool ValidKeyChar(byte key);

#endif //C_VALID_H
