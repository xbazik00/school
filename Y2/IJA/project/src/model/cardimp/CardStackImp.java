package model.cardimp;

import model.cards.Card;
import model.cards.CardDeck;
import model.cards.CardStack;

public class CardStackImp implements CardStack{
	private int siz;
	private int number_cards;
        protected CardImp c_stack[];

	public CardStackImp(int size){
                this.c_stack = new CardImp[size];
                this.siz = size;
		this.number_cards = 0;
        }

	
	public int size(){
		return this.number_cards;
	}

	
	public boolean put(CardImp card){
		if((!isEmpty() && card.value() == this.get().value()-1) || (card.value() == 13 && isEmpty()) ){
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
	
	public boolean put_any(CardImp card){
		if(this.number_cards + 1 <= this.siz){
			this.number_cards += 1;
                	this.c_stack[number_cards-1] = card;
		}
		return true;
	}

	public boolean put_any(CardStackImp stack){
		if(stack == null){
			return false;
		}
		CardImp pom[] = new CardImp[stack.size()];
		int num = stack.size();

		int i = 0;
		for(; i < num; i++){
			pom[i] = stack.pop();
		}
		
		CardStackImp cs = new CardStackImp(i+1);
		for(int j = 0; j < num; j++){
			cs.put_any(pom[j]);
		}

		while(!cs.isEmpty()){
			this.put_any(cs.pop());
		}
		return true;
        }


	public boolean put(CardStackImp stack){
		if(stack == null){
			return false;
		}
		CardImp pom[] = new CardImp[stack.size()];
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



	public CardImp pop(){
		if(number_cards > 0){
			number_cards -= 1;
			return this.c_stack[number_cards];
		}
		return null;
	}
	
	

	public CardStackImp pop(CardImp card){
		int i = 0;
		CardImp pom[] = new CardImp[this.number_cards];
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
}
