// io.h
// Řešení IJC-DU2, příklad 2), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Funkcia nacitava text.

#ifndef IO_H_INCLUDED
#define	IO_H_INCLUDED


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/*
*	Nacitava retazce zo suboru, ktore vracia cez ukazovatel.
*	Vstup:	ukazatel na retazec
*			maximalny pocet znakov
*			subor 
*	Vystup: dlzka slova
*/
int get_word(char *s, int max, FILE *f);

#endif