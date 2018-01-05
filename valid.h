//
// Created by Quentin Nouvel on 28/11/2017.
//

#ifndef C_VALID_H
#define C_VALID_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "types.h"

bool is_key_valid(byte *key, int lenkey);
byte** buildkey(int lenkey, int lentar, byte *tar,int* nb);
void lib_double_pointeur(byte** pointeur, int len_pointeur);
int C1(int lenkey, int lentar, byte* tar);

byte **buildkey_opti(int lenkey, int lentar, byte *tar, int *nb);

#endif //C_VALID_H
