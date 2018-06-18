package ija.ija2016.homework1.cardpack;

public class Card{
	protected int val;
	public static enum Color{
		SPADES{
			@Override
			public String toString() {
				return "S";
			}
		},
		DIAMONDS{
			@Override
			public String toString() {
				return "D";
			}
		},
		HEARTS{
			@Override
			public String toString() {
				return "H";
			}
		},
		CLUBS{
			@Override
			public String toString() {
				return "C";
			}
		}


	}

	protected Color col;

	public Card(Color c, int value){
		this.col = c;
		this.val = value;
	}
	public Color color(){
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

}


