// primes.c 
// Řešení IJC-DU1, příklad a),b), 7.5.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Hlásenie chýb.

#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>


// Tlačí chybu na stderr. Prijíma argumenty vo formáte funcie printf.
void warning_msg(const char *fmt, ...){
	va_list args;

	fprintf(stderr, "CHYBA: ");
    va_start(args, fmt);
	vfprintf(stderr,fmt,args);
	va_end (args);


}

// Tlačí chybu na stderr a ukončí program. Prijíma argumenty vo formáte funcie printf.
void fatal_error(const char *fmt, ...){
	va_list args;

	fprintf(stderr, "CHYBA: ");
    va_start(args, fmt);
	vfprintf(stderr,fmt,args);
	exit(1);

}
