#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "cipher.h"
#include "valid.h"
#include "copy.h"
#include "freq.h"
#include "dico.h"


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
       *      31   : C3.1
       *      32   : C3.2,
       */
      int choix = 0;
      if (!(strcmp(m, "1"))) choix = 10;
      else if (!(strcmp(m, "2"))) choix = 20;
      else if (!(strcmp(m, "3.1"))) choix = 31;
      else if (!(strcmp(m, "3"))) choix = 32;
      else {
        fprintf(stderr, "ERROR : Le critère de cassage du  chiffrage est incorrect.\n");
        return -1;
      }
      if (choix == 31){
        if (l < 1) {
          fprintf(stderr, "ERROR : Vous devez spécifiez une taille de clé valide pour l'option -m 31.\n");
          return -1;
        }
      }else if (choix == 32){
        if (l < 1) {
          fprintf(stderr, "ERROR : Si vous ne spécifiez pas de valeurs pour -m 32 la valeurs 7 est prise par reférence.\n");
        }
        return choix;
      }else if (l > 0) {
        ++choix;
        return choix;
      }
      return choix;
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
   *      31   : C3.1,
   *      32   : C3.2,
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
        //printf("Solution pour les clé de longueur %d : \n", lenkey);
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
    case 31:
      file_i = fopen(i, "r");
      tar = copyfile(file_i, &lentar);
      C3(l, lentar, tar);
      break;
    case 32:
      file_i = fopen(i, "r");
      tar = copyfile(file_i, &lentar);
      if (l < 1){
        l = 8;
      }
      for (int lenkey = 3; lenkey < l; ++lenkey) {
        C3(lenkey, lentar, tar);
      }
      break;
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
  option(argc, argv);
  /*byte* test;
  int nb = 0;
  int nb_mot;
  byte key[] = "aBbdN";
  FILE* file = fopen("../../tests/crypted_crack/adijd_mini.txt", "r");
  test = copyfile(file, &nb);
  fclose(file);
  xorcipher(5, key, nb, test);
  printf("%s  |||||\n", test);
  byte** mot = parsing_prof(nb, test, &nb_mot);
  for (int i = 0; i < nb_mot; ++i) {
    printf("%s ", mot[i]);
  }*/
  //byte key[] = "Ba5";
  //xorcipher(3, key, nb, test);
  //printf("\n%s", test);*/
  return 0;
}
