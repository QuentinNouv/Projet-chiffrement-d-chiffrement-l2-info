#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned char byte;

void xorcipher(int lenkey, byte *key, int lentar,byte *tar){
  for (int i = 0; i < lentar; ++i) {
    tar[i] ^= key[i%lenkey];
  }
}
