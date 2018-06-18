package ija.ija2016.homework2.model.cardimp;

import ija.ija2016.homework2.model.cards.Card;
import ija.ija2016.homework2.model.cards.CardDeck;
import ija.ija2016.homework2.model.cards.CardStack;

public class CardStackImp implements CardStack{
	private int siz;
	private int number_cards;
        protected Card c_stack[];

	public CardStackImp(int size){
                this.c_stack = new Card[size];
                this.siz = size;
		this.number_cards = 0;
        }

	
	public int size(){
		return this.number_cards;
	}

	

	public boolean put(Card card){
		if(card.value() == 13 - this.number_cards){
			if(this.number_cards + 1 <= this.siz){
				if(this.isEmpty() || !this.get().similarColorTo(card)){
					this.number_cards += 1;
                			this.c_stack[number_cards-1] = card;
					return true;
				}
        		}
		}
		return false;
	}
	
	private void put_any(Card card){
		if(this.number_cards + 1 <= this.siz){
			this.number_cards += 1;
                	this.c_stack[number_cards-1] = card;
		}
	}

	public boolean put(CardStack stack){
		Card pom[] = new Card[stack.size()];
		int num = stack.size();

		if(stack.isEmpty()){
			return true;
		}
		if(this.isEmpty()){
			return false;
		}

		int i = 0;
		for(; i < num; i++){
			pom[i] = stack.pop();
		}
		
		CardStackImp cs = new CardStackImp(i+1);
		for(int j = 0; j < num; j++){
			cs.put_any(pom[j]);
		}


		if(this.number_cards + cs.size() <= this.siz){
			while(!cs.isEmpty()){
				this.put(cs.pop());
			}
			return true;
		}
		return false;
        }

	public Card get(){
		if(number_cards <= 0){
			return null;
		}
		else{
			return c_stack[number_cards-1];
		}
	}
	
	public Card get(int index){
		if(number_cards <= 0 || index >= number_cards){
			return null;
		}
		else{
			return c_stack[index];
		}
	}



	public Card pop(){
		if(number_cards > 0){
			number_cards -= 1;
			return this.c_stack[number_cards];
		}
		return null;
	}
	
	

	public CardStackImp pop(Card card){
		int i = 0;
		Card pom[] = new Card[this.number_cards];
		int num = this.number_cards;
                for(;i<num;i++){
			pom[i] = this.pop();
			if(pom[i].color() == card.color() && pom[i].value() == card.value()){
				i++;
				break;
			}
		}

		CardStackImp cs = new CardStackImp(i+1);
		for(int j = i - 1; j >= 0; j--){
			cs.put_any(pom[j]);
		}
		return cs;
        }

	public boolean isEmpty(){
		return (this.number_cards == 0);
	}

	/*@Override
	public boolean equals(Object obj){
		if (obj == null) {
			return false;
		}
		CardStack cs = (CardStack) obj;
		int i;
		for(i = 0; i < this.size() && i < cs.size(); i++){
			if(!cs.c_stack[i].toString().equals(this.c_stack[i].toString())){
				return false;
			}
		}
		if(i != this.size())
			return false;
		return true;
	}*/
}
