#include <stdio.h>
#include <stdlib.h>


typedef unsigned char byte;

void xorcipher(int lenkey, const byte *key, int lentar,byte *tar){
  //vestige du passé
  for (int i = 0; i < lentar; ++i) {
    tar[i] ^= key[i%lenkey];
  }
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
  int c;
  while ((c = fgetc(file)) != EOF)
  {

    c ^= key[(*len)%lenkey];
    fputc(c, sortie);
    ++(*len);
  }
}
