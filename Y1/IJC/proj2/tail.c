// tail.c 
// Řešení IJC-DU2, příklad 1) a), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: gcc 4.9.2
// Popis: Imituje posixovy program tail

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h> 

//plati pre vstup 510 znakov na riadok
#define BUFSIZE 509 
#define DEFAULT_LINES 11

int main(int argc, char *argv[]){
	char s_cmp[]="-n";
	char filename[]="\0";
	
	//Plati pre 10 riadkov
	int lines = DEFAULT_LINES;
	char *end_ptr;
	int number_lines=0;

	int c;
	int flag=1;

	FILE *fp;

    //Rozpoznava argumenty, s ktorymi bol spusteny. Chybu zobrazuje na stderr.
    if(argc == 1){
    	flag=1;
    }

	else if(argc==2){
		strcpy(filename,argv[1]);
		flag=0;
		fp=fopen(filename,"rb");
		if (fp==NULL){
			fprintf(stderr, "Subor %s nebol najdeny!\n",filename);
			return 1;
		}
	}

	else if(argc==3){
		if(strcmp(argv[1],s_cmp)==0){		//kontrola ci obsahuje "-n"
			lines=strtod(argv[2],&end_ptr);
			if(strcmp(end_ptr, "")!=0){     //kontrola ci sa za "-n" nachadza cislo
				fprintf(stderr, "Nespravny argument cisla!\n");
				return 1;
			}
			lines++;  //pocitam s jednym riadkom navyse
			flag=1;

		}
		else{
			fprintf(stderr, "Chybne argumenty!\n");
			return 1;
		}
	}

	else if(argc==4){
		if(strcmp(argv[1],s_cmp)==0){
			strcpy(filename,argv[3]);
			lines=strtod(argv[2],&end_ptr);
			if(strcmp(end_ptr, "")!=0){
				fprintf(stderr, "Nespravny argument poctu!\n");
				return 1;
			}
			lines++;
			flag=0;
			fp=fopen(filename,"rb");
			if (fp==NULL){
				fprintf(stderr, "Subor %s nebol najdeny!\n",filename);
				return 1;
			}
		}
	}

	else{
		fprintf(stderr, "Nespravny pocet argumentov!\n");
		return 1;
	}

	//2D pole s velkostou zalozenou na premennej.
    char buffer[DEFAULT_LINES][BUFSIZE];

    //Nacitava standardny vstup
	if(flag==1){
		int check=0;
		while((c = getc(stdin))!=EOF){
			ungetc(c,stdin);
			fgets(buffer[0],BUFSIZE,stdin);
			if (strchr(buffer[0], '\n') == NULL){
				if(check++ == 0) fprintf(stderr, "Prilis dlhy riadok!\n"); //check++, aby pisaloprilis dlhy riadok iba raz
				buffer[0][BUFSIZE-2] = '\n';  
				while ((c = getc(stdin)) != '\n' && c != EOF) ;  
			}
			
			for(int i = lines-1;i>0;i--)		//posuvam v buffery
				strcpy(buffer[i],buffer[i-1]);

			number_lines++;
		}	
	}

	//Nacitava zo suboru
	if(flag==0){
		int check=0;
		while((c = getc(fp))!=EOF){
			ungetc(c,fp);
			

			fgets(buffer[0],BUFSIZE,fp);
			if (strchr(buffer[0], '\n') == NULL){
				if(check++ == 0) fprintf(stderr, "Prilis dlhy riadok!\n");
				buffer[0][BUFSIZE-2] = '\n';  
				while ((c = getc(fp)) != '\n' && c != EOF) ;  
			}
			
			for(int i = lines-1;i>0;i--)
				strcpy(buffer[i],buffer[i-1]);
			number_lines++;	
		}
		fclose(fp);	
	}

	if(lines>number_lines)
		lines=number_lines;


	//Vystup
	for(int i =lines;i>0;i--)
		printf("%s",buffer[i]);

	return 0;
}