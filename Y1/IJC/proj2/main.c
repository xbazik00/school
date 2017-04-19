// main.c
// Řešení IJC-DU2, příklad 2), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Main funkcia programu wordcount, pocita pocet vyskytu
//        urciteho retazca na zaklade hashovacej funkcie.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "io.h"
#include "hash_function.h"



#define SIZE_HTAB 555555541
/*
*   Velkost hashovacej tabulky je zavisla na hashovacej funkcii,
*   nemala by byt nasobkom hashovacej konstanty kvoli zhlukovaniu
*   vid http://srinvis.blogspot.cz/2006/07/hash-table-lengths-and-prime-numbers.html
*   , zaroven by mala byt dostatocne velka. Vseobecne je dobre si zvolit prvocislo
*   a tak som aj urobil.
*/


#define SIZE_WORD 127

void print(const char *key, const unsigned value);

int main(void){

    unsigned long length_flag = 0;
    char word[SIZE_WORD+1] = { 0 };
    htab_t *my_tab; 

    //inicializacia hashovacej tabulky
    if ((my_tab = htab_init2(SIZE_HTAB,&hash_function)) == NULL){
    	fprintf(stderr, "Chyba pri alokacii pola!\n");
        return EXIT_FAILURE;
    }	

    
    //nacitanie retazcov z stdin
    for(int count = get_word(word, SIZE_WORD, stdin); count != EOF;){
        //ked prekroci velkost slova
        if(count >= SIZE_WORD && length_flag++ == 0){
            fprintf(stderr,"Prilis dlhe slovo!\n");
        }
        
        
        if (htab_lookup_add(my_tab, word) == NULL){
            fprintf(stderr, "Chyba pri alokacii pamati pre zaznam!\n");
            htab_free(my_tab);
            return EXIT_FAILURE;
         }
         count = get_word(word, SIZE_WORD, stdin);
    }


    htab_foreach(my_tab, &print);
    htab_free(my_tab);
    return EXIT_SUCCESS;

}

void print(const char *key, const unsigned value){
    printf("%s\t%u\n", key, value);
}
