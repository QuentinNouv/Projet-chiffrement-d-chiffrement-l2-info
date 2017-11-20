#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cipher.h"
#include "valid.h"

typedef unsigned char byte;

int main(int argc, char const *argv[]) {
  byte entree[] = "Bonjour je m'appelle bite !hebv dba cbe ?ndb cheyt";
  byte key[] = "mdr";
  printf("Key : %s\nEntree : '%s'\n", key, entree);
  xorcipher(3, key, 50, entree);
  printf("Après chiffrage : début'%s'fin\n", entree);
  byte** test = keygen(3, 50, entree);
  for (int i = 0; i < 3; i++) {
    printf("%s\n", test[i]);
  }
  return 0;
}
