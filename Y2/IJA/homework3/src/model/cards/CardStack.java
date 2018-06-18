package model.cards;

import java.util.Stack;

public interface CardStack extends CardDeck{
	boolean put(CardStack stack);
	CardStack pop(Card card);
}
