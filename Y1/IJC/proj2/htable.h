// htable.h
// Řešení IJC-DU2, příklad 2), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Hashovacia tabulka s deklaraciami funkcii

#ifndef HTABLE_H_INCLUDED
#define HTABLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_function.h"

struct htab_listitem{
	char *key;
	unsigned long data;
	struct htab_listitem *next;
};

typedef struct htable{
	unsigned int htab_size;
	unsigned long n;
	unsigned int (*hash_fun_ptr)(const char*, unsigned);
	struct htab_listitem *items[];
} htab_t;


/*
*	Inicializuje tabulku s danou velkostou.
*	Vstup: 	velkost tabulky
*	Vystup: ukazatel na tabulku
*			NULL pri neuspechu
*/
htab_t *htab_init(unsigned long size);

/*
*	Inicializuje tabulku s danou velkostou a explicitnou hashovacou funkciou.
*	Vstup: 	velkost tabulky
*			ukazatel na funkciu
*	Vystup: ukazatel na tabulku
*			NULL pri neuspechu
*/
htab_t *htab_init2(unsigned long size,unsigned int (*hashfnp)(const char*, unsigned));

/*
*	Funkcia vyhlada polozku na zaklade kluca a ak dany kluc nenajde
*	vytvori danu polozku.
*	Vstup: 	ukazatel na tabulku
*			ukazatel na kluc
*	Vystup: polozku tabulky
*			NULL pri neuspechu
*/
struct htab_listitem * htab_lookup_add(htab_t*,char*);

/*
*	Funkcia na vyuzitie funkcie pre vsetky polozky tabulky.
*	Vstup: 	ukazatel na tabulku
*			ukazatel na funkciu
*/
void htab_foreach(htab_t *t,void (*func)(const char*, unsigned));

/*
*	Funkcia najde a odstrani polozku so zadanym klucom ak ju nenajde
*	vypise chybu.
*	Vstup: 	ukazatel na tabulku
*			ukazatel na kluc
*/
void htab_remove(htab_t *t,char *key);

/*
*	Funkcia vycisti tabulku.
*	Vstup: 	ukazatel na tabulku
*/
void htab_clear(htab_t *t);

/*
*	Funkcia uvolni tabulku.
*	Vstup: 	ukazatel na tabulku
*/
void htab_free(htab_t *t);

#endif

