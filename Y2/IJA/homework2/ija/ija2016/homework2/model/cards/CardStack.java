package ija.ija2016.homework2.model.cards;

import java.util.Stack;

public interface CardStack extends CardDeck{
	boolean put(CardStack stack);
	CardStack pop(Card card);
}
