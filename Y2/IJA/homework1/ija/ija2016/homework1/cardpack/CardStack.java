package ija.ija2016.homework1.cardpack;

import java.util.Stack;

public class CardStack{
	private int siz;
	private int number_cards;
        protected Card c_stack[];
	public CardStack(int size){
                this.c_stack = new Card[size];
                this.siz = size;
		this.number_cards = 0;
        }

	
	public int size(){
		return this.number_cards;
	}
	public void put(Card card){
		if(this.number_cards + 1 <= this.siz){
			this.number_cards += 1;
                	this.c_stack[number_cards-1] = card;
        	}
	}

	public void put(CardStack stack){
		int i = 0;
		while(!stack.isEmpty()){
                	this.put(stack.c_stack[i]);
			i++;
		}
        }
	
	public CardStack takeFrom(Card card){
		int i = 0;
		Card pom[] = new Card[this.number_cards];
		int num = this.number_cards;
                for(;i<num;i++){
			this.number_cards -= 1;
			pom[i] = this.c_stack[this.number_cards];
			if(pom[i].toString().equals(card.toString())){
				i++;
				break;
			}
		}

		//if(i >= this.siz)
		CardStack cs = new CardStack(i+1);
		for(int j = i-1; j >= 0; j--){
			cs.put(pom[j]);
		}
		return cs;
        }

	public boolean isEmpty(){
		return (this.number_cards == 0);
	}

	@Override
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
	}
	
	
	
	/*private Stack<Card> card_stack;
        public CardStack(int size){
                this.card_stack = new Stack<Card>();
                this.siz = size;
		this.number_cards = 0;
        }*/
	
	/*public void put(Card card){
		this.number_cards += 1;
                this.card_stack.push(card);
        }*/
	/*public void put(CardStack stack){
		while(!isEmpty(stack)){
			this.number_cards += 1;
                	this.card_stack.push(stack.);
		}
        }*/

        /*public CardStack takeFrom(Card card){
		int i = 0;
		Stack<Card> pom_stack = new Stack<Card>();
                while(pom_stack.push(this.card_stack.pop()).toString() != card.toString()) i++;
		System.out.println(pom_stack.push(this.card_stack.pop()).toString());
		System.out.println(pom_stack.push(this.card_stack.pop()).toString());
		System.out.println(pom_stack.push(this.card_stack.pop()).toString());
		System.out.println(card.toString());
		CardStack cs = new CardStack(3);
		while(!pom_stack.empty()){
			this.number_cards -= 1;
			cs.put(pom_stack.pop());
		}
		return cs;
        }*/

}
