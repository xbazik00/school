// primes.c 
// Řešení IJC-DU1, příklad a), 7.5.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Výpočet prvočísel, hlavný súbor.

#include <stdio.h>
#include <math.h>
#include "bit_array.h"
#include "eratosthenes.h"
#include "error.h"

#define X 202000000
#define N_DISPLAYED 10

void Eratosthenes(bit_array_t pole);

int main(){
	int c = 0;
	int result[N_DISPLAYED]={0};
	ba_create(arr,X);
	Eratosthenes(arr);
	// Uloží prvočísla v opačnom poradí
	for (int i = X; c<N_DISPLAYED && i>1;i--){
		if (ba_get_bit(arr,i)==0){
			result[N_DISPLAYED -1 -c]=i;
			c++;
		}
	}
	// Vypíše prvočísla.
	for (int i = N_DISPLAYED-c; i<N_DISPLAYED; i++)
		printf("%d\n",result[i]);
	return 0;
}
