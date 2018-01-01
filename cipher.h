#ifndef C_CIPHER_H
#define C_CIPHER_H

#include <stdio.h>
#include <stdlib.h>

#include "types.h"

byte* xorciphercopy(int lenkey, const byte *key, int lentar, const byte *tar);
int xorcipher2(char* file_i, int* len, char* file_o, int lenkey, const byte *key);

#endif //C_CIPHER_H
