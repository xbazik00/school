// primes.c 
// Řešení IJC-DU1, příklad b), 7.5.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Spracúvava súbor vo formáte PPM,typ P6
 
#ifndef PPM_H_INCLUDED
#define PPM_H_INCLUDED

// Uchováva dáta PPM súboru
struct ppm {
	unsigned xsize;
	unsigned ysize;
	char data[]; // RGB bajty, celkem 3*xsize*ysize 
};

 struct ppm * ppm_read(const char * filename);
 int ppm_write(struct ppm *p, const char * filename);

#endif