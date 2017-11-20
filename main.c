#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cipher.h"

int main(int argc, char const *argv[]) {
  byte entree[] = "lol";
  byte key[] = "mdr";
  xorcipher(3, key, 3, entree);
  printf("%s\n", entree);
  xorcipher(3, key, 3, entree);
  printf("%s\n", entree);
  return 0;
}
