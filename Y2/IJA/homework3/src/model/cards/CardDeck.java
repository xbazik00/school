package model.cards;

public interface CardDeck{
	public int size();
	public boolean put(Card card);
	public Card pop();
	public boolean isEmpty();
	public Card get(int index);
	public Card get();
}
