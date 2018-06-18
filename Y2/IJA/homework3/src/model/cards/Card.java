package model.cards;

public interface Card{
	public int value();
	public Card.Color color();
	public int compareValue(Card c);
	public boolean isTurnedFaceUp();
	public boolean similarColorTo(Card c);
	public boolean turnFaceUp();
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
}
