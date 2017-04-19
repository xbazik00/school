// htab_clear.c
// Řešení IJC-DU2, příklad 2), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Funkcia na vycistenie tabulky

#include "htable.h"

void htab_clear(htab_t *t){
	if(t == NULL){
		fprintf(stderr,"Tabulka neexistuje!\n");
		return;
	}
	
	if(t->items == NULL){
		fprintf(stderr,"Tabulka neobsahuje ziadne polozky!\n");
		return;
	}

	struct htab_listitem *next_item = NULL;

	for(int i = 0; i < t->htab_size; i++){		//prechadza vsetky polozky
		for(struct htab_listitem *item = t->items[i] ;item != NULL ;item=NULL, item = next_item){	//prechadza cely zoznam
			free(item->key);
			item->key = NULL;
			next_item = item->next;
			free(item);
		}
	}
	t->n=0;

}