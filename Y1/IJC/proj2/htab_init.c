// htab_init.c
// Řešení IJC-DU2, příklad 2), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Funkcia na inicializaciu tabulky s implicitnou hashovacou funkciou

#include "htable.h"

htab_t *htab_init(unsigned long size){
	htab_t *new_table;
	if((new_table = malloc(size * sizeof(struct htable_listitem *) + sizeof(*new_table) )) == NULL){
		fprintf(stderr,"Nepodarilo sa alokovat pamat pre tabulku!\n");
		return NULL;
	}


	memset(new_table, 0, size * sizeof(struct htable_listitem *) + sizeof(*new_table) );	//nulovanie tabulky

	new_table->htab_size=size;
	new_table->n=0;
	new_table->hash_fun_ptr=&hash_function;
	
	return new_table;
}