#include "cipher.h"


/**********************************************************
 *                                                        *
 * FUNCTION NAME: xorcipher                               *
 *                                                        *
 * ARGUMENTS:                                             *
 *                                                        *
 * ARGUMENT TYPE  I/O DESCRIPTION                         *
 * ________ _____ ___ _____________________________       *
 * lenkey   int   I   Longueur de la clé                  *
 * key      byte* I   Clé                                 *
 * lentar   int   I   Longueur de la texte d'entrée       *
 * tar      byte* I/O Texte a tranformé                   *
 *                                                        *
 * RETURNS: Rien                                          *
 *                                                        *
 *********************************************************/
/*void xorcipher(int lenkey, const byte* key, int lentar, byte* tar){
  for (int i = 0; i < lentar; ++i) {
    tar[i] ^= key[i%lenkey];
  }
}*/

/**********************************************************
 *                                                        *
 * FUNCTION NAME: xorciphercopy                           *
 *                                                        *
 * ARGUMENTS:                                             *
 *                                                        *
 * ARGUMENT TYPE  I/O DESCRIPTION                         *
   * ________ _____ ___ ____________________________      *
 * lenkey   int   I   Longueur de la clé                  *
 * key      byte* I   Clé                                 *
 * lentar   int   I   Longueur de la texte d'entrée       *
 * tar      byte* I/O Texte a tranformé                   *
 *                                                        *
 * RETURNS: byte* de tar chiffré/déchiffré.               *
 *                                                        *
 *********************************************************/
byte *xorciphercopy(int lenkey, const byte *key, int lentar, const byte *tar){
	byte *res = (byte *) malloc(lentar * sizeof(byte));
	for (int i = 0; i < lentar; ++i) {
		res[i] = tar[i] ^ key[i % lenkey];
	}
	return res;
}


/**********************************************************
 *                                                        *
 * FUNCTION NAME: xorcipher2                              *
 *                                                        *
 * ARGUMENTS:                                             *
 *                                                        *
 * ARGUMENT TYPE  I/O DESCRIPTION                         *
 * ________ _____ ___ ________________________________    *
 * file_i   FILE* I   Fichier d'entrée                    *
 * len      int   I   Longueur de la clé                  *
 * file_i   FILE*   O Fichier de sortie                   *
 * lenkey   int   I   Longueur de la texte d'entrée       *
 * key      byte* I   Clé                                 *
 *                                                        *
 * RETURNS: exit code 1 : ko                              *
 *               code 0 : ok,                             *
 *          envoie dans file_o le fichier file_i          *
 *          chiffré/déchiffré.                            *
 *                                                        *
 *********************************************************/
int xorcipher2(char *file_i, int *len, char *file_o, int lenkey, const byte *key){
	FILE *file = fopen(file_i, "r");
	FILE *sortie = fopen(file_o, "w");
	*len = 0;
	if (file == NULL) {
		fprintf(stderr, "ERROR : Fichier %s illisible.", file_i);
		return 1;
	}
	if (sortie == NULL) {//Inutile avec "w" :')
		fprintf(stderr, "ERROR : Fichier %s non éditable.", file_o);
		return 1;
	}
	char c;
	while ((fscanf(file, "%c", &c)) != EOF) {
		c ^= key[(*len) % lenkey];
		fputc(c, sortie);
		++(*len);
	}
	return 0;
}