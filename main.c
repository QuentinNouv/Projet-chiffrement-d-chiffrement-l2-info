#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "cipher.h"
#include "valid.h"

int verifOptions(char* i, char* o, char* k, long l, const char* m, FILE* i_file, FILE* o_file){
  // Return -1 si pas bon
  if (i != NULL){
    //Ouvre le fichier si ne peut pas ferme le programme
    i_file = fopen(i, "r");
    if (i_file == NULL){
      fprintf(stderr, "ERROR : Le fichier %s n'est pas lisible ou n'existe pas.\n", i);
      return -1;
    }
    if ((k != NULL && o != NULL && l == -1 && m == NULL)){
      if (!IskeyValid((byte *) k, (int) strlen(k))){
        fprintf(stderr, "ERROR : La clé %s n'est pas valide.\n", k);
        return -1;
      }
      o_file = fopen(o, "w");
      if (o_file == NULL){
        fprintf(stderr, "ERROR : Le fichier %s n'est pas être completé?\n", o);
      }
      return 1;
    } else if ((k == NULL && o == NULL && m != NULL)) {
      /* Code 11   : C1   + l, 10   : C1   sans l
       *      21   : C2   + L, 20   : C2   sans l
       *      311  : C3.1 + L, 310  : C3.1 sans l
       *      321  : C3.2 + L, 320  : C3.2 sans l
       */
      int choix = 0;
      if (m == "C1") choix = 10;
      else if (m == "C2") choix = 20;
      else if (m == "C3.1") choix = 310;
      else if (m == "C3.2") choix = 320;
      else {
        fprintf(stderr, "ERROR : Le critère de cassage du  chiffrage est incorrect.\n");
      }
      if (l > 0) {
        ++choix;
        return choix;
      }else return choix;
    }
    fprintf(stderr, "ERROR : Une erreur innatendu est intervenue.\n");
    return -1;
  }
  fprintf(stderr, "ERROR : Le paramètre -i est obligatoire et a besoin d'un argument .\n");
  return -1;
}

void LancementOption(int mode, FILE* i, FILE* o, char k){
  // mode -1 erreur, 1 chiffrage
  switch (mode)
  {
    case 1:
      // TODO : fontions de remplissage de fichier par texte chiffrer
    case 10:
      //TODO for 3 -> 7 into fonction de cassage C1
    case 11:
      //TODO Fonction de cassage C1
    case 20:
      //TODO for 3 -> 7 into fonction de cassage C2
    case 21:
      //TODO Fonction de cassage C2
    case 320:
      //TODO for 3 -> 7 into fonction de cassage C3.2
    case 321:
      //TODO Fonction de cassage C3.2
    case 310:
      //TODO for 3 -> 7 into fonction de cassage C3.1
    case 311:
      //TODO Fonction de cassage C3.1
    case -1:
      fprintf(stderr,"Fin du programme.\n");
          abort();
    default:
      fprintf(stderr, "Bruh\n");//TODO : message d'erreur
          abort();
  }
}

void option(int argc, char** argv){
  int c;
  char* i = NULL;
  FILE* i_file = NULL;
  char* o = NULL;
  FILE* o_file = NULL;
  char* k = NULL;
  long l = -1;
  char* m = NULL;
  while((c = getopt(argc, argv, "i:o:k:l:")) != -1) {
    switch (c)
      {
      case 'i':
        i = optarg;
        break;
      case 'o':
        o = optarg;
        break;
      case 'k':
        k = optarg;
        break;
      case 'l':
        l = strtol(optarg, NULL, 10);
        break;
      case 'm':
        m = optarg;
        break;
      case '?':
        if (optopt == 'c')
          fprintf(stderr, "L'option -%c à besoin d'un argument.\n", optopt);
        else if (isprint(optopt))
          fprintf(stderr, "Option inconnue `-%c'.\n", optopt);
        else
          fprintf(stderr,"Caractère inconnue`\\x%x'.\n", optopt);
        abort();
      default:
        abort();
      }
    }
  int Instruction = verifOptions(i, o, k, l, m, i_file, o_file);
  LancementOption(Instruction, i_file, o_file, (char) k);
}

int main(int argc, char **argv) {
  option(argc, argv);
  byte entree[] = "Bonjour je m'appelle dzqdqd !TEst gd ceg cq qzcgc qcg ?";
  byte key[] = "mdr";
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
