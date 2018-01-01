//
// Created by Quentin Nouvel on 29/11/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

byte* copyfile(FILE* file, int* lentar){
  int taille = 500;
  int i = 0;
  byte c;
  byte* copy = (byte*) malloc(taille*sizeof(byte));
  while (fscanf(file, "%c", &c) != EOF){
    if (i > taille-3){
      taille += 100;
      copy = (byte*)realloc(copy, taille*sizeof(byte));
    }
    copy[i] = c;
    i++;
  }
  copy[i] = (byte)"\0";
  *lentar = i;
  return copy;
}