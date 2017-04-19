// hash_function.h 
// Řešení IJC-DU2, příklad 2), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Hashovacia funkcia, zo zadania

#ifndef HASH_FUN_H_INCLUDED
#define HASH_FUN_H_INCLUDED

/*
*	Hashuje retazce
*	Vstup:	ukazatel na retazec
*		velkost tabulky
*	Vystup: hashovaci index
*/
unsigned int hash_function(const char *str, unsigned htab_size);

#endif
