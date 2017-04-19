// hash_function.c 
// Řešení IJC-DU2, příklad 2), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Hashovacia funkcia, zo zadania

#include "hash_function.h"

unsigned int hash_function(const char *str, unsigned htab_size) { 
	unsigned int h=0; 
	const unsigned char *p; 
	for(p=(const unsigned char*)str; *p!='\0'; p++) 
		h = 65599*h + *p; 
	return h % htab_size; 
}