// htab_lookup_add.c
// Řešení IJC-DU2, příklad 2), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Funkcia vyhlada polozku na zaklade kluca a ak dany kluc nenajde
//        vytvori danu polozku

#include "htable.h"

struct htab_listitem * htab_lookup_add(htab_t *t,char *key){
	if(t == NULL){
		fprintf(stderr,"Tabulka neexistuje!\n");
		return NULL;
	}
	
	if(t->items == NULL){
		fprintf(stderr,"Tabulka neobsahuje ziadne polozky!\n");
		return NULL;
	}

	if(key == NULL){
		fprintf(stderr,"Chybny kluc!\n");
		return NULL;
	}
		
	unsigned hash_index = t->hash_fun_ptr(key,t->htab_size);
	struct htab_listitem *prev_item = NULL;
	struct htab_listitem *first_item = t->items[hash_index];
	struct htab_listitem *new_item;

	
	//prechod zoznamom poloziek daneho hashovacieho indexu
	for(struct htab_listitem *item = first_item; item != NULL; prev_item = item, item = item->next){
		if(strcmp(key,item->key) == 0){		//ak najde zhodu
			item->data += 1;
			return item;
		}	
	}

	//alokacia novej polozky
	if ((new_item = malloc(sizeof(struct htab_listitem)))==NULL){
		fprintf(stderr,"Nepodarilo sa alokovat pamat pre novu polozku!\n");
		return NULL;
	}

	//alokacia pre velkost kluca + 1 byte pre nulu na konci retazca		
	if ((new_item->key = malloc((strlen(key)+1)*sizeof(char))) == NULL){
		fprintf(stderr,"Nepodarilo sa alokovat pamat pre kluc!\n");
		free(new_item);
		return NULL;
	}

	new_item->data = 1;
	new_item->next = NULL;
	strcpy(new_item->key,key);
	t->n++;

	//ak ide o prvy, polozka sa nacita na zaciatok
	if(prev_item == NULL)
		t->items[hash_index] = new_item;
		

	//inak ju nacita za poslednu polozku
	else
		prev_item->next = new_item;
		

	return new_item;
}
