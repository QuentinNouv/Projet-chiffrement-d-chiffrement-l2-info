#include <stdio.h>
#include <stdlib.h>
#include "types.h"

/*void xorcipher(int lenkey, const byte *key, int lentar, byte *tar){
  for (int i = 0; i < lentar; ++i) {
    tar[i] ^= key[i%lenkey];
  }
}*/

byte* XorcipherCopy(int lenkey, const byte *key, int lentar, const byte *tar){
  byte* res = (byte*) malloc(lentar* sizeof(byte));
  for (int i = 0; i < lentar; ++i) {
    res[i] = tar[i] ^ key[i%lenkey];
  }
  return res;
}

void xorcipher2(char* file_i, int* len, char* file_o, int lenkey, const byte *key){
  FILE* file = fopen(file_i, "r");
  FILE* sortie = fopen(file_o, "w");
  *len = 0;
  if(file == NULL){
    fprintf(stderr,"ERROR : Fichier %s illisible.", file_i);
    abort();
  }if(sortie == NULL){//Inutile avec "w" :')
    fprintf(stderr,"ERROR : Fichier %s non éditable.", file_o);
    abort();
  }
  char c;
  while ((fscanf(file, "%c", &c)) != EOF)
  {
    c ^= key[(*len)%lenkey];
    fputc(c, sortie);
    ++(*len);
  }
}