//
// Created by Quentin Nouvel on 04/12/2017.
//
#include <math.h>
#include "types.h"
#include "valid.h"
#include "cipher.h"

#define Nb_L_Alphabet 26
#define MaxChar_L 30

const float freq_th[Nb_L_Alphabet] = {9.42, 1.02, 2.64, 3.39, 15.87, 0.95, 1.04, 0.77, 8.41, 0.89, 0.00, 5.34, 3.24, 7.15, 5.14, 2.86, 1.06, 6.46, 7.90, 7.26, 6.24, 2.15, 0.00, 0.30, 0.24, 0.32};
byte alphabet[Nb_L_Alphabet][MaxChar_L] = {"aAàâäÀÂÄ", "bB", "cCçÇ", "dD", "eEèéêëÈÉÊË", "fF", "gG", "hH", "iIîïÎÏ", "jJ", "kK", "lL", "mM", "nN", "oOôöÔÖ", "pP", "qQ", "rR", "sS", "tT","uUùûÙÛ", "vV", "wW", "xX", "yY", "zZ"};

void libDoublePointeurFLoat(float** pointeur, int lenPointeur) {
  for (int i = 0; i < lenPointeur; i++) {
    free(pointeur[i]);
  }
  free(pointeur);
}

int index_lettre(byte carac){
  int j = 0;
  char temp;
  for (int i = 0; i < Nb_L_Alphabet; ++i) {
    temp = alphabet[i][0];
    while (temp != '\0'){
      if (temp == carac){
        return i;
      }
      temp = alphabet[i][++j];
    }
    j = 0;
  }
  return -1;
}

float* Calc_Freq(byte* tar, int lentar){
  int ind_Lettre;
  int nb_lettre = 0;
  float* freq = calloc(Nb_L_Alphabet, sizeof(float));
  for (int i = 0; i < lentar; ++i) {
    ind_Lettre = index_lettre(tar[i]);
    if (ind_Lettre != -1){
      freq[ind_Lettre] += 1;
      ++nb_lettre;
    }
  }
  for (int j = 0; j < Nb_L_Alphabet; ++j) {
    freq[j] /= nb_lettre;
    freq[j] *= 100;
  }
  return freq;
}

float Calcul_Prox(float* freq){
  float prox = 0;
  for (int i = 0; i < Nb_L_Alphabet; ++i) {
    prox += powf(freq_th[i]-freq[i], 2);
  }
  return prox;
}

int C2(int lenkey, int lentar, byte* tar){
  int nb;
  float** tab_freq;
  float temp_min_prox = 0;
  float temp_prox;
  byte* temp_key = NULL;
  byte** liste_key = buildkey(lenkey, lentar, tar, &nb);
  if (liste_key == NULL) return 1;
  tab_freq = (float**) malloc(nb*sizeof(float*));
  for (int i = 0; i < nb; ++i) {
    xorcipher(lenkey, liste_key[i], lentar, tar);
    tab_freq[i] = Calc_Freq(tar, lentar);
    if (i == 0){
      temp_key = liste_key[i];
      temp_min_prox = Calcul_Prox(tab_freq[i]);
    } else {
      temp_prox = Calcul_Prox(tab_freq[i]);
      if (temp_prox < temp_min_prox){
        temp_key = liste_key[i];
        temp_min_prox = Calcul_Prox(tab_freq[i]);
      }
    }
    //printf("]\n%s %s %f %f \n", liste_key[i], temp_key, temp_min_prox, temp_prox);
    xorcipher(lenkey, liste_key[i], lentar, tar);
  }
  printf("%s\n", temp_key);
  libDoublePointeurFLoat(tab_freq, nb);
  libDoublePointeur(liste_key, nb);
  return 0;
}
