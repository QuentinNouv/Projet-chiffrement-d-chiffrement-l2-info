#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "cipher.h"
#include "valid.h"
#include "copy.h"
#include "freq.h"


int VerifOptions(char* i, char* o, byte* k, long l, const char* m, FILE* i_file, FILE* o_file){
  // Return -1 si pas bon
  if (i != NULL){
    //Ouvre le fichier si ne peut pas ferme le programme
    i_file = fopen(i, "r");
    if (i_file == NULL){
      fprintf(stderr, "ERROR : Le fichier %s n'est pas lisible ou n'existe pas.\n", i);
      return -1;
    }
    fclose(i_file);
    if ((k != NULL && o != NULL && l == -1 && m == NULL)){
      if (!IskeyValid(k, (int) strlen((char*)k))){
        fprintf(stderr, "ERROR : La clé %s n'est pas valide.\n", k);
        return -1;
      }
      o_file = fopen(o, "w");
      if (o_file == NULL){
        fprintf(stderr, "ERROR : Le fichier %s ne peut être édité.\n", o);
        return -1;
      }
      fclose(o_file);
      return 1;
    } else if ((k == NULL && o == NULL && m != NULL)) {
      /* Code 11   : C1   + l, 10   : C1   sans l
       *      21   : C2   + L, 20   : C2   sans l
       *      311  : C3.1 + L, 310  : C3.1 sans l
       *      321  : C3.2 + L, 320  : C3.2 sans l
       */
      int choix = 0;
      if (!(strcmp(m, "C1"))) choix = 10;
      else if (!(strcmp(m, "C2"))) choix = 20;
      else if (!(strcmp(m, "C3.1"))) choix = 310;
      else if (!(strcmp(m, "C3.2"))) choix = 320;
      else {
        fprintf(stderr, "ERROR : Le critère de cassage du  chiffrage est incorrect.\n");
        return -1;
      }
      if (l > 0) {
        ++choix;
        return choix;
      }else return choix;
    }
    fprintf(stderr, "ERROR : Une erreur inattendu est intervenue.\n");
    return -1;
  }
  fprintf(stderr, "ERROR : Le paramètre -i est obligatoire et a besoin d'un argument .\n");
  return -1;
}

void LancementOption(int mode, char*  i, char* o, byte* k, int l){
  // mode -1 erreur, 1 chiffrage
  /* Code 11   : C1   + l, 10   : C1   sans l
   *      21   : C2   + L, 20   : C2   sans l
   *      311  : C3.1 + L, 310  : C3.1 sans l
   *      321  : C3.2 + L, 320  : C3.2 sans l
   */
  int len, lentar;
  FILE* file_i;
  byte* tar;
  switch (mode)
  {
    case 1:
      xorcipher2(i, &len, o, (int)strlen((char*) k), k);
      break;
    case 10:
      file_i = fopen(i, "r");
      tar = copyfile(file_i, &lentar);
      for (int lenkey = 3; lenkey < 8 ; ++lenkey) {
        printf("Solution pour les clé de longueur %d : \n", lenkey);
        C1(lenkey, lentar, tar);
      }
    case 11:
      file_i = fopen(i, "r");
      tar = copyfile(file_i, &lentar);
      C1(l, lentar, tar);
      break;
    case 20:
      file_i = fopen(i, "r");
      tar = copyfile(file_i, &lentar);
      for (int lenkey = 3; lenkey < 8; ++lenkey) {
        C2(lenkey, lentar, tar);
      }
      break;
    case 21:
      file_i = fopen(i, "r");
      tar = copyfile(file_i, &lentar);
      C2(l, lentar, tar);
      break;
    case 320:
      //TODO for 3 -> 7 into fonction de cassage C3.2
    case 321:
      break;
      //TODO Fonction de cassage C3.2
    case 310:
      break;
      //TODO for 3 -> 7 into fonction de cassage C3.1
    case 311:
      break;
      //TODO Fonction de cassage C3.1
    case -1:
      fprintf(stderr,"Fin du programme.\n");
          exit(-1);
    default:
      fprintf(stderr, "ERROR : INCONNU\n");//TODO : message d'erreur
      exit(-1);
  }
}

void option(int argc, char** argv){
  int c;
  char* i = NULL;
  FILE* i_file = NULL;
  char* o = NULL;
  FILE* o_file = NULL;
  byte* k = NULL;
  long l = -1;
  char* m = NULL;
  while((c = getopt(argc, argv, "i:o:k:l:m:")) != -1) {
    switch (c) {
      case 'i':
        i = optarg;
        break;
      case 'o':
        o = optarg;
        break;
      case 'k':
        k = (byte *) optarg;
        break;
      case 'l':
        l = strtol(optarg, NULL, 10);
        break;
      case 'm':
        m = optarg;
        break;
      case '?':
        if (optopt == 'c'){
          fprintf(stderr, "L'option -%c à besoin d'un argument.\n", optopt);
          exit(-1);
        }
        else if (isprint(optopt)) {
          fprintf(stderr, "Option inconnue `-%c'.\n", optopt);
          exit(-1);
    }else{
          fprintf(stderr,"Caractère inconnue`\\x%x'.\n", optopt);
          exit(-1);
      }
      default:
        exit(-1);
      }
    }
  int Instruction = VerifOptions(i, o, k, (int) l, m, i_file, o_file);
  LancementOption(Instruction, i, o, k, (int) l);
}


int main(int argc, char **argv) {
  option(argc, argv);/*
  byte* test;
  int nb = 0;
  FILE* file = fopen("../../tests/crypted/df9_bovary-isolatin1.txt", "r");
  test = copyfile(file, &nb);
  C3(3, nb, test);*/
  return 0;
}
