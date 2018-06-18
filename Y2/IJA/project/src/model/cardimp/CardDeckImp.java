package model.cardimp;

import model.cards.CardDeck;
import model.cards.Card;
import model.cardimp.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class CardDeckImp implements CardDeck{
	private int siz;
	private int number_cards;
        protected CardImp c_stack[];
	private Card.Color color;


	public CardDeckImp(int size){
                this.c_stack = new CardImp[size];
                this.siz = size;
		this.number_cards = 0;
        }

	public CardDeckImp(int size, Card.Color color){
		this.c_stack = new CardImp[size];
                this.siz = size;
		this.number_cards = 0;
		this.color = color;
        }



	public static CardDeckImp createStandardDeck(){
		CardDeckImp cd = new CardDeckImp(52);
		for (int i = 1; i <=13 ; i++) cd.put_any(new CardImp(Card.Color.CLUBS,i));
		for (int i = 1; i <=13 ; i++) cd.put_any(new CardImp(Card.Color.DIAMONDS,i));
		for (int i = 1; i <=13 ; i++) cd.put_any(new CardImp(Card.Color.HEARTS,i));
		for (int i = 1; i <=13 ; i++) cd.put_any(new CardImp(Card.Color.SPADES,i));
		return cd;
	}

	public void shuffleDeck(){
		List<CardImp> list = new ArrayList<>();
		for (CardImp i : c_stack) {
			list.add(i);
		}
	
		Collections.shuffle(list);

		for (int i = 0; i < list.size(); i++) {
			c_stack[i] = list.get(i);
		} 
	}

	public int size(){
		return this.number_cards;
	}


	public boolean put(CardImp card){
		if(card.color() == this.color && card.value() == this.number_cards + 1){
			if(this.number_cards + 1 <= this.siz){
				this.number_cards += 1;
                		this.c_stack[number_cards-1] = card;
				return true;
        		}
			return false;
		}
		else{
			return false;
		}
	}
	
	public boolean put_any(CardImp card){
		if(this.number_cards + 1 <= this.siz){
			this.number_cards += 1;
                	this.c_stack[number_cards-1] = card;
			return true;
        	}
		return false;
	}

	
	public CardImp pop(){
		if(number_cards > 0){
			number_cards -= 1;
			return this.c_stack[number_cards];
		}
		return null;
	}

	public boolean isEmpty(){
		if(number_cards == 0){
			return true;
		}
		else{
			return false;
		}
	}

	public CardImp get(){
		if(number_cards <= 0){
			return null;
		}
		else{
			return c_stack[number_cards-1];
		}
        }

	public CardImp get(int index){
		if(number_cards <= 0 || index >= number_cards){
			return null;
		}
		else{
			return c_stack[index];
		}
        }

}
