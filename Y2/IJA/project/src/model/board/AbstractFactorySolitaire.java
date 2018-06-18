package model.board;

import model.cards.Card;
import model.cards.CardDeck;
import model.cards.CardStack;
import model.cardimp.*;
import java.io.Serializable;


public abstract class AbstractFactorySolitaire implements Serializable{
	public abstract CardImp createCard(Card.Color color, int value);
	public abstract CardDeckImp createCardDeck();
	public abstract CardDeckImp createShuffledCardDeck();
	public abstract CardDeckImp createTargetPack(Card.Color color);
	public abstract CardStackImp createWorkingPack();
}
