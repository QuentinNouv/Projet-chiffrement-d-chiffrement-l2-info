//
// Created by Quentin Nouvel on 06/12/2017.
//

#include "types.h"
#include "valid.h"
#include "cipher.h"
const byte DICO[] = "dictionnaire.txt";
const int nbMot = 336531;

const int tailleMaxMot = 25;

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

bool LettreMot(byte carac){
  //All Lettre/accent
  return true;
}

bool LettreMotDebut(byte carac){
  //All Lettre/accent/MAJ
  return true;
}

byte** ParsingTar(int lentar, byte* tar, int* nb){
  int taille_liste_mot = 10;
  int ind_mot = -1;
  int ind_curseur = 0;
  byte** liste_mot = (byte**) malloc(taille_liste_mot* sizeof(byte*));
  liste_mot[0] = (byte*) malloc(tailleMaxMot*sizeof(byte));
  for (int i = 0; i < lentar; ++i) {
    if (ind_curseur == 0){
      if (LettreMotDebut(tar[i])){
        ind_mot++;
        liste_mot[ind_mot][ind_curseur++] = tar[i];
      }
    }else {
      if (!LettreMot(tar[i])){
        ind_curseur = 0;
        if (ind_mot >= taille_liste_mot){
          taille_liste_mot+=10;
          liste_mot = (byte**) realloc(liste_mot, (size_t) taille_liste_mot);
        }
      }else {
        liste_mot[ind_mot][ind_curseur++] = tar[i];
      }
    }
  }
  *nb = ++ind_mot;
  return liste_mot;
}

int C3(int lenkey, int lentar, byte* tar){
  int nb;
  int nb_mot_cle = 0;
  byte** liste_key = buildkey(lenkey, lentar, tar, &nb);
  if (liste_key == NULL) return 1;
  byte** dico = RemplirDico();
  byte** liste_mot;
  for (int i = 0; i < nb; ++i) {
    xorcipher(lenkey, liste_key[i], lentar, tar);
    liste_mot = ParsingTar(lentar, tar, &nb_mot_cle);
    xorcipher(lenkey, liste_key[i], lentar, tar);
  }
  libDoublePointeur(liste_key, nb);
  libDoublePointeur(dico, nbMot-1);
  return 0;
}