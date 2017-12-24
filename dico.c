//
// Created by Quentin Nouvel on 06/12/2017.
//
#include <ctype.h>
#include <memory.h>
#include <math.h>
#include "types.h"
#include "valid.h"
#include "cipher.h"
const byte DICO[] = "dictionnaire.txt";
const int nbMot = 336531;

const int tailleMaxMot = 30;

byte** RemplirDico(){
  int nbMot_act = 0;
  FILE* dico_f = fopen((const char *) DICO, "r");
  byte** dico = (byte**) malloc(nbMot* sizeof(byte*));
  dico[0] = (byte*) malloc(tailleMaxMot* sizeof(byte));
  while(fscanf(dico_f, "%s", dico[nbMot_act]) == 1){
    dico[++nbMot_act] = (byte*) malloc(tailleMaxMot*sizeof(byte));
  }
  dico[nbMot] = (byte*) "\n";
  fclose(dico_f);
  return dico;
}

unsigned long int hash(byte c, int i){
  return (unsigned long int) ((int) c * (pow(c, i)));
}

unsigned long int hash_mot(byte* word){
  unsigned long int word_value = 0;
  for (int i = 0; word[i] != '\0'; ++i) {
    word_value += hash(word[i], i);
  }
  return word_value;
}

bool* hash_dico(byte** dico){
  bool* hashed_dico = (bool*) calloc(4294967296, sizeof(bool));
  for (int i = 0; i < 336531; ++i) {
    if (strlen((const char *) dico[i]) < 4){
      hashed_dico[hash_mot(dico[i])] = true;
    }
  }
  return hashed_dico;
}

bool LettreMot(byte carac){
  switch (carac){
    case 97 ... 122:
      return true;
    case 224 ... 228:
      return true;
    case 232 ... 235:
      return true;
    case 238 ... 239:
      return true;
    case 249 ... 251:
      return true;
    case 244 ...246:
      return true;
    case 231:
      return true;
    default:
      return false;
  }
  return false;
}

bool LettreMotDebut(byte carac){
  //All Lettre/accent/MAJ
  switch (carac){
    case 65 ... 90:
      return true;
    case 97 ... 122:
      return true;
    case 192 ... 196:
    case 224 ... 228:
      return true;
    case 200 ... 203:
    case 232 ... 235:
      return true;
    case 207 ... 207:
    case 238 ... 239:
      return true;
    case 217 ... 219:
    case 249 ... 251:
      return true;
    case 212 ...214:
    case 244 ...246:
      return true;
    case 199:
    case 231:
      return true;
    default:
      return false;
  }
  return false;
}

byte** ParsingTar(int lentar, byte* tar, int* nb){
  int taille_liste_mot = 20;
  int ind_mot = -1;
  int ind_curseur = 0;
  bool maj_previous = false;
  byte** liste_mot = (byte**) malloc(taille_liste_mot* sizeof(byte*));
  for (int i = 0; i < lentar; ++i) {
    if (LettreMotDebut(tar[i]) && ind_curseur == 0){
      liste_mot[++ind_mot] = (byte*) malloc(tailleMaxMot*sizeof(byte));
      if ( tar[i] <= 90 && 65<= tar[i]){
        liste_mot[ind_mot][ind_curseur++] = (byte) tolower(tar[i]);
        maj_previous = true;
      } else{
        liste_mot[ind_mot][ind_curseur++] = (byte) tar[i];
        maj_previous = false;
      }
    } else if (maj_previous && LettreMotDebut(tar[i])){
      if ( tar[i] <= 90 && 65<= tar[i]){
        liste_mot[ind_mot][ind_curseur++] = (byte) tolower(tar[i]);
        maj_previous = true;
      } else{
        liste_mot[ind_mot][ind_curseur++] = (byte) tar[i];
        maj_previous = false;
      }
    } else if (LettreMot(tar[i]) && ind_curseur != 0){
      liste_mot[ind_mot][ind_curseur++] = (byte) tar[i];
    } else{
      if (ind_curseur != 0) {
        liste_mot[ind_mot][ind_curseur] = (byte) '\0';
        if (ind_curseur >= 5){
          //Si mot > 4 lettres ne compte pas donc revient sur le mort pour ecrire par dessus
          ind_mot--;
        }
        ind_curseur = 0;
      }
      if (ind_mot >= taille_liste_mot-1){
        taille_liste_mot += 10;
        liste_mot = (byte**) realloc(liste_mot, taille_liste_mot*sizeof(byte*));
      }
    }
  }
  *nb = ++ind_mot;
  return liste_mot;
}

bool verif_dico(unsigned long int i, bool* hash_dico){
  return hash_dico[i];
}

int text_score(byte** liste_mot, int nb_mot, bool* hash_dico){
  int score = 0;
  for (int i = 0; i < nb_mot ; ++i) {
    if (verif_dico(hash_mot(liste_mot[i]), hash_dico)){
      score++;
    }
  }
  //printf("%d\n", score);
  return score;
}

int C3(int lenkey, int lentar, byte* tar){
  int nb;
  int nb_mot_cle = 0;
  int score = 0;
  int current_best_score = 0;
  byte* current_best_key = NULL;
  byte* current_text;
  byte** liste_key = buildkey(lenkey, lentar, tar, &nb);
  if (liste_key == NULL) return 1;
  byte** dico = RemplirDico();
  bool* hashed_dico = hash_dico(dico);
  libDoublePointeur(dico, nbMot);
  byte** liste_mot;
  for (int i = 0; i < nb; ++i) {
    current_text = XorcipherCopy(lenkey, liste_key[i], lentar, tar);
    liste_mot = ParsingTar(lentar, current_text, &nb_mot_cle);
    for (int j = 0; j < nb_mot_cle; ++j) {
      printf("%s ", liste_mot[j]);
    }
    score = text_score(liste_mot, nb_mot_cle, hashed_dico);
    printf("%d %d\n", current_best_score, score);
    if (score >= current_best_score){
      current_best_score = score;
      current_best_key = liste_key[i];
    }
    free(current_text);libDoublePointeur(liste_mot, nb_mot_cle);
  }
  printf("%s", current_best_key);
  libDoublePointeur(liste_key, nb);
  free(hashed_dico);
  return 0;
}