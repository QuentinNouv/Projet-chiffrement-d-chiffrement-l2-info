//
// Created by Quentin Nouvel on 04/12/2017.
//

#include "freq.h"

const float freq_th[Nb_L_Alphabet] = {9.42, 1.02, 2.64, 3.39, 15.87, 0.95, 1.04, 0.77, 8.41, 0.89, 0.00, 5.34, 3.24,
									  7.15, 5.14, 2.86, 1.06, 6.46, 7.90, 7.26, 6.24, 2.15, 0.00, 0.30, 0.24, 0.32};

/********************************************************************
 *                                                                  *
 * FUNCTION NAME: lib_double_pointeur_float                         *
 *                                                                  *
 * ARGUMENTS:                                                       *
 *                                                                  *
 * ARGUMENT TYPE    I/O DESCRIPTION                                 *
 * ___________ _______ ___ ______________________________________   *
 * pointeur    float** I   adresse de la liste de pointeur à free   *
 * lenpointeur int     I   nombre d'element dans la liste           *
 *                                                                  *
 * RETURNS: Libère un double pointeur.                              *
 *                                                                  *
 *******************************************************************/
void lib_double_pointeur_float(float **pointeur, int len_pointeur){
	for (int i = 0; i < len_pointeur; i++) {
		free(pointeur[i]);
	}
	free(pointeur);
}

/****************************************************************
 *                                                              *
 * FUNCTION NAME: index_lettre                                  *
 *                                                              *
 * ARGUMENTS:                                                   *
 *                                                              *
 * ARGUMENT TYPE  I/O DESCRIPTION                               *
 * ________ _____ ___ ___________                               *
 * carac    byte  I   Caractère                                 *
 *                                                              *
 * RETURNS: L'index du caractère dans le tableau de fréquence.  *
 *                                                              *
 ***************************************************************/
int index_lettre(byte carac){
	switch (carac) {
		case 65 ... 90:
			return carac - 65;
		case 97 ... 122:
			return carac - 97;
		case 192 ... 196:
		case 224 ... 228:
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

/************************************************************
 *                                                          *
 * FUNCTION NAME: calc_freq                                 *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ _____ ___ ____________                          *
 * tar      byte* I   Texte déchiffré                       *
 * lentar   int   I   Longueur du texte                     *
 *                                                          *
 * RETURNS: Tableau de fréquence d'apparition de chaque     *
 *          lettres de l'alphabet.                          *
 *                                                          *
 ***********************************************************/
float *calc_freq(byte *tar, int lentar){
	int nb_lettre = 0;
	float *freq = calloc(Nb_L_Alphabet + 1, sizeof(float));
	for (int i = 0; i < lentar; ++i) {
		freq[index_lettre(tar[i])] += 1;
		++nb_lettre;
	}
	for (int j = 0; j < Nb_L_Alphabet; ++j) {
		freq[j] /= lentar - freq[27];
		freq[j] *= 100;
	}
	return freq;
}

/************************************************************************************
 *                                                                                  *
 * FUNCTION NAME: calcul_prox                                                       *
 *                                                                                  *
 * ARGUMENTS:                                                                       *
 *                                                                                  *
 * ARGUMENT TYPE   I/O DESCRIPTION                                                  *
 * ________ ______ ___ ____________                                                 *
 * freq     float* I   Tableau de fréquence d'occurences des lettres de l'alphabet  *
 *                                                                                  *
 * RETURNS: Score de proximité de freq avec le tableau de fréquences                *
 *          d'occurence des lettre de l'alphabet de la langue française,            *
 *          Plus le score est bas meilleurs est la proximité.                       *
 *                                                                                  *
 ***********************************************************************************/
float calcul_prox(float *freq){
	float prox = 0;
	for (int i = 0; i < Nb_L_Alphabet; ++i) {
		prox += powf(freq_th[i] - freq[i], 2);
	}
	return prox;
}

/************************************************************
 *                                                          *
 * FUNCTION NAME: C3                                        *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ _____ ___ ____________                          *
 * lenkey   int   I   Longueur de la clé que l'on recherche *
 * lentar   int   I   Longueur du texte a déchiffrer        *
 * tar      byte* I   Texte chiffré à déchiffrer            *
 *                                                          *
 * RETURNS: exit code 1 : ko                                *
 *               code 0 : ok                                *
 *          Prinf également la meilleur clé candidate de    *
 *          longueur lenkey pour le texte tar.              *
 *                                                          *
 ***********************************************************/
int C2(int lenkey, int lentar, byte *tar){
	int nb;
	float **tab_freq;
	float current_best_prox = 0;
	float temp_prox;
	byte *current_text;
	byte *current_best_key = NULL;
	byte **liste_key = buildkey(lenkey, lentar, tar, &nb);
	if (liste_key == NULL) return 0;
	tab_freq = (float **) malloc(nb * sizeof(float *));
	for (int i = 0; i < nb; ++i) {
		// Parcours toute les clé,
		current_text = xorciphercopy(lenkey, liste_key[i], lentar, tar);// Déchiffre le texte,
		tab_freq[i] = calc_freq(current_text,
								lentar);// Calcule le tableau de fréquence d'occurence des lettres du texte,
		if (i == 0) {
			current_best_key = liste_key[i];
			current_best_prox = calcul_prox(tab_freq[i]);
		} else {
			temp_prox = calcul_prox(tab_freq[i]);
			if (temp_prox < current_best_prox) {
				current_best_key = liste_key[i];
				current_best_prox = temp_prox;
			}
		}
		free(current_text);
	}
	if (current_best_key != NULL) {
		for (int j = 0; j < lenkey; ++j) {
			printf("%c", current_best_key[j]);
		}
	}
	printf("\n");
	lib_double_pointeur_float(tab_freq, nb);
	lib_double_pointeur(liste_key, nb);
	return 0;
}
