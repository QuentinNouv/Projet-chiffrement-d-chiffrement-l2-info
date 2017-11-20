#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cipher.h"

byte keyCharlist[] = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz,-.:?_{}0123456789";
int lenkeyCharList = 70;
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

bool IsKeyCharOk(byte *tar, int lentar, byte keyChar, int indkey, int lenkey){
  byte temp;
  for (int i = indkey; i < lentar; i+=lenkey) {
    temp = tar[i] ^ keyChar;
    if (!ValidTextChar(temp)) {
      return false;
    }
  }
  return true;
}

byte** keygen(int lenkey,int lentar, byte *tar){
  byte** tab_key = (byte**) malloc(lenkey*sizeof(byte*));
  int nb_char;
  for (int i = 0; i < lenkey; i++) {
    nb_char = 0;
    tab_key[i] = (byte*) calloc(1,lenkeyCharList*sizeof(byte));
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
