// primes.c 
// Řešení IJC-DU1, příklad b), 7.5.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Spracúvava súbor vo formáte PPM,typ P6

#include "ppm.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "error.h"

// Načíta obsah PPM súboru do dynamicky alokovanej štruktúry.
// Vstup: názov súboru
// Výstup: ukazovateľ na štruktúru
 struct ppm * ppm_read(const char * filename){
	unsigned color_depth;	
 	FILE* fp;
 	unsigned long count=0; 

 	unsigned x,y;

 	if((fp = fopen(filename,"rb"))==NULL){
 		warning_msg("Súbor %s nebolo možné otvoriť!\n",filename);
 		return NULL;
 	}

	
	// Načitanie a kontrola hlavičky súboru. 	
 	if(fgetc(fp)!='P' || fgetc(fp)!='6'){
 		warning_msg("Nesprávny formát obrázku!\n");
 		if (fclose(fp) == EOF)
 			warning_msg("Nepodarilo sa zatvorit subor!\n");
 		return NULL;	
 	}

	if(fscanf(fp, "%u %u ", &x, &y)!=2){
		warning_msg("Neboli nájdené rozmery!\n");	
		if (fclose(fp) == EOF)
 			warning_msg("Nepodarilo sa zatvorit subor!\n");
 		return NULL;
	}

	if(fscanf(fp, "%u ", &color_depth)!=1 || color_depth != 255){
		warning_msg("Nebola nájdená farebná hĺbka!\n");
		 if (fclose(fp) == EOF)
 			warning_msg("Nepodarilo sa zatvorit subor!\n");
 		return NULL;
	}
	
	struct ppm* p=malloc(sizeof(struct ppm) + 3 * x * y);

 	if(p==NULL){
 		warning_msg("Pamäť pre štruktúru ppm nebola alokovaná!\n");
 		if (fclose(fp) == EOF)
 			warning_msg("Nepodarilo sa zatvorit subor!\n");
 		return NULL;
 	}

 	p->xsize = x;
 	p->ysize = y;

	for (int i = 0;  i<3*p->xsize*p->ysize;i++){
		p->data[i]=(fgetc(fp));
		count++;
	}
	if(count!=3*p->xsize*p->ysize){
		free(p);
		warning_msg("Chyba pri citani suboru!\n");
		if (fclose(fp) == EOF)
 			warning_msg("Nepodarilo sa zatvoit subor!\n");
 		return NULL;
	}
	if (fclose(fp) == EOF){
		free(p);
 		warning_msg("Nepodarilo sa zatvorit subor!\n");
 	}
	return p;

 }


// Načíta obsah PPM súboru do dynamicky alokovanej štruktúry.
// Vstup: názov súboru, ukazovateľ na štruktúru
// Výstup: 0 pri správnom priebehu, -1 pri chybe 
 int ppm_write(struct ppm *p, const char * filename){
 	FILE* fp;
 	int i;
 	unsigned long count=0; 

 	// Vytvorí súbor
 	if((fp = fopen(filename,"ab+"))==NULL){
 		warning_msg("Nebolo možné vytvoriť a otvoriť súbor %s!\n",filename);
 		return -1;
 	}

 	// Zapíše hlavičku súboru.
 	fprintf(fp, "P6\n");

 	fprintf(fp, "%d %d\n",p->xsize,p->ysize);

 	fprintf(fp, "255\n");

 	for (i = 0;  i<3*p->xsize*p->ysize;i++){
 		fprintf(fp, "%c",p->data[i]);
 		count++;
 	}
 	if(count!=3*p->xsize*p->ysize){
		warning_msg("Chyba pri citani suboru!\n");
		if (fclose(fp) == EOF)
 			warning_msg("Nepodarilo sa zatvoit subor!\n");
 		return -1;
	}
 	if (fclose(fp) == EOF)
 		warning_msg("Nepodarilo sa zatvoit subor!\n");
 	return 0;
 }

