//
// Created by Quentin Nouvel on 28/11/2017.
//

#include "valid.h"

const byte keyCharlist[] = ",-.0123456789:?ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz{}";
const int lenkeyCharList = 70;

/************************************************************
 *                                                          *
 * FUNCTION NAME: valid_text_char                           *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ _____ ___ ______________________                *
 * text     byte  I   Caractère d'un message                *
 *                                                          *
 * RETURNS: Vérifie que le caractère est un caractère       *
 *          possible de message.                            *
 *                                                          *
 ***********************************************************/
bool valid_text_char(byte text){
	switch (text) {
		case 0 ... 9:
		case 11 ... 31:
		case 35 ... 38:
		case 42 ... 43:
		case 47 ... 57:
		case 60 ... 62:
		case 64:
		case 91 ... 94:
		case 96:
		case 124:
		case 126 ... 130:
		case 132 ... 191:
		case 193:
		case 195:
		case 197 ... 198:
		case 204 ... 205:
		case 208 ... 211:
		case 213:
		case 215 ... 216:
		case 218:
		case 220 ... 223:
		case 225:
		case 227:
		case 229 ... 231:
		case 236 ... 237:
		case 240 ... 243:
		case 245:
		case 247 ... 248:
		case 250:
		case 252 ... 255:
			return false;
		default:
			return true;
	}

	/*return ((text == 32) || (text >= 65 && text <= 90) || (text >= 97 && text <= 122)
  //Pontuations Only messages
  || (text == 33) || (text == 34) || (text == 39) || (text == 40) || (text == 41) || (text == 59)
  //Ponctuations clé & messages
  || (text == 44) || (text == 45) || (text == 46) || (text == 58) || (text == 63) || (text == 95) || (text == 123) || (text == 125)
  //ACCENTS
  // A
  || (text == 192) || (text == 194) || (text == 196) || (text == 224) || (text == 226) || (text == 228)
  // C
  || (text == 131) || (text == 199)
  // E
  || (text>=200 && text<=203) || (text>=232 && text<=235)
  // I
  || (text == 206) || (text == 207) || (text == 238) || (text == 239)
  // U
  || (text == 217) || (text == 219) || (text == 249) || (text == 251)
  // O
  || (text == 212) || (text == 214) || (text == 244) || (text == 246)
  // Carriage return
  || (text == 10));*/
}

/************************************************************
 *                                                          *
 * FUNCTION NAME: valid_key_char                            *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ _____ ___ ___________________                   *
 * key      byte  I   Caractère d'une clé                   *
 *                                                          *
 * RETURNS: Vérifie que le caractère est un caractère       *
 *          possible de clé.                                *
 *                                                          *
 ***********************************************************/
bool valid_key_char(byte key){
	switch (key) {
		case 126 ... 255:
		case 0 ... 43:
			return false;
		case 65 ... 90:
		case 97 ... 122:
		case 48 ... 57:
		case 44 ... 46:
		case 58:
		case 63:
		case 95:
		case 123:
		case 125:
			return true;
		default:
			return false;

	}
	/*return (key>=65 && key<=90) || (key>=97 && key<=122)//Majuscules, Minuscules
	|| (key == 44) || (key == 45) || (key == 46) || (key == 58) || (key == 63) || (key == 95) || (key == 123) || (key == 125)//Ponctuations clé & messages
	|| (key>47 && key<58);//Nombres
	 */
}

/************************************************************
 *                                                          *
 * FUNCTION NAME: is_key_valid                              *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ _____ ___ __________________                    *
 * key      byte* I   Clé                                   *
 * lenkey   int   I   Longueur de la clé                    *
 *                                                          *
 * RETURNS: Vérifie que la clé est valide                   *
 *                                                          *
 ***********************************************************/
bool is_key_valid(byte *key, int lenkey){
	int i = 0;
	bool res = true;
	while (i < lenkey && res) {
		res = valid_key_char(key[i++]);
	}
	return res;
}

/********************************************************************
 *                                                                  *
 * FUNCTION NAME: lib_double_pointeur                               *
 *                                                                  *
 * ARGUMENTS:                                                       *
 *                                                                  *
 * ARGUMENT     TYPE   I/O DESCRIPTION                              *
 * ____________ ______ ___ ________________________________________ *
 * pointeur     byte** I   Adresse de la liste de pointeur a libéré *
 * len_pointeur int    I   Nombre d'element de la liste             *
 *                                                                  *
 * RETURNS: Libère un double pointeur.                              *
 *                                                                  *
 *******************************************************************/
void lib_double_pointeur(byte **pointeur, int len_pointeur){
	for (int i = 0; i < len_pointeur; i++) {
		free(pointeur[i]);
	}
	free(pointeur);
}

/********************************************************************
 *                                                                  *
 * FUNCTION NAME: is_key_char_ok                                    *
 *                                                                  *
 * ARGUMENTS:                                                       *
 *                                                                  *
 * ARGUMENT TYPE  I/O DESCRIPTION                                   *
 * ________ _____ ___ __________________________________            *
 * tar      byte* I   Chaîne de caractère                           *
 * lentar   int   I   Longueur de la chaîne de caractère            *
 * keychar  byte  I   Caractère de clé que l'on vérifie             *
 * ind_key  int   I   Indice de keychar dans la clé                 *
 * lenkey   int   I   Longueur de la clé                            *
 *                                                                  *
 * RETURNS: Vérifie qu'un caractère a un indicé précis de la clé    *
 *          donne des caractère valide pour tous les caractère      *
 *          qu'il déchiffreras.                                     *
 *                                                                  *
 *******************************************************************/
bool is_key_char_ok(const byte *tar, int lentar, byte key_char, int ind_key, int lenkey){
	byte temp;
	for (int i = ind_key; i < lentar; i += lenkey) {
		temp = tar[i] ^ key_char;
		if (!valid_text_char(temp)) {
			return false;
		}
	}
	return true;
}

/************************************************************
 *                                                          *
 * FUNCTION NAME: keygen                                    *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ _____ ___ ____________                          *
 * lenkey   int   I   Longueur de clé recherché             *
 * lentar   int   I   Longueur du texte a déchiffrer        *
 * tar      byte* I   Texte qu'on cherche a déchiffrer      *
 *                                                          *
 * RETURNS: Génère pour chaque indice de clé la liste des   *
 *          caractères qui donnerons une valeurs de         *
 *          caractère de message valide après déchiffrage   *
 *                                                          *
 ***********************************************************/
byte **keygen(int lenkey, int lentar, byte *tar){
	byte **tab_key = (byte **) malloc(lenkey * sizeof(byte *));
	int nb_char;
	for (int i = 0; i < lenkey; i++) {
		nb_char = 0;
		tab_key[i] = (byte *) malloc((lenkeyCharList + 1) * sizeof(byte));
		for (int j = 0; j < lenkeyCharList; j++) {
			if (is_key_char_ok(tar, lentar, keyCharlist[j], i, lenkey)) {
				tab_key[i][nb_char] = keyCharlist[j];
				++nb_char;
			}
		}
		tab_key[i][nb_char] = (byte) '\0';
	}
	return tab_key;
}

/************************************************************
 *                                                          *
 * FUNCTION NAME: buildkey                                  *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ _____ ___ ____________                          *
 * lenkey   int   I   Longueur de clé recherché             *
 * lentar   int   I   Longueur du texte a déchiffrer        *
 * tar      byte* I   Texte qu'on cherche a déchiffrer      *
 * nb       int*    O Nombre de clé généré                  *
 *                                                          *
 * RETURNS: Génère toutes les clé possible avec les liste   *
 *          caractère généré par keygen.                    *
 *                                                          *
 ***********************************************************/
byte **buildkey(int lenkey, int lentar, byte *tar, int *nb){
	byte **tab_key = keygen(lenkey, lentar, tar);
	int nb_key = 1;
	int nb_char[lenkey];
	int diviseur[lenkey];
	for (int i = 0; i < lenkey; i++) {
		// Calcule le nombre de caractère dans chaque liste.
		nb_char[i] = 0;
		for (int j = 0; tab_key[i][j] != '\0'; j++) {
			++nb_char[i];
		}
		if (nb_char[i] == 0) return NULL;
		nb_key *= nb_char[i];
	}
	for (int i = 0; i < lenkey; i++) {
		// Calcule le nombre de fois que chaque indice va être répéter d'affilé.
		if (i == 0) {
			diviseur[0] = nb_key / nb_char[0];
		} else {
			diviseur[i] = diviseur[i - 1] / nb_char[i];
		}
	}
	*nb = nb_key;
	byte **key = (byte **) malloc(nb_key * sizeof(byte *));
	for (int i = 0; i < nb_key; i++) {
		key[i] = (byte *) malloc(lenkey * sizeof(byte));
		for (int j = 0; j < lenkey; j++) {
			key[i][j] = tab_key[j][(i / diviseur[j]) % nb_char[j]];
		}
	}
	lib_double_pointeur(tab_key, lenkey);
	return key;
}

byte **buildkey_opti(int lenkey, int lentar, byte *tar, int *nb){
	byte **tab_key = keygen(lenkey, lentar, tar);
	int nb_key = 1;
	int nb_char[lenkey];
	int diviseur[lenkey];
	for (int i = 0; i < lenkey; i++) {
		// Calcule le nombre de caractère dans chaque liste.
		nb_char[i] = 0;
		for (int j = 0; tab_key[i][j] != '\0'; j++) {
			++nb_char[i];
		}
		if (nb_char[i] == 0) return NULL;
		nb_key *= nb_char[i];
	}
	for (int i = 0; i < lenkey; i++) {
		// Calcule le nombre de fois que chaque indice va être répéter d'affilé.
		if (i == 0) {
			diviseur[0] = nb_key / nb_char[0];
		} else {
			diviseur[i] = diviseur[i - 1] / nb_char[i];
		}
	}
	*nb = nb_key;
	byte **key = (byte **) malloc(nb_key * sizeof(byte *));
	int indice_div = 0;
	int indice_key = 0;
	for (int i = 0; i < nb_key; ++i) {
		key[i] = (byte *) malloc(lenkey * sizeof(byte));
	}
	for (int i = 0; i < lenkey; i++) {
		indice_div = 0;
		indice_key = 0;
		for (int j = 0; j < nb_key; j++) {
			if (indice_key == diviseur[i]) {
				indice_key = 0;
				++indice_div;
				if (indice_div == nb_char[i]) {
					indice_div = 0;
				}
			}
			key[j][i] = tab_key[i][indice_div];
			++indice_key;
		}
	}
	lib_double_pointeur(tab_key, lenkey);
	return key;
}

/************************************************************
 *                                                          *
 * FUNCTION NAME: C1                                        *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ _____ ___ ____________                          *
 * lenkey   int   I   Longueur de clé recherché             *
 * lentar   int   I   Longueur du texte a déchiffrer        *
 * tar      byte* I   Texte qu'on cherche a déchiffrer      *
 *                                                          *
 * RETURNS: Printf les listes de caractères valide pour     *
 *          pour chaque caractère de la clé.                *
 *                                                          *
 ***********************************************************/
int C1(int lenkey, int lentar, byte *tar){
	byte **list_char = keygen(lenkey, lentar, tar);
	for (int i = 0; i < lenkey; ++i) {
		if (strlen((const char *) list_char[i]) == 0) {
			return 1;
		}
	}
	for (int i = 0; i < lenkey; ++i) {
		printf("[");
		for (int j = 0; list_char[i][j] != '\0'; ++j) {
			printf("%c", list_char[i][j]);
		}
		printf("]");
	}
	if (lenkey < 8) {
		printf("\n");
	}
	lib_double_pointeur(list_char, lenkey);
	return 0;
}
