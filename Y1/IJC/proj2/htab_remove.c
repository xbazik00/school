// htab_remove.c
// Řešení IJC-DU2, příklad 2), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Funkcia najde a odstrani polozku so zadanym klucom ak ju nenajde
//        vypise chybu.

#include "htable.h"

void htab_remove(htab_t *t,char *key){
	if(t == NULL){
		fprintf(stderr,"Tabulka neexistuje!\n");
		return;
	}
	
	if(t->items == NULL){
		fprintf(stderr,"Tabulka neobsahuje ziadne polozky!\n");
		return;
	}

	if(key == NULL){
		fprintf(stderr,"Chybny kluc!\n");
		return;
	}

	int err_flag=1;		//kontrolna premenna 

	unsigned hash_index = t->hash_fun_ptr(key,t->htab_size);
	struct htab_listitem *prev_item = NULL;

	//prechod zoznamom poloziek a odstranenie danej polozky
	for(struct htab_listitem *item = t->items[hash_index]; item != NULL; item=item->next){
		if(strcmp(key,item->key)==0){
			err_flag=0;
			if(prev_item != NULL){
				t->n -= 1;
				item->data -= 1;
				prev_item->next=item->next;
				free(item->key);
				item->key = NULL;
				free(item);
				return;
			}
			else{
				t->n -= 1;
				item->data -= 1;
				t->items[hash_index]=item->next;
				free(item->key);
				item->key = NULL;
				free(item);
				return;
			}
		}
		prev_item=item;
		
	}

	//vypis ak nebol najdeny kluc
	if (err_flag == 1)
		fprintf(stderr,"Kluc \"%s\" nebol najdeny!\n", key);
}