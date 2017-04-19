// htab_free.c
// Řešení IJC-DU2, příklad 2), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Funkcia na uvolnenie tabulky

#include "htable.h"

void htab_free(htab_t *t){
	if(t == NULL){
		fprintf(stderr,"Tabulka neexistuje!\n");
		return;
	}
	
	if(t->items == NULL){
		fprintf(stderr,"Tabulka neobsahuje ziadne polozky!\n");
		return;
	}
	htab_clear(t);
	free(t);
	t=NULL;	
} 