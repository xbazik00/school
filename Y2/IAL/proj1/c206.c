
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Bohuslav Křena, říjen 2016
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu, 
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem, 
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu, 
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free.
**/
	tDLElemPtr item = NULL;
	while(L->First != NULL){			/*Prechadzame zoznam, kym prvy prvok nie
							je prazdny ukazatel*/
		item = L->First;			/*Vzdy si ulozime ukazatel na prvy prvok*/
		L->First = L->First->rptr;		/*Potom sa posuniene s ukazatelom doprava*/
		free(item);				/*Uvolnime pamat, kde ukazoval ukazatel
							na prvy prvok*/
	}
	L->Act = NULL;					/*Stav po inicializacii*/
	L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr first = NULL;
	if((first = malloc(sizeof(struct tDLElem)))!=NULL){	/*Alokujeme pamat pre novy prvok*/
		first->data = val;			/*Ulozenie hodnoty*/
		first->rptr = L->First;			/*Nastavenie praveho ukazatela na
							povodny prvy prvok*/
		first->lptr = NULL;			/*Nastavenie laveho ukazatela na NULL*/
		if(L->First == NULL)			/*Ak bol zoznam prazdny, tak vlozeny
							prvok bude zaroven ako posledny*/
			L->Last = first;
		else					/*Inak sa nastavi lavy ukazatel
							povodneho prveho prvku na novy prvok*/
			L->First->lptr = first;
		L->First = first;			/*Novy prvok sa nastavi ako prvy*/
	}
	else
		DLError();				/*Chyba pri alokacii pamate*/
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr last = NULL;
	if((last = malloc(sizeof(struct tDLElem)))!=NULL){	/*Alokujeme pamat pre novy prvok*/
		last->data = val;			/*Ulozenie hodnoty*/
		last->rptr = NULL;			/*Nastavenie praveho ukazatela na NULL*/
		last->lptr = L->Last;			/*Nastavenie praveho ukazatela na
							povodny prvy prvok*/
		if(L->First == NULL){			/*Ak bol zoznam prazdny*/
			L->First = last;		/*Novy prvok sa nastavi ako posledny*/
			last->lptr = NULL;		/*Nastavenie laveho ukazatela na NULL*/
		}
		else{					/*Inak*/
			L->Last->rptr = last;		/*Lavy ukazatel posledneho prvku sa
							nastavi na novy prvok*/
			last->lptr = L->Last;		/*a pravy ukazatel noveho nastavi na
							posledny prvok*/
		}
		L->Last = last;				/*Novy prvok sa nastavi ako posledny*/
	}
	else
		DLError();				/*Chyba pri alokacii pamate*/
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if(L->First!=NULL)				/*Ak je zoznam prazdny*/
		*val = L->First->data;
	else
		DLError();				/*Chyba citania z prazdneho zoznamu*/
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if(L->First!=NULL)				/*Ak je zoznam prazdny*/
		*val = L->Last->data;
	else
		DLError();				/*Chyba citania z prazdneho zoznamu*/
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	tDLElemPtr item = L->First;			/*Pomocny ukazatel sa nastavi na prvy prvok*/
	if(L->First != NULL){				/*Ak je zoznam prazdny nerobime nic*/
		if(L->First == L->Act)			/*Ak je prvy prvok aktivny rusime aktivitu*/
			L->Act=NULL;
		if(L->First == L->Last){		/*Ak zoznam obsahuje iba jeden prvok
							tak zoznam sa dostava do stavu po
							inicializacii*/
			L->First = NULL;
			L->Last = NULL;
		}
		else{					/*Inak*/
			L->First = L->First->rptr;	/*Prvy prvok ukazuje na svoj pravy ukazatel*/
			L->First->lptr = NULL;		/*Lavy ukazatel noveho prveho prvku sa
							nastavi na NULL*/
		}
		free(item);				/*Uvolnime pamat*/
	}
}

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	tDLElemPtr item = L->Last;			/*Pomocny ukazatel sa nastavi na posledny prvok*/
	if(L->Last != NULL){				/*Ak je zoznam prazdny nerobime nic*/
		if(L->Last == L->Act)
			L->Act=NULL;
		if(L->First == L->Last){		/*Ak zoznam obsahuje iba jeden prvok
							tak zoznam sa dostava do stavu po
							inicializacii*/
			L->First = NULL;
			L->Last = NULL;
		}
		else{					/*Inak*/
			L->Last = L->Last->lptr;	/*Posledny prvok ukazuje na svoj lavy
							ukazatel*/
			L->Last->rptr = NULL;		/*Pravy ukazatel noveho posledneho
							prvku sa nastavi na NULL*/
		}
		free(item);				/*Uvolnime pamat*/
	}
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	tDLElemPtr item = NULL;				/*Pomocny prvok*/
	if(L->Act != NULL && L->Act != L->Last){	/*Ak neexistuje aktivny prvok alebo je
							aktivny prvok poslednym, nerobime nic*/
		item = L->Act->rptr;			/*Ulozime si pravy ukazatel aktivneho
							prvku, teda prvok ktory chceme odstranit*/
		if(item == L->Last){			/*Ak je tento prvok poslednym*/
			L->Last = L->Act;		/*Aktivny prvok sa stane poslednym*/
			L->Act->rptr = NULL;		/*Pravy ukazatel aktivneho prvku sa
							nastavi na NULL*/
		}
		else{					/*Inak*/
			item->rptr->lptr = L->Act;	/*Lavy ukazatel prvku, ktory je na
							pravo od prvku, ktory chceme odstranit,
							nastavime na aktivny prvok*/
			L->Act->rptr = item->rptr;	/*Pravy ukazatel aktivneho prvku sa
							nastavi na pravy ukazatel odstranovaneho
							prvku*/
		}
		free(item);				/*Uvolnime pamat*/
	}
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	tDLElemPtr item = NULL;				/*Pomocny prvok*/
	if(L->Act != NULL && L->Act != L->First){	/*Ak neexistuje aktivny prvok alebo je
							aktivny prvok poslednym, nerobime nic*/
		item = L->Act->lptr;			/*Ulozime si lavy ukazatel aktivneho
							prvku, teda prvok ktory chceme odstranit*/
		if(item == L->First){			/*Ak je tento prvok prvym*/
			L->First = L->Act;		/*Aktivny prvok sa stane prvym*/
			L->Act->lptr = NULL;		/*Lavy ukazatel aktivneho prvku sa
							nastavi na NULL*/
		}
		else{
			item->lptr->rptr = L->Act;	/*Pravy ukazatel prvku, ktory je na
							lavo od prvku, ktory chceme odstranit,
							nastavime na aktivny prvok*/
			L->Act->lptr = item->lptr;	/*Lavy ukazatel aktivneho prvku sa
							nastavi na lavy ukazatel odstranovaneho
							prvku*/
		}
		free(item);				/*Uvolnime pamat*/
	}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr item = NULL;				/*Pomocny prvok*/
	if(L->Act != NULL){				/*Musi existovat aktivny prvok*/
		if((item = malloc(sizeof(struct tDLElem)))!= NULL){	/*Alokacia pamate*/
			item->data = val;		/*Nastavenie noveho prvku*/
			item->lptr = L->Act;
			item->rptr = L->Act->rptr;
			L->Act->rptr = item;		/*Naviazanie praveho ukazatela
							aktivneho prvku na novy prvok*/
			if(L->Last == L->Act || L->Last == L->First)	/*Ak je aktivny prvok
							posledny alebo existuje len jeden prvok*/
				L->Last = item;		/*Novy prvok je poslednym*/
			else
				item->rptr->lptr = item;	/*Naviazanie laveho ukazatela
							nasledujuceho prvku na novy prvok*/
		}
		else
			DLError();			/*Chyba pri alokovani pamate*/
	}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr item = NULL;				/*Pomocny ukazatel*/
	if(L->Act != NULL){				/*Musi existovat aktivny prvok*/
		if((item = malloc(sizeof(struct tDLElem)))!= NULL){
			item->data = val;		/*Nastavenie noveho prvku*/
			item->lptr = L->Act->lptr;
			item->rptr = L->Act;
			L->Act->lptr = item;		/*Naviazanie laveho ukazatela aktivneho
							prvku na na novy prvok*/
			if(L->Act == L->First || L->Last == L->First)	/*Ak je aktivny prvok
							posledny alebo existuje len jeden prvok*/
				L->First = item;	/*Novy prvok je prvym*/
			else
				item->lptr->rptr = item;	/*Naviazanie praveho ukazatela
							predchadzajuceho prvku na novy prvok*/
		}
		else
			DLError();			/*Chyba pri alokovanie pamate*/
	}
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	if(L->Act != NULL)
		*val = L->Act->data;
	else
		DLError();				/*Chyba pri citani z neexistujuceho prvku*/
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if(L->Act != NULL)
		L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	if(L->Act != NULL)
		L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if(L->Act != NULL)
		L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	return L->Act != NULL;

}

/* Konec c206.c*/
