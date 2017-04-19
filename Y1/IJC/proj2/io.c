// io.c
// Řešení IJC-DU2, příklad 2), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Funkcia nacitava text.

#include "io.h"



int get_word(char *s, int max, FILE *f){
    if (f == NULL){
    	fprintf(stderr,"Neplatny subor!\n");
        return EOF;
    }

    int word_length=0;		//dlzka slova
    int count;
    int c = fgetc(f);

    while (isspace(c))		//preskoci prazdne znaky na zaciatku 
    	c = fgetc(f);
    if (c == EOF)
        return EOF;

	for (count = 0; isspace(c)==0; count++){
		
		if (count == 0 && c == EOF)
			return EOF;

		if (count != 0 && c == EOF){
			s[count]='\0';
			return EOF;
		}
		if (count >= max - 1){
			while (isspace(c)==0){
				word_length++;
				c=fgetc(f);
			}
			break;
		}
		
		s[count]=c;
		c = fgetc(f);
		word_length++;
	}
	
	s[count]='\0';		//nula na koniec retazca
	return word_length;
}
