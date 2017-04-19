//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Martin Bazik <xbazik00@stud.fit.vutbr.cz>
// $Date:       $2017-02-26
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Martin Bazik
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
	this->root = NULL;
	this->head = NULL;
}

PriorityQueue::~PriorityQueue()
{
	Element_t *pom = NULL;
	Element_t *pom2 = NULL;
	for(pom = this->root; pom != NULL; pom = pom2){
		pom2 = pom->pNext;
		delete pom;
	}

}

void PriorityQueue::Insert(int value)
{
	// Fronta je prazdna
	if(this->root == NULL){
		this->root = new Element_t;
		this->root->pNext = NULL;
		this->root->pPrev = NULL;
		this->head = this->root;
		this->root->value = value;
	}
	else{
		Element_t *pom = NULL;
		Element_t *pom2 = NULL;
		pom = this->FindInsert(value);

		// Na fronte je jeden prvok, vkladame preden 
		if(this->root == pom && this->root == this->head){
			this->root = new Element_t;
			this->root->pNext = this->head;
			this->root->pPrev = NULL;
			this->head->pPrev = this->root;
			this->root->value = value;
		}
		// Na fronte je jeden prvok, vkladame zan
		else if(pom == NULL && this->root == this->head){

			this->head = new Element_t;
			this->root->pNext = this->head;
			this->head->pNext = NULL;
			this->head->pPrev = this->root;
			this->head->value = value;
		}
		// Na koniec
		else if(pom == NULL){
			this->head->pNext = new Element_t;
			this->head->pNext->value = value;
			this->head->pNext->pPrev = this->head;
			this->head = this->head->pNext;
			this->head->pNext = NULL;

		}
		// Na zaciatok
		else if(pom->pPrev == NULL){
			pom->pPrev = new Element_t;
			this->root = pom->pPrev;
			pom->pPrev->pNext = pom;
			pom->pPrev->pPrev = NULL;
			pom->pPrev->value = value;
		}
		// Do vnutra
		else{
			pom2 = pom->pPrev;
			pom->pPrev = new Element_t;
			pom->pPrev->pNext = pom;	
	       		pom->pPrev->pPrev = pom2;
			pom2->pNext = pom->pPrev;
			pom->pPrev->value = value;
		}

		
	}
}

bool PriorityQueue::Remove(int value)
{
	// Fronta je prazdna
	if(this->root == NULL){
		return NULL;
	}
	else{
		Element_t *pom = NULL;
		Element_t *pom2 = NULL;
		pom = this->Find(value);
		// Prvok sa tam nenachadza
		if(pom == NULL)
			return false;
		// Jeden prvok
		if(this->root == this->head){
			this->root = NULL;
			this->head = NULL;
		}
		// Zaciatok fronty
		else if(pom->pPrev == NULL){
			pom->pNext->pPrev = NULL;
			this->root = pom->pNext;
		}
		// Koniec fronty
		else if(pom->pNext == NULL){
			pom->pPrev->pNext = NULL;
			this->head = pom->pPrev;
		}
		// Vnutri
		else{
			pom->pPrev->pNext = pom->pNext;
			pom->pNext->pPrev = pom->pPrev;
		}
		delete pom;
	
	}
	return true;


}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
	Element_t *pom = NULL;
	for(pom = this->root; pom != NULL; pom = pom->pNext){
		if(pom->value == value){
			return pom;
		}
	}
	return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
	return this->root;
}

PriorityQueue::Element_t *PriorityQueue::FindInsert(int value)
{
	Element_t *pom = NULL;	
	for(pom = this->root; pom != NULL; pom = pom->pNext){
		if(pom->value >= value){
			break;
		}
	}
	return pom;
}


/*** Konec souboru tdd_code.cpp ***/
