/*******************************************************************************************************************************
*	Projekt 2 - Roller Coaster
*	
*	Autor: Martin Bazik
*	Datum 29.4.2016	
*
*	Popis: Implementacia synchronizacneho problemu Roller Coaster. 
*		   Vyskytuju sa tu 2 druhy procesov, vozik (car) a pasazier(passenenger).
*		   V tejto konkretnej implementacii je iba 1 vozik s obmedzenou kapacitou.
*		   Ked sa naplni, vyraza na trat, dalsi pasazieri nastupuju az ked sa vrati
*		   a vyprazdni.
*
*	Argumenty spustenia: ./proj2 P C PT RT
*		P je pocet procesov pasaziera; P>0
*		C je kapacita vozika; C > 0, P > C, P musi byt vzdy nasobkom C
*		PT je maximalna hodnota doby (v milisekundach), po kterej je generovany novy proces pre pasaziera; PT >= 0 && PT < 5001.
*		RT je maximálna hodnota doby (v milisekundach) prejazdu trati; RT >= 0 && RT < 5001.
*		Všechny parametry jsou celá čísla.
*
*********************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

//Nazov suboru
#define FIL "proj2.out"

//Deklaracia semaforou
sem_t *s_car, *s_passenger, *s_loading, *s_boarded;
sem_t *s_mutex, *s_write, *s_unloading, *s_unboarded;
sem_t *s_pass_finish, *s_mutex2, *s_counter, *s_arb_wait;

//Deklaracia globalnych premennych
int  P,C,PT,RT;

//Deklaracia ID pre zdielanu pamat
int shm_passenger_id = 0;
int shm_car_id = 0;
int shm_free_id = 0;
int shm_count_id = 0;

//Deklaracia ukazovatelov zdielanych premennych
int *number_passenger = NULL;
int *number_car = NULL;
int *free_places = NULL;
int *count=NULL;

//Deklaracia vystupneho suboru
FILE *fp=NULL;

//Deklaracia funkcii
void car();
void passenger();
void arb(pid_t *pid_arr);
int in_sem();
int in_mem();
int clear_sem();
int clear_mem();
int inc_poradie(int *number);
void args(int argc, char *argv[]);



int main(int argc, char *argv[]){
	//nacita a skontroluje argumenty
	args(argc,argv);

	//otvara vystupovy subor
	if((fp = fopen(FIL,"w+"))==NULL){
		fprintf(stderr, "Nepodarilo sa otvorit subor!\n");
		exit(1);
	}


	//Inicializacia procesov
	if(in_sem()==-1){
		fprintf(stderr,"Nepodarilo sa inicializovat semafory!\n");
		clear_sem();
		fclose(fp);
		exit(2);
	}

	//Inicializacia zdielanej pamati
	if(in_mem()==-1){
		fprintf(stderr,"Nepodarilo sa alokovat pamat!\n");
		clear_sem();
		clear_mem();
		fclose(fp);
		exit(2);
	}

	//Inicializacia zdielanych premennych
	*number_passenger = 1;
	*number_car = 1;
	*free_places = 0;
	*count = 1;

	//Deklaracia ID procesov
	pid_t pid_arr[P];
	pid_t car_pid;
	pid_t arb_pid;

	//hlavny proces vytvara prve dieta, ktorym je vozik
	car_pid = fork ();
	if (car_pid == 0) {
		car();
	}
	else if (car_pid > 0){
		//hlavny proces vytvara druhe dieta, ktorym je pomocny proces pre 
		//generovanie procesov pasazierov
		arb_pid = fork();
		
		if (arb_pid == 0){
			arb(pid_arr);
		}
		else if (arb_pid < 0){
			clear_sem();
			clear_mem();
			fclose(fp);
			kill(getpid(),SIGTERM);
			kill(getppid(),SIGTERM);
			fprintf(stderr,"Chyba pri tvorbe procesu vozika!\n");
			exit(2);
		}
	}
	else{
		clear_sem();
		clear_mem();
		fclose(fp);
		kill(getpid(),SIGTERM);
		kill(getppid(),SIGTERM);
		fprintf(stderr,"Chyba pri tvorbe procesu vozika!\n");
		exit(2);
	}

	//Cakanie na procesy na vozik a pomocny proces
	waitpid(arb_pid,NULL,0);
	waitpid(car_pid,NULL,0);

	fclose(fp);
	clear_sem();
	clear_mem();

	return 0;
}


/*
*	Nacitava a kontroluje spravnost argumentov.
*
*	Input: argc a argv zo vstupu
*/
void args(int argc, char *argv[]){
	char* end_ptr=NULL;
	if(argc == 5){
		if(isdigit(*argv[1])){
			P = strtoul(argv[1], &end_ptr, 10);
			if (P<=0){
				fprintf(stderr, "Nespravne argumenty!\n");
				exit(1);
			}
		}
		else{
			fprintf(stderr, "Nespravne argumenty!\n");
			exit(1);
		}

		if(isdigit(*argv[2])){
			C = strtoul(argv[2], &end_ptr, 10);
			if (C <= 0 || C >= P || P % C){
				fprintf(stderr, "Nespravne argumenty!\n");
				exit(1);
			}
		}
		else{
			fprintf(stderr, "Nespravne argumenty!\n");
			exit(1);
		}

		if(isdigit(*argv[3])){
			PT = strtoul(argv[3], &end_ptr, 10);
			if (PT < 0 || PT >= 5001){
				fprintf(stderr, "Nespravne argumenty!\n");
				exit(1);
			}
		}
		else{
			fprintf(stderr, "Nespravne argumenty!\n");
			exit(1);
		}

		if(isdigit(*argv[4])){
			RT = strtoul(argv[4], &end_ptr, 10);
			if (RT < 0 || RT >= 5001){
				fprintf(stderr, "Nespravne argumenty!\n");
				exit(1);
			}
		}
		else{
			fprintf(stderr, "Nespravne argumenty!\n");
			exit(1);
		}			
	}
	else{
		fprintf(stderr, "Nespravne argumenty!\n");
		exit(1);
	}
}


/*
*	Funkcia bezi v procese arb a vytvara procesy pasaziera.
*
*	Output: Pole ID procesov pasazierov
*/
void arb(pid_t *pid_arr){

	pid_t passenger_pid;
	for (int i=0; i<P; i++){

		//pomocny proces vytvara procesy pasazierov
		passenger_pid = fork();
		pid_arr[i] = passenger_pid;

		//proces sa uspi na nahodny cas medzi 0 a PT milisekund
		if(RT!=0){
			int tm =  (random() % PT);
			usleep(tm*1000);
		}
		if (passenger_pid == 0){
			//proces pasaziera
			passenger();
		}
		else if (passenger_pid < 0){
			clear_sem();
			clear_mem();
			fclose(fp);
			kill(getpid(),SIGTERM);
			kill(getppid(),SIGTERM);
			fprintf(stderr,"Chyba pri tvorbe procesu vozika!\n");
			exit(2);
		}			
	}
	sem_wait(s_arb_wait);
	//pocka na pasazieraov
	for(int i = 0; i < P; i++)
		waitpid(pid_arr[i],NULL,0);
	fclose(fp);
	exit(0);
}

/*
*	Funkcia tvori proces pasaziera.
*/
void passenger(){
	//ulozi cislo pasaziera do pomocnej premennej
	int lok_n_pass = *number_passenger;
	(*number_passenger)++;



	//start
	sem_wait(s_write);
	fprintf (fp,"%d\t: P %d\t: started\n",inc_poradie(count),lok_n_pass);
	sem_post(s_write);
	fflush(fp);

	//Caka, kym ho zavola vozik, aby mohol nastupit.
	sem_wait(s_loading);

	sem_wait(s_mutex);

	//board
	sem_wait(s_write);
	fprintf (fp,"%d\t: P %d\t: board\n",inc_poradie(count),lok_n_pass);
	sem_post(s_write);
	fflush(fp);

	//Navysi sa pocet zabratych miest.
	(*free_places)++;
	if(*free_places==C){
		sem_wait(s_write);
		fprintf (fp,"%d\t: P %d\t: board last\n",inc_poradie(count),lok_n_pass);
		sem_post(s_write);
		fflush(fp);
		
		//Vozik je plny a predava riadenie procesu vozika.
		sem_post(s_boarded);
		*free_places = 0;
	}
	else{
		sem_wait(s_write);
		fprintf (fp,"%d\t: P %d\t: board order %d\n",inc_poradie(count),lok_n_pass,*free_places);
		sem_post(s_write);
		fflush(fp);

	}
	sem_post(s_mutex);
	
	//Caka, kym ho zavola vozik, aby mohol vytupit.
	sem_wait(s_unloading);
	sem_wait(s_mutex2);

	sem_wait(s_write);
	fprintf (fp,"%d\t: P %d\t: unboard\n",inc_poradie(count),lok_n_pass);
	sem_post(s_write);
	fflush(fp);

	//Navysi sa pocet volnych miest.
	(*free_places)++;
	if(*free_places==C){
		sem_wait(s_write);
		fprintf (fp,"%d\t: P %d\t: unboard last\n",inc_poradie(count),lok_n_pass);
		sem_post(s_write);
		fflush(fp);

		//Vozik je prazdny a predava riadenie procesu vozika.
		sem_post(s_unboarded);
		*free_places = 0;
	}
	else{
		sem_wait(s_write);
		fprintf (fp,"%d\t: P %d\t: unboard order %d\n",inc_poradie(count),lok_n_pass,*free_places);
		sem_post(s_write);
		fflush(fp);

	}
	
	sem_post(s_mutex2);
		
	//Caka sa na ukoncenie procesov.
	sem_wait(s_pass_finish);
	fflush(fp);
	sem_wait(s_write);
	fprintf (fp,"%d\t: P %d\t: finished\n",inc_poradie(count),lok_n_pass);
	sem_post(s_write);
	fflush(fp);
	sem_post(s_pass_finish);
	sem_post(s_arb_wait);
	fclose(fp);
	exit(0);

}


/*
*	Funkcia tvori proces vozika.
*/
void car(){

	//ulozi cislo vozika do pomocnej premennej
	int lok_n_car=*number_car;

	fflush(fp);

	sem_wait(s_write);
	fprintf (fp,"%d\t: C %d\t: started\n",inc_poradie(count),lok_n_car);
	sem_post(s_write);
	fflush(fp);

	for(int i = 0; i < (P/C);i++){
		sem_wait(s_write);
		fprintf (fp,"%d\t: C %d\t: load\n",inc_poradie(count),lok_n_car);
		sem_post(s_write);
		fflush(fp);

		for(int i =0; i<C;i++){

			//Dava podnet pre C pasazierov aby nastupili
			sem_post(s_loading);
		}
		//Vozik caka, kym vsetci nastupia.
		sem_wait(s_boarded);
		
		sem_wait(s_write);
		fprintf (fp,"%d\t: C %d\t: run\n",inc_poradie(count),lok_n_car);
		sem_post(s_write);
		fflush(fp);

		//proces sa uspi na nahodny cas medzi 0 a RT milisekund
		if(RT!=0){
			int tm =  (random() % RT);
			usleep(tm*1000);
		}
		sem_wait(s_write);
		fprintf (fp,"%d\t: C %d\t: unload\n",inc_poradie(count),lok_n_car);
		sem_post(s_write);
		fflush(fp);
		
		*free_places=0;
		for(int i =0; i<C;i++){

			//Dava podnet pre C pasazierov aby vystupili.
			sem_post(s_unloading);
		}

		//Vozik caka, kym vsetci vystupia.
		sem_wait(s_unboarded);
	}
	
	//Dava pokyn na ukoncenie procesov
	sem_post(s_pass_finish);
	
	
	//Caka na pokyn na ukoncenie procesov
	sem_wait(s_pass_finish);
	fflush(fp);

	sem_wait(s_write);
	fprintf (fp,"%d\t: C %d\t: finished\n",inc_poradie(count),lok_n_car);
	sem_post(s_write);
	fflush(fp);
	sem_post(s_pass_finish);
	fclose(fp);

	exit(0);
}

/*
*	Funkcia inkrementuje poracie vypisu.
*
*	Input: number - sucasny poces
*
*	Output: number - navyseny pocet
*			former  - povodny pocet
*/
int inc_poradie(int *number){
	sem_wait(s_counter);
	int former = (*number)++;
	sem_post(s_counter);

	return former;

}

/*
*	Funkcia inicializuje semafory.
*
*	Output: vracia: -1 pri chybe
*					1 ked nenastava chyba
*/
int in_sem(){
	int err = 1;

	s_passenger = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
	if(sem_init(s_passenger, 1, 0) == -1) err = -1;
	
	s_loading = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
	if(sem_init(s_loading, 1, 0) == -1) err = -1;
	
	s_boarded = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
	if(sem_init(s_boarded, 1, 0) == -1) err = -1;
	
	s_mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
	if(sem_init(s_mutex, 1, 1) == -1) err = -1;
	
	s_write = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
	if(sem_init(s_write, 1, 1) == -1) err = -1;
	
	s_unloading = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
	if(sem_init(s_unloading, 1, 0) == -1) err = -1;
	
	s_unboarded = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
	if(sem_init(s_unboarded, 1, 0) == -1) err = -1;
		
	s_pass_finish = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
	if(sem_init(s_pass_finish, 1, 0) == -1) err = -1;
	
	s_mutex2 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
	if(sem_init(s_mutex2, 1, 1) == -1) err = -1;
	
	s_counter = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
	if(sem_init(s_counter, 1, 1) == -1) err = -1;

	s_arb_wait = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
	if(sem_init(s_arb_wait, 1, 1) == -1) err = -1;

	if(s_passenger == MAP_FAILED || s_loading == MAP_FAILED || s_boarded == MAP_FAILED || s_mutex == MAP_FAILED)
		return -1;
	if(s_write == MAP_FAILED || s_unloading == MAP_FAILED || s_unboarded == MAP_FAILED || s_mutex2 == MAP_FAILED)
		return -1;
	if(s_pass_finish == MAP_FAILED || s_counter == MAP_FAILED || s_arb_wait == MAP_FAILED)
		return -1;

	return err;
}


/*
*	Funkcia ukonci semafory.
*
*	Output: vracia: -1 pri chybe
*					1 ked nenastava chyba
*/
int clear_sem(){
	int err = 1;

	//Odmapujem pamat pre semafory
	if(munmap(s_passenger,sizeof(sem_t))==-1) err = -1;
	if(munmap(s_loading,sizeof(sem_t))==-1) err = -1;
	if(munmap(s_boarded,sizeof(sem_t))==-1) err = -1;
	if(munmap(s_mutex,sizeof(sem_t))==-1) err = -1;
	if(munmap(s_write,sizeof(sem_t))==-1) err = -1;
	if(munmap(s_unloading,sizeof(sem_t))==-1) err = -1;
	if(munmap(s_unboarded,sizeof(sem_t))==-1) err = -1;
	if(munmap(s_pass_finish,sizeof(sem_t))==-1) err = -1;
	if(munmap(s_mutex2,sizeof(sem_t))==-1) err = -1;
	if(munmap(s_counter,sizeof(sem_t))==-1) err = -1;


	//Odstranim semafory
	if(sem_destroy(s_passenger) == -1) err = -1;
	if(sem_destroy(s_loading) == -1) err = -1;
	if(sem_destroy(s_boarded) == -1) err = -1;
	if(sem_destroy(s_mutex) == -1) err = -1;
	if(sem_destroy(s_write) == -1) err = -1;
	if(sem_destroy(s_unloading) == -1) err = -1;
	if(sem_destroy(s_unboarded) == -1) err = -1;
	if(sem_destroy(s_pass_finish) == -1) err = -1;
	if(sem_destroy(s_mutex2) == -1) err = -1;
	if(sem_destroy(s_counter) == -1) err = -1;
	
	if(err == -1)
		fprintf(stderr, "Nepodarilo sa odstranit semafor!\n");

	return err;
}

/*
*	Funkcia inicializuje zdielanu pamat.
*
*	Output: vracia: -1 pri chybe
*					1 ked nenastava chyba
*/
int in_mem(){
	int err = 1;

	shm_passenger_id = shm_open("/shmpassxb00",O_CREAT | O_EXCL | O_RDWR , 0666);	
	ftruncate(shm_passenger_id, sizeof(int));
	if(shm_passenger_id == -1) err = -1;
	
	number_passenger = (int *) mmap(NULL,sizeof(int),  PROT_READ | PROT_WRITE, MAP_SHARED, shm_passenger_id, 0);
	if(number_passenger == MAP_FAILED) err = -1;

	shm_car_id = shm_open("/shmcarxb00",O_CREAT | O_EXCL | O_RDWR , 0666);	
	ftruncate(shm_car_id, sizeof(int));
	if(shm_car_id == -1) err = -1;
	
	number_car = (int *) mmap(NULL,sizeof(int),  PROT_READ | PROT_WRITE, MAP_SHARED, shm_car_id, 0);
	if(number_car == MAP_FAILED) err = -1;

	shm_free_id = shm_open("/shmfreexb00",O_CREAT | O_EXCL | O_RDWR , 0666);	
	ftruncate(shm_free_id, sizeof(int));
	if(shm_free_id == -1) err = -1;

	free_places = (int *) mmap(NULL,sizeof(int),  PROT_READ | PROT_WRITE, MAP_SHARED,shm_free_id, 0);
	if(free_places == MAP_FAILED) err = -1;

	shm_count_id = shm_open("/shmcountxb00",O_CREAT | O_EXCL | O_RDWR,0666);	
	ftruncate(shm_count_id, sizeof(int));
	if(shm_count_id == -1) err = -1;

	count = (int *) mmap(NULL,sizeof(int),  PROT_READ | PROT_WRITE, MAP_SHARED,shm_count_id, 0);
	if(count == MAP_FAILED) err = -1;


	return err;
}

/*
*	Funkcia uvolni pamat.
*
*	Output: vracia: -1 pri chybe
*					1 ked nenastava chyba
*/
int clear_mem(){
	int err = 1;
	
	//Odmapujem pamat
	if(munmap(number_passenger,sizeof(int))==-1) err = -1;
	if(munmap(number_car,sizeof(int))==-1) err = -1;
	if(munmap(free_places,sizeof(int))==-1) err = -1;
	if(munmap(count,sizeof(int))==-1) err = -1;

	//Odstranim link
	if(shm_unlink("/shmcountxb00")==-1) err = -1;
    if(shm_unlink("/shmfreexb00")==-1) err = -1;
    if(shm_unlink("/shmcarxb00")==-1) err = -1;
    if(shm_unlink("/shmpassxb00")==-1) err = -1;

    //Uzavriem pamat
    if(close(shm_car_id)==-1) err = -1;
    if(close(shm_passenger_id)==-1) err = -1;
    if(close(shm_count_id)==-1) err = -1;
    if(close(shm_free_id)==-1) err = -1;

    if(err == -1)
		fprintf(stderr, "Nepodarilo sa vycistit pamat!\n");

	return 1;
}