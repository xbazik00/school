// primes.c 
// Řešení IJC-DU1, příklad a),b), 7.5.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Definuje makra a inline funkcie na prácu s bitovým poľom




#ifndef BIT_ARRAY_H_INCLUDED
#define BIT_ARRAY_H_INCLUDED

#include "error.h"
#include <limits.h>

// Dátový typ, na ktorom je založené pole. 
#define T unsigned long

// Definuje názov pre pole bitov.
typedef T bit_array_t[];

// Makro pre indexovanie a nastavovanie bitu.
#define DU1_SET_BIT(p,i,b)  if(b){p[(i/(sizeof(T)*CHAR_BIT))]|= (T)1 << (i%(sizeof(T)*CHAR_BIT));}\
							else {p[(i/(sizeof(T)*CHAR_BIT))]^= (T)0 << (i%(sizeof(T)*CHAR_BIT));}

// Makro pre indexovanie a čítanie bitu.
#define DU1_GET_BIT(p,i) 	((p[i/(sizeof(T)*CHAR_BIT)] & ((T)1 << (i%(sizeof(T)*CHAR_BIT)))) != 0) 

// v prípade. že veľkosť poľa je deliteľná (sizeof(T)*CHAR_BIT) nealokuje prvok navyše.
#define BA_LIMIT_SIZE(size_array)   (((size_array) % (sizeof(T)*CHAR_BIT) != 0) ? ((size_array)/(sizeof(T)*CHAR_BIT) + 1) : ((size_array)/(sizeof(T)*CHAR_BIT)))

// Defunuje a inicializuje pole, nuluje a na nultý člen ukladá veľkosť poľa.
#define ba_create(arr,size_array)   T arr[1+BA_LIMIT_SIZE(size_array)] = { 0 };\
									arr[0]=size_array

// Definuje inline fukcie, ak nie je vyuźitý použijú sa  makrá.
#ifdef USE_INLINE
// Vracia veľkosť poľa.
inline long unsigned int ba_size(bit_array_t arr) {return arr[0];}
// Nastavuje bit a vykonáva kontrolu hraníc poľa.
inline void ba_set_bit(bit_array_t arr,unsigned int index, int expression)  {if(index<=arr[0]){DU1_SET_BIT(arr,(index+(sizeof(T)*CHAR_BIT)+1),expression)}
																			else {fatal_error("Index %ld mimo rozsah 0..%ld\n",(long)index,(long) arr[0] );};}
// Číta bit a vykonáva kontrolu hraníc poľa.
inline int ba_get_bit(bit_array_t arr,unsigned int index)   {if(index<=arr[0]) {return DU1_GET_BIT(arr,(index+(sizeof(T)*CHAR_BIT)+1));}
															else {fatal_error("Index %ld mimo rozsah 0..%ld\n",(long)index,(long) arr[0] );return 0;};}
// Makrá vykonávajú to isté ako rovnomenné inline funkcie.
#else
#define ba_size(arr) arr[0]
#define ba_set_bit(arr,index,expression)    if(index<=arr[0]){DU1_SET_BIT(arr,(index+(sizeof(T)*CHAR_BIT)),expression);}\
											else{fatal_error("Index %ld mimo rozsah 0..%ld\n",(long)index,(long) arr[0] );}
#define ba_get_bit(arr,index) 	(((unsigned)index>arr[0]) ? (fatal_error("Index %ld mimo rozsah 0..%ld\n",(long)index,(long) arr[0]),0)\
								: (DU1_GET_BIT(arr,(index+(sizeof(T)*CHAR_BIT)))))
#endif
#endif