// primes.c 
// Řešení IJC-DU1, příklad a),b), 7.5.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Definícia mplementácie eratosthenovho sita.

#include "bit_array.h"
#include "eratosthenes.h"
#include <stdio.h>
#include <math.h>
#include "error.h"

// Vyhľadá prvočísla a nastaví ich násobky na nulu. 
void Eratosthenes(bit_array_t pole){
	ba_set_bit(pole,0,1);
	ba_set_bit(pole,1,1);

	unsigned long X = pole[0];      // veľkosť na nultom člene
	unsigned int limit = sqrt(X);	// hranica potrebná na určenie prvočísel  
	for (unsigned int i = 2; i<=limit;i++){
		if (ba_get_bit(pole,i)==0){
			for(unsigned int j = i; j*i<=X; j++)  // j*i sú násobky i
			{
				ba_set_bit(pole,(i)*j,1);
			}
		}
	}
}