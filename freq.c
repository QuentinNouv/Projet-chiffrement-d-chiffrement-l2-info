//
// Created by Quentin Nouvel on 04/12/2017.
//
#include <math.h>
#include "types.h"
#include "valid.h"
#include "cipher.h"

#define Nb_L_Alphabet 26
//  #define MaxChar_L 30

const float freq_th[Nb_L_Alphabet] = {9.42, 1.02, 2.64, 3.39, 15.87, 0.95, 1.04, 0.77, 8.41, 0.89, 0.00, 5.34, 3.24, 7.15, 5.14, 2.86, 1.06, 6.46, 7.90, 7.26, 6.24, 2.15, 0.00, 0.30, 0.24, 0.32};
// byte alphabet[Nb_L_Alphabet][MaxChar_L] = {"aAàâäÀÂÄ", "bB", "cCçÇ", "dD", "eEèéêëÈÉÊË", "fF", "gG", "hH", "iIîïÎÏ", "jJ", "kK", "lL", "mM", "nN", "oOôöÔÖ", "pP", "qQ", "rR", "sS", "tT","uUùûÙÛ", "vV", "wW", "xX", "yY", "zZ"};

void libDoublePointeurFLoat(float** pointeur, int lenPointeur) {
  for (int i = 0; i < lenPointeur; i++) {
    free(pointeur[i]);
  }
  free(pointeur);
}
/* // Mdr ce caffard j'ai honte d'avoir fais ça
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
}*/

int index_lettre(byte carac){
  switch(carac){
    case 65 ... 90:
      return carac-65;
    case 97 ... 122:
      return carac-97;
    case 192 ... 196:
    case 224 ... 198:
      return 0;
    case 200 ... 203:
    case 232 ... 235:
      return 4;
    case 207 ... 207:
    case 238 ... 239:
      return 8;
    case 217 ... 219:
    case 249 ... 251:
      return 20;
    case 212 ...214:
    case 244 ...246:
      return 14;
    case 199:
    case 231:
      return 2;
    default:
      return 27;
  }
}

float* Calc_Freq(byte* tar, int lentar){
  int nb_lettre = 0;
  float* freq = calloc(Nb_L_Alphabet+1, sizeof(float));
  for (int i = 0; i < lentar; ++i) {
      freq[index_lettre(tar[i])] += 1;
      ++nb_lettre;
  }
  for (int j = 0; j < Nb_L_Alphabet; ++j) {
    freq[j] /= lentar-freq[27];
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
  byte* current_text;
  byte* temp_key = NULL;
  byte** liste_key = buildkey(lenkey, lentar, tar, &nb);
  if (liste_key == NULL) return 1;
  tab_freq = (float**) malloc(nb*sizeof(float*));
  for (int i = 0; i < nb; ++i) {
    current_text = XorcipherCopy(lenkey, liste_key[i], lentar, tar);
    tab_freq[i] = Calc_Freq(current_text, lentar);
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
    free(current_text);
  }
  for (int j = 0; j < lenkey; ++j) {
    printf("%c", temp_key[j]);
  }
  printf("\n");
  libDoublePointeurFLoat(tab_freq, nb);
  libDoublePointeur(liste_key, nb);
  return 0;
}
