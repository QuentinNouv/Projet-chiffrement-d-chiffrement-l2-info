#ifndef C_CIPHER_H
#define C_CIPHER_H

#include "types.h"
#include <stdio.h>
#include <stdlib.h>

byte* XorcipherCopy(int lenkey, const byte *key, int lentar, const byte *tar);
void xorcipher2(char* file_i, int* len, char* file_o, int lenkey, const byte *key);

#endif //C_CIPHER_H
