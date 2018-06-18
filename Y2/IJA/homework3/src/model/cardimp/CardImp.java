package model.cardimp;

import model.cards.Card;

public class CardImp implements Card{
	protected int val;
	protected boolean faceUp;
	protected Card.Color col;

	public CardImp(Card.Color c, int value){
		this.col = c;
		this.val = value;
		this.faceUp = false;
	}

	public Card.Color color(){
		return this.col;	
	}

	public int value(){
		return this.val;
	}


	@Override
	public String toString() {
		if (val == 1)
			return "A("+col+")";
		else if(val < 11)
			return val+"("+col+")";
		else if(val == 11)
			return "J("+col+")";
		else if(val == 12)
			return "Q("+col+")";
		else
			return "K("+col+")";
	}


	@Override
	public int hashCode(){
		return this.val*200+this.col.ordinal();
	}


	public boolean equals(Object obj){
		if(this == obj)
			return true;
		if(obj instanceof Card){
			Card card = (Card) obj;
			if(card.toString().equals(this.toString()))
				return true;
		}
		return false;
	}

	public int compareValue(Card c){
		if(this.value() > c.value()){
			return (this.value() - c.value());
		}

		else if(this.value() == c.value()){
			return 0;
		}
		else{
			return -1;
		}
	}

	public boolean similarColorTo(Card c){
		if(this.color() == Card.Color.SPADES || this.color() == Card.Color.CLUBS){
			if(c.color() == Card.Color.SPADES || c.color() == Card.Color.CLUBS){
				return true;
			}
			else{
				return false;
			}
		}
		else if(this.color() == Card.Color.DIAMONDS || this.color() == Card.Color.HEARTS){
			 if(c.color() == Card.Color.DIAMONDS || c.color() == Card.Color.HEARTS){
				return true;
			 }
			 else{
			 	return false;
			 }
		}
		return false;

	}

	public boolean turnFaceUp(){
		if(this.faceUp == false){
			this.faceUp = true;
			return true;
		}
		else{
			return false;
		}
	}

	public boolean isTurnedFaceUp(){
		return this.faceUp;
	}
}


