//
// Created by Quentin Nouvel on 29/11/2017.
//

#include "copy.h"

/************************************************************
 *                                                          *
 * FUNCTION NAME: copyfile                                  *
 *                                                          *
 * ARGUMENTS:                                               *
 *                                                          *
 * ARGUMENT TYPE  I/O DESCRIPTION                           *
 * ________ _____ ___ ______________________________________*
 * file     FILE* I   Fichier d'entrée préalablement ouvert *
 * lentar   int*    O Nombre d'octets de données            *
 *                                                          *
 * RETURNS: byte* du contenue du fichier                    *
 *                                                          *
 ***********************************************************/
byte *copyfile(FILE *file, int *lentar){
	int taille = 500;
	int i = 0;
	byte c;
	byte *copy = (byte *) malloc(taille * sizeof(byte));
	while (fscanf(file, "%c", &c) != EOF) {
		if (i > taille - 3) {
			taille += 100;
			copy = (byte *) realloc(copy, taille * sizeof(byte));
		}
		copy[i++] = c;
	}
	copy[i] = (byte) "\0";
	*lentar = i;
	return copy;
}