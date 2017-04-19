// primes.c 
// Řešení IJC-DU1, příklad a),b), 7.5.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Pops: Hlásenie chýb.

#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

void warning_msg(const char *fmt, ...);
void fatal_error(const char *fmt, ...);

#endif