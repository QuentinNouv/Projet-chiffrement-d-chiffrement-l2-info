#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cipher.h"

const byte keyCharlist[] = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz,-.:?_{}0123456789";
const int lenkeyCharList = 70;
bool ValidTextChar(byte text){
  if (
  //Espace , Majuscules, Minuscules
  (text == 32) || (text>64 && text<91) || (text>89 && text<123)
  //Pontuations Only messages
  || (text == 33) || (text == 34)|| (text == 39) || (text == 40) || (text == 41) || (text == 59)
  //Ponctuations clé & messages
  || (text == 44) || (text == 45) || (text == 46) || (text == 58) || (text == 63) || (text == 95) || (text == 123) || (text == 125)
  //ACCENTS
  // A
  || (text == 192) || (text == 194) || (text == 196) || (text == 224) || (text == 226) || (text == 228)
  // E
  || (text>199 && text<204) ||(text>231 && text<236)
  // I
  || (text == 206) || (text == 207) || (text == 238) || (text == 239)
  // U
  || (text == 217) || (text == 219) || (text == 249) || (text == 251)
  // O
  || (text == 212) || (text == 214) || (text == 244) || (text == 246) ){
    return true;
  }
  return false;
}

bool ValidKeyChar(byte key){
  if (
  //Majuscules, Minuscules
  (key>64 && key<91) || (key>89 && key<123)
  //Ponctuations clé & messages
  || (key == 44) || (key == 45) || (key == 46) || (key == 58) || (key == 63) || (key == 95) || (key == 123) || (key == 125)
  //Nombres
  || (key>47 && key<58)){
    return true;
  }
  return false;
}

bool IsKeyCharOk(const byte *tar, int lentar, byte keyChar, int indkey, int lenkey){
  byte temp;
  for (int i = indkey; i < lentar; i+=lenkey) {
    temp = tar[i] ^ keyChar;
    if (!ValidTextChar(temp)) {
      return false;
    }
  }
  return true;
}

bool IskeyValid(byte* key, int lenkey){
  int i = 0;
  bool res = true;
  while (i< lenkey && res){
    res = ValidKeyChar(key[i++]);
  }
  return res;
}

byte** keygen(int lenkey,int lentar, byte *tar){
  byte** tab_key = (byte**) malloc(lenkey*sizeof(byte*));
  int nb_char;
  for (int i = 0; i < lenkey; i++) {
    nb_char = 0;
    tab_key[i] = (byte*) malloc((lenkeyCharList+1)*sizeof(byte));
    for (int j = 0; j < lenkeyCharList; j++) {
      if (IsKeyCharOk(tar, lentar, keyCharlist[j], i, lenkey)) {
        tab_key[i][nb_char] = keyCharlist[j];
        ++nb_char;
      }
    }
    tab_key[i][nb_char] = '\0';
  }
  return tab_key;
}

void libDoublePointeur(byte** pointeur, int lenPointeur) {
  for (int i = 0; i < lenPointeur; i++) {
    free(pointeur[i]);
  }
  free(pointeur);
}

byte** buildkey(int lenkey, int lentar, byte *tar,int* nb){
  byte** tab_key = keygen(lenkey, lentar, tar);;
  int nb_key = 1;
  int nb_char[lenkey];
  int diviseur[lenkey];
  for (int i = 0; i < lenkey; i++) {
    nb_char[i] = 0;
    for (int j = 0; tab_key[i][j]!='\0'; j++) {
      ++nb_char[i];
    }
    nb_key *= nb_char[i];
  }
  for (int i = 0; i < lenkey; i++) {
    if (i==0) {
      diviseur[0] = nb_key/nb_char[0];
    } else {
      diviseur[i] = diviseur[i-1]/nb_char[i];
    }
  }
  *nb = nb_key;
  byte** key = (byte**) malloc(nb_key*sizeof(byte*));
  for (int i = 0; i < nb_key; i++) {
    key[i] = (byte*) malloc(lenkey*sizeof(byte));
    for (int j = 0; j < lenkey; j++) {
      key[i][j] = tab_key[j][(i/diviseur[j])%nb_char[j]];
    }
  }
  libDoublePointeur(tab_key, lenkey);
  return key;
}
