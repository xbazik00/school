// htab_foreach.c
// Řešení IJC-DU2, příklad 2), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Funkcia na vyuzitie funkcie pre vsetky polozky tabulky

#include "htable.h"

void htab_foreach(htab_t *t,void (*func)(const char*, unsigned)){
	if(t == NULL){
		fprintf(stderr,"Tabulka neexistuje!\n");
		return;
	}
	if(t->items == NULL){
		fprintf(stderr,"Tabulka neobsahuje ziadne polozky!\n");
		return;
	}

	for (unsigned i = 0 ; i < t->htab_size; i++)		//prechod tabulkou
		for(struct htab_listitem *item = t->items[i] ;item != NULL; item = item->next)		//prechod polozkami
			func(item->key,item->data);
}