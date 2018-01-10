#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "cipher.h"
#include "valid.h"
#include "copy.h"
#include "freq.h"
#include "dico.h"

/********************************************************************
 *                                                                  *
 * FUNCTION NAME: verif_options                                     *
 *                                                                  *
 * ARGUMENTS:                                                       *
 *                                                                  *
 * ARGUMENT TYPE  I/O DESCRIPTION                                   *
 * ________ _____ ___ __________________________________________    *
 * i        char* I   Nom du fichier d'entrée                       *
 * o        char* I   Nom de fichier de sortie                      *
 * k        byte* I   Clé si c'est en mode chiffrage/déchiffrage    *
 * m        char* I   Mode (1,2,3,31)                               *
 * i_file   FILE* I   Fichier d'entrée                              *
 * o_file   FILE*   O Fichier de sortie                             *
 *                                                                  *
 * RETURNS: exit code 0 ok                                          *
 *                    1 ko                                          *
 *          Vérifie que les options sont cohérentes et lances les   *
 *          instructions en fonction.                               *
 *                                                                  *
 *******************************************************************/
int verif_options(char *i, char *o, byte *k, long l, const char *m, FILE *i_file, FILE *o_file){
	// Return -1 si pas bon
	if (i != NULL) {
		//Ouvre le fichier si ne peut pas ferme le programme
		i_file = fopen(i, "r");
		if (i_file == NULL) {
			fprintf(stderr, "ERROR : Le fichier %s n'est pas lisible ou n'existe pas.\n", i);
			return -1;
		}
		fclose(i_file);
		if ((k != NULL && o != NULL && l == -1 && m == NULL)) {
			if (!is_key_valid(k, (int) strlen((char *) k))) {
				fprintf(stderr, "ERROR : La clé %s n'est pas valide.\n", k);
				return -1;
			}
			o_file = fopen(o, "w");
			if (o_file == NULL) {
				fprintf(stderr, "ERROR : Le fichier %s ne peut être édité.\n", o);
				return -1;
			}
			fclose(o_file);
			return 1;
		} else if ((k == NULL && o == NULL && m != NULL)) {
			/* Code 11   : C1   + l, 10   : C1   sans l
			 *      21   : C2   + L, 20   : C2   sans l
			 *      31   : C3.1
			 *      32   : C3.2,
			 */
			int choix = 0;
			if (!(strcmp(m, "1"))) choix = 10;
			else if (!(strcmp(m, "2"))) choix = 20;
			else if (!(strcmp(m, "3.1"))) choix = 31;
			else if (!(strcmp(m, "3"))) choix = 32;
			else {
				fprintf(stderr, "ERROR : Le critère de cassage du  chiffrage est incorrect.\n");
				return -1;
			}
			if (choix == 31) {
				if (l < 1) {
					fprintf(stderr, "ERROR : Vous devez spécifiez une taille de clé valide pour l'option -m 31.\n");
					return -1;
				}
			} else if (choix == 32) {
				if (l < 1) {
					fprintf(stderr,
							"ERROR : Si vous ne spécifiez pas de valeurs pour -m 32 la valeurs 7 est prise par reférence.\n");
				}
				return choix;
			} else if (l > 0) {
				++choix;
				return choix;
			}
			return choix;
		}
		fprintf(stderr, "ERROR : Une erreur inattendu est intervenue.\n");
		return -1;
	}
	fprintf(stderr, "ERROR : Le paramètre -i est obligatoire et a besoin d'un argument .\n");
	return -1;
}

/********************************************************************************
 *                                                                              *
 * FUNCTION NAME: lancement_option                                              *
 *                                                                              *
 * ARGUMENTS:                                                                   *
 *                                                                              *
 * ARGUMENT TYPE  I/O DESCRIPTION                                               *
 * ________ _____ ___ __________________________________________________________*
 * mode     int   I   Code du traitement a effectuer                            *
 * i        char* I   Nom du fichier d'entrée                                   *
 * o        char* I   Nom du fichier de sortie                                  *
 * k        byte* I   Clé si c'est en mode chiffrage/déchiffrage                *
 * l        int   I   Longueur de la clé recherché si c'est une recherche de clé*
 *                                                                              *
 * RETURNS: exit code 0 ok                                                      *
 *                    1 ko                                                      *
 *          Lance également les instructions correspondant à mode.              *
 *                                                                              *
 *******************************************************************************/
int lancement_option(int mode, char *i, char *o, byte *k, int l){
	// mode -1 erreur, 1 chiffrage
	/* Code 11   : C1   + l, 10   : C1   sans l
	 *      21   : C2   + L, 20   : C2   sans l
	 *      31   : C3.1,
	 *      32   : C3.2,
	 */
	int return_code = 0;
	int len, lentar;
	FILE *file_i;
	byte *tar;
	switch (mode) {
		case 1:
			xorcipher2(i, &len, o, (int) strlen((char *) k), k);
			return return_code;
		case 10:
			file_i = fopen(i, "r");
			tar = copyfile(file_i, &lentar);
			for (int lenkey = 3; lenkey < 8; ++lenkey) {
				C1(lenkey, lentar, tar);
			}
			return 0;
		case 11:
			file_i = fopen(i, "r");
			tar = copyfile(file_i, &lentar);
			return_code += C1(l, lentar, tar);
			if (return_code)return 255;
			else return 0;
		case 20:
			file_i = fopen(i, "r");
			tar = copyfile(file_i, &lentar);
			for (int lenkey = 3; lenkey < 8; ++lenkey) {
				return_code += C2(lenkey, lentar, tar);
			}
			return 0;
		case 21:
			file_i = fopen(i, "r");
			tar = copyfile(file_i, &lentar);
			return_code += C2(l, lentar, tar);
			if (return_code)return 255;
			else return 0;
		case 31:
			file_i = fopen(i, "r");
			tar = copyfile(file_i, &lentar);
			return_code += C3(l, lentar, tar);
			if (return_code)return 255;
			else return 0;
		case 32:
			file_i = fopen(i, "r");
			tar = copyfile(file_i, &lentar);
			if (l < 1) {
				l = 8;
			}
			for (int lenkey = 3; lenkey < l; ++lenkey) {
				return_code += C3(lenkey, lentar, tar);
			}
			return return_code;
		case -1:
			fprintf(stderr, "Fin du programme.\n");
			return -1;
		default:
			fprintf(stderr, "ERROR : INCONNU\n");
			return -1;
	}
}

/************************************************************
 *                                                          *
 * FUNCTION NAME: option                                    *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ ______ ___ _____________________                *
 * argc     int    I   Nombre d'argument                    *
 * argv     char** I   Liste de argv élément                *
 *                                                          *
 * RETURNS: Analyse les options via getopt puis lance les   *
 *          instructions.                                   *
 *                                                          *
 ***********************************************************/
int option(int argc, char **argv){
	int c;
	char *i = NULL;
	FILE *i_file = NULL;
	char *o = NULL;
	FILE *o_file = NULL;
	byte *k = NULL;
	long l = -1;
	char *m = NULL;
	while ((c = getopt(argc, argv, "i:o:k:l:m:")) != -1) {
		switch (c) {
			case 'i':
				i = optarg;
				break;
			case 'o':
				o = optarg;
				break;
			case 'k':
				k = (byte *) optarg;
				break;
			case 'l':
				l = strtol(optarg, NULL, 10);
				break;
			case 'm':
				m = optarg;
				break;
			case '?':
				if (optopt == 'c') {
					fprintf(stderr, "L'option -%c à besoin d'un argument.\n", optopt);
					exit(-1);
				} else if (isprint(optopt)) {
					fprintf(stderr, "Option inconnue `-%c'.\n", optopt);
					exit(-1);
				} else {
					fprintf(stderr, "Caractère inconnue`\\x%x'.\n", optopt);
					exit(-1);
				}
			default:
				exit(-1);
		}
	}
	int Instruction = verif_options(i, o, k, (int) l, m, i_file, o_file);
	return lancement_option(Instruction, i, o, k, (int) l);
}


int main(int argc, char **argv){
	int retour;
	clock_t start, end;
	start = clock();
	retour = option(argc, argv);
	end = clock();
	//fprintf(stderr, " Effectué en %f secondes.\n", (double)(end-start)/CLOCKS_PER_SEC);
	return retour;
}
