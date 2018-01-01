//
// Created by Quentin Nouvel on 06/12/2017.
//

#include <ctype.h>
#include <memory.h>
#include <math.h>
#include "types.h"
#include "valid.h"
#include "cipher.h"

//const byte DICO[] = "dictionnaire.txt";
//const int nbMot = 336531;
//const int tailleMaxMot = 30;

/*
byte** remplir_Dico(){
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
 */

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

bool* hash_dico(){
  unsigned long i;
  FILE* dico = fopen("dico.txt", "r");
  bool* hashed_dico = (bool*) calloc(4294967296, sizeof(bool));
  while (fscanf(dico, "%lu", &i) != EOF){
    hashed_dico[i] = true;
  }
  return hashed_dico;
}

bool isseparator(const char c){
  return (c == 32 || c == 33 || c == 44 || c == 46 || c == 63 || c == 10);
}

byte** parsing_prof(int lentar, byte* tar, int* nb){
  int taille_liste_mot = 20;
  int ind_mot = 0;
  int ind_curseur = 0;
  int taille_mot = 30;
  int taille_current_mot = 0;
  byte* mot = NULL;
  byte** liste_mot = (byte**) malloc(taille_liste_mot* sizeof(byte*));
  for (int i = 0; i < lentar; ++i) {
    if (ind_curseur == 0){
      if (!isseparator(tar[i])){
        mot = (byte*) malloc(taille_mot*sizeof(byte));
        mot[ind_curseur++] = (byte) tolower(tar[i]);
      }
    }else {
      if (ind_curseur == taille_mot){
        taille_mot += 10;
        mot = (byte*) realloc(mot, taille_mot* sizeof(byte));

      }
      if (isseparator(tar[i])){
        taille_current_mot = (int) strlen((const char *) mot);
        liste_mot[ind_mot++] = mot;
        ind_curseur = 0;
        taille_mot = 30;
        if (taille_current_mot > 4 || taille_current_mot < 2){
          liste_mot[--ind_mot] = NULL;
        }
      }else {
        mot[ind_curseur++] = (byte) tar[i];
      }
    }
    if (ind_mot+2 >= taille_liste_mot){
      taille_liste_mot += 10;
      liste_mot = (byte**) realloc(liste_mot, taille_liste_mot*sizeof(byte*));
    }
  }
  *nb = ind_mot;
  return liste_mot;
}


bool verif_dico(unsigned long int i, bool* hashed_dico){
  return hashed_dico[i];
}

int text_score(byte** liste_mot, int nb_mot, bool* hash_dico){
  int score = 0;
  for (int i = 0; i < nb_mot ; ++i) {
    if (verif_dico(hash_mot(liste_mot[i]), hash_dico)){
      score++;
    }
  }
  return score;
}


int C3(int lenkey, int lentar, byte* tar){
  int nb;
  int nb_mot_cle = 0;
  int score = 0;
  int current_best_score = -1;
  byte* current_text;
  byte* current_best_key = NULL;
  byte** liste_key = buildkey(lenkey, lentar, tar, &nb);
  if (liste_key == NULL) return 1;
  bool* hashed_dico = hash_dico();
  byte** liste_mot;
  for (int i = 0; i < nb; ++i) {
    current_text = XorcipherCopy(lenkey, liste_key[i], lentar, tar);
    liste_mot = parsing_prof(lentar, current_text, &nb_mot_cle);
    score = text_score(liste_mot, nb_mot_cle, hashed_dico);
    if (score > current_best_score){
      current_best_score = score;
      current_best_key = liste_key[i];
    }
    free(current_text);
    libDoublePointeur(liste_mot, nb_mot_cle);
  }
  for (int j = 0; j < lenkey; ++j) {
    printf("%c", current_best_key[j]);
  }
  printf("\n");
  libDoublePointeur(liste_key, nb);
  free(hashed_dico);
  return 0;
}