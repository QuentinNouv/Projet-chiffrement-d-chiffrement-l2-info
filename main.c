#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cipher.h"
#include "valid.h"

typedef unsigned char byte;

int main(int argc, char const *argv[]) {
  byte entree[] = "Bonjour je m'appelle bite !TEst gd ceg cq qzcgc qcg ?";
  byte key[] = "mdr";
  printf("Key : %s\nEntree : '%s'\n", key, entree);
  xorcipher(3, key, 27, entree);
  printf("Après chiffrage : début'%s'fin\n", entree);
  int nb;
  byte** test = buildkey(3, 27, entree,&nb);
  for (int i = 0; i < nb; i++) {
    printf("%s\n", test[i]);
  }
  libDoublePointeur(test, 3);
  return 0;
}
