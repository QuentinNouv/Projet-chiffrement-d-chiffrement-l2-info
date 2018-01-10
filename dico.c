//
// Created by Quentin Nouvel on 06/12/2017.
//

#include "dico.h"

//const byte DICO[] = "dictionnaire.txt";
//const int nbMot = 336531;
//const int tailleMaxMot = 30;

/************************************************************
 *                                                          *
 * FUNCTION NAME: remplir_dico                              *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ _____ ___ ____________                          *
 *                                                          *
 * RETURNS: liste de mot du dictionnaire.                   *
 *                                                          *
 ***********************************************************/
/*
byte** remplir_dico(){
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

/****************************************************************
 *                                                              *
 * FUNCTION NAME: hash                                          *
 *                                                              *
 * ARGUMENTS:                                                   *
 *                                                              *
 * ARGUMENT TYPE  I/O DESCRIPTION                               *
 * ________ _____ ___ ____________                              *
 * c        byte  I   Lettre d'un mot a hasher                  *
 * i        int   I   Indice de la lettre a l'intérieur du mot  *
 *                                                              *
 * RETURNS: La valeurs de la lettre pour le hashage.            *
 *                                                              *
 ***************************************************************/
unsigned long int hash(byte c, int i){
	return (unsigned long int) ((int) c * (pow(256, i)));
	/*
	 * Fontion hashage de caractère, c * (indice^256)
	 */
}

/************************************************************************
 *                                                                      *
 * FUNCTION NAME: hash_mot                                              *
 *                                                                      *
 * ARGUMENTS:                                                           *
 *                                                                      *
 * ARGUMENT TYPE  I/O DESCRIPTION                                       *
 * ________ _____ ___ ____________                                      *
 * word     byte* I   Mot à hasher                                      *
 *                                                                      *
 * RETURNS: Somme des valeurs de chaque lettres du mot après le hashage.*
 *                                                                      *
 ************************************************************************/
unsigned long int hash_mot(byte *word){
	unsigned long int word_value = 0;
	for (int i = 0; word[i] != '\0'; ++i) {
		word_value += hash(word[i], i);
	}
	return word_value;
}

/************************************************************
 *                                                          *
 * FUNCTION NAME: hash_dico                                 *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ _____ ___ ____________                          *
 *                                                          *
 * RETURNS: Table de hashage.                               *
 *                                                          *
 ***********************************************************/
bool *hash_dico(){
	// Dictionnaire pré-hashé et stocké dans dico.txt
	unsigned long i;
	FILE *dico = fopen("dico.txt",
					   "r");// Fichier dans le répertoire depuis lequel on éxécute le programme, dictionnaire préalablement hashé
	bool *hashed_dico = (bool *) calloc(4294967296, sizeof(bool));
	// 4294967296 = 256^4 nombres de combinaisons possible de caractère latin-1. Et Calloc pour ne pas a avoir a initialiser manuellement toute les valeurs a False.
	while (fscanf(dico, "%lu", &i) != EOF) { // NOLINT
		hashed_dico[i] = true;
	}
	return hashed_dico;
}

/********************************************************************************
 *                                                                              *
 * FUNCTION NAME: verif_dico                                                    *
 *                                                                              *
 * ARGUMENTS:                                                                   *
 *                                                                              *
 * ARGUMENT     TYPE              I/O DESCRIPTION                               *
 * ___________  _________________ ___ ________________________________________  *
 * i            unsigned long int I   Indice de la table de hashage a vérifier  *
 * hashed_dico  bool*             I   Table de hashage                          *
 *                                                                              *
 * RETURNS: Présence du mot dans le dictionnaire.                               *
 *                                                                              *
 ********************************************************************************/
bool verif_dico(unsigned long int i, bool *hashed_dico){
	return hashed_dico[i];
}

/************************************************************
 *                                                          *
 * FUNCTION NAME: isseparator                               *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ _____ ___ ____________________                  *
 * c        char  I   Caractère d'un texte                  *
 *                                                          *
 * RETURNS: True si le caractère est un séparateur.         *
 *                                                          *
 ***********************************************************/
bool is_separator(const char c){
	switch (c) {
		case 32:
		case 33:
		case 44:
		case 46:
		case 63:
		case 10:
			return true;
		default:
			return false;
	}
	//return (c == 32 || c == 33 || c == 44 || c == 46 || c == 63 || c == 10);
}

/************************************************************
 *                                                          *
 * FUNCTION NAME: parsing_prof                              *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ _____ ___ _____________________________         *
 * lentar   int   I   Longueur de la chaîne à parse         *
 * tar      byte* I   Chaîne à parse                        *
 * nb       int*    O Nombre de mots 2 à 4 lettres          *
 *                                                          *
 * RETURNS: Analyse la chaîne de caractère pour trouver les *
 *          mots qui se trouvent a l'intérieur.             *
 *                                                          *
 ***********************************************************/
/*//Ancienne fonction de Parse pour C3
byte **parsing_prof(int lentar, byte *tar, int *nb){
	int taille_liste_mot = 20;
	int ind_mot = 0;
	int ind_curseur = 0;
	int taille_mot = 30;
	int taille_current_mot = 0;
	byte *mot = NULL;
	byte **liste_mot = (byte **) malloc(taille_liste_mot * sizeof(byte *));
	for (int i = 0; i < lentar; ++i) {
		if (ind_curseur == 0) {
			if (!is_separator(tar[i])) {
				mot = (byte *) malloc(taille_mot * sizeof(byte));
				mot[ind_curseur++] = (byte) tolower(tar[i]);
			}
		} else {
			if (ind_curseur == taille_mot) {
				taille_mot += 10;
				mot = (byte *) realloc(mot, taille_mot * sizeof(byte));

			}
			if (is_separator(tar[i])) {
				taille_current_mot = (int) strlen((const char *) mot);
				liste_mot[ind_mot++] = mot;
				ind_curseur = 0;
				taille_mot = 30;
				if (taille_current_mot > 4 || taille_current_mot < 2) {
					// Si le mot n'est pas des tailles qui nous intéresse, le supprime et décrémente le nombre de mot actuel
					liste_mot[--ind_mot] = NULL;
				}
			} else {
				mot[ind_curseur++] = (byte) tar[i];
			}
		}
		if (ind_mot + 2 >= taille_liste_mot) {
			taille_liste_mot += 10;
			liste_mot = (byte **) realloc(liste_mot, taille_liste_mot * sizeof(byte *));
		}
	}
	*nb = ind_mot;
	return liste_mot;
}
*/

/************************************************************
 *                                                          *
 * FUNCTION NAME: parsing_et_score                          *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ___________ _____ ___ _____________________________      *
 * lentar      int   I   Longueur de la chaîne à parse      *
 * tar         byte* I   Chaîne à parse                     *
 * hashed_dico bool* I	 Dictionnaire hashé					*
 *                                                          *
 * RETURNS: Analyse la chaîne de caractère pour trouver les *
 *          mots qui se trouvent a l'intérieur calcule au	*
 *          passage le nombres de mots valide				*
 *                                                          *
 ***********************************************************/
int parse_et_score(int lentar, byte *tar, bool *hashed_dico){
	int score = 0;
	int taille_mot = 0;
	const int taille_max_mot = 4;
	byte mot[taille_max_mot + 1];
	for (int i = 0; i < lentar; ++i) {
		if (is_separator(tar[i])) {
			switch (taille_mot) {
				case 2 ... taille_max_mot:
					mot[taille_mot] = '\0';
					mot[0] = (byte) tolower(mot[0]);
					score += verif_dico(hash_mot(mot), hashed_dico);
				default:
					taille_mot = 0;
			}
		} else {
			if (taille_mot < taille_max_mot) {
				mot[taille_mot] = tar[i];
			}
			++taille_mot;
		}
	}
	return score;
}

/************************************************************
 *                                                          *
 * FUNCTION NAME: text_score                                *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT  TYPE   I/O DESCRIPTION                         *
 * _________ ______ ___ ____________________________        *
 * liste_mot byte** I   Liste des mots d'un texte*          *
 * nb_mot    int    I   Nombre de mot dans la liste*        *
 * hash_dico bool*  I   Table de hashage                    *
 *                                                          *
 * RETURNS: Nombre de mots de la liste trouver dans le      *
 *          dictionnaire.                                   *
 *                                                          *
 ***********************************************************/
/*// Fonction utilisé dans la version précédente de C3
int text_score(byte **liste_mot, int nb_mot, bool *hash_dico){
	int score = 0;
	for (int i = 0; i < nb_mot; ++i) {
		if (verif_dico(hash_mot(liste_mot[i]), hash_dico)) {
			score++;
		}
	}
	return score;
}
*/

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
/*
int C3_ancienne_version(int lenkey, int lentar, byte *tar){
	int nb;
	int nb_mot_cle = 0;
	int score = 0;
	int current_best_score = -1;
	byte *current_text;
	byte *current_best_key = NULL;
	byte **liste_key = buildkey_opti(lenkey, lentar, tar, &nb);
	if (liste_key == NULL) return 0;
	bool *hashed_dico = hash_dico();
	byte **liste_mot;
	for (int i = 0; i < nb; ++i) {
		// Parcours toute les clé,
		current_text = xorciphercopy(lenkey, liste_key[i], lentar, tar);// Déchiffre le texte,
		liste_mot = parsing_prof(lentar, current_text, &nb_mot_cle);// Cherche les mots dans le texte déchiffré,
		if (nb_mot_cle > current_best_score) {
			score = text_score(liste_mot, nb_mot_cle, hashed_dico);// Calcule le score du texte déchiffré.
			if (score > current_best_score) {
				current_best_score = score;
				current_best_key = liste_key[i];
			}
		}
		free(current_text);
		lib_double_pointeur(liste_mot, nb_mot_cle);
	}
	if (current_best_key != NULL) {
		for (int j = 0; j < lenkey; ++j) {
			printf("%c", current_best_key[j]);
		}
	}
	printf("\n");
	lib_double_pointeur(liste_key, nb);
	free(hashed_dico);
	return 0;
}
 */

int C3(int lenkey, int lentar, byte *tar){
	int nb;
	int score = 0;
	int current_best_score = -1;
	byte *current_text;
	byte *current_best_key = NULL;
	byte **liste_key = buildkey_opti(lenkey, lentar, tar, &nb);
	if (liste_key == NULL) return 0;
	bool *hashed_dico = hash_dico();
	for (int i = 0; i < nb; ++i) {
		// Parcours toute les clé,
		current_text = xorciphercopy(lenkey, liste_key[i], lentar, tar);// Déchiffre le texte,
		score = parse_et_score(lentar, current_text, hashed_dico);// Cherche les mots dans le texte déchiffré,
		//fprintf(stderr, "%d %s\n", score, liste_key[i]);
		if (score > current_best_score) {
			current_best_score = score;
			current_best_key = liste_key[i];
		}
		free(current_text);
	}
	if (current_best_key != NULL) {
		for (int j = 0; j < lenkey; ++j) {
			printf("%c", current_best_key[j]);
		}
	}
	printf("\n");
	lib_double_pointeur(liste_key, nb);
	free(hashed_dico);
	return 0;
}