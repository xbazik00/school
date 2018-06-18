package model.cards;

public interface Card{
	/**
	 * Returns value of the card.
	 */
	public int value();
	
	/**
	 * Returns color of the card.
	 */
	public Card.Color color();

	/**
	 * Compares values of the cards.
	 *
	 * @param c card to compare
 	*/
	public int compareValue(Card c);
	
	/**
	 * True if the card is turned face up, else false.
	 */
	public boolean isTurnedFaceUp();

	/**
	 * True if the cards are of a similar color, else false.
	 *
	 * @param c card to compare
	 */
	public boolean similarColorTo(Card c);

	/**
	 * Turns the card face up.
	 */
	public boolean turnFaceUp();

	/**
	 * Turns the card face down.
	 */
	public boolean turnFaceDown();

	/**
	 * Coordinates for card movement.
	 *
	 * @param x x-coordinate
	 * @param y y-coordinate
	 */
	public void dragAt(int x, int y);
	
	/**
	 * Get x drag coordinate
	 */
	public int dXAt();

	/**
	 * Get y drag coordinate
	 */
	public int dYAt();

	/**
	 * Sets card to the given location.
	 *
	 * @param x x-coordinate
	 * @param y y-coordinate
	 */
	public void setLoc(int x, int y);
	public int getXCoord();
	public int getYCoord();
	public void change();
	public void moveShow();
	public void moveStock();
	public void moveBack();
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
