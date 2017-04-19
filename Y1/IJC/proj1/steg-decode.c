// primes.c 
// Řešení IJC-DU1, příklad b), 7.5.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Vyhľadá tajnú správu ukrytú na LSB prvočíselnych bytoch dát farieb.

#include "ppm.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include "error.h"
#include "bit_array.h"


#include "eratosthenes.h"

int main(int argc, char* argv[]){ 	
 	int count=0;
 	char chr=0;

	struct ppm* p = ppm_read(argv[1]);
	if(p==NULL)
		fatal_error("Chyba pri nacitavani suboru!\n");

	unsigned long *arr = (unsigned long*) calloc(3 * p->xsize * p->ysize + sizeof(unsigned long),1);
	if(arr==NULL){
 		fatal_error("Pamäť pre pole bitov nebola alokovaná!\n");
 	}
	arr[0]=3 * p->xsize * p->ysize;
	Eratosthenes(arr);
	
	// Prechádza LSB prvočíselných bytov a postupne ich vypisuje na stdout.
	for (int i = 2;i<3*p->xsize*p->ysize;i++){
		if (ba_get_bit(arr,i)==0){
			chr|= ((p->data[i]) & 1) << count%8;			
			if(count%8==7 && chr=='\0')
				break;
			//Vyhodnocuje tlačiteľné znaky.
			if(count%8==7 && !(isprint(chr))){
				fatal_error("Text obsahuje netlačitelné znaky!\n");
			}

			if(count%8==7 && isprint(chr)){
				printf("%c",chr);
				chr=0;
			}
			count++;
		}		
	}
	printf("\n");


	free(arr);

	// Hľadá znak '\0' na konci poľa znakov.
	if (chr!='\0')
			fatal_error("Text nie je ukončený znakom '\\0'!\n");

	free(p);
	return 0;
 }