package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.model.cards.Card;
import ija.ija2016.homework2.model.cards.CardDeck;
import ija.ija2016.homework2.model.cards.CardStack;

import ija.ija2016.homework2.model.cardimp.CardImp;
import ija.ija2016.homework2.model.cardimp.CardDeckImp;
import ija.ija2016.homework2.model.cardimp.CardStackImp;

public class FactoryKlondike extends AbstractFactorySolitaire{
	@Override
	public CardImp createCard(Card.Color color, int value){
		if(value < 1 || value > 13){
			return null;
		}
		/*if(color != Card.Color.DIAMONDS){
			return null;
		}*/

		return new CardImp(color,value);
	}

	@Override
	public CardDeckImp createCardDeck(){
		return CardDeckImp.createStandardDeck();
	}

	@Override
	public CardDeckImp createTargetPack(Card.Color color){
		int x = 13;
		return new CardDeckImp(x,color);
	}

	@Override
	public CardStackImp createWorkingPack(){
		return new CardStackImp(13);
	}
}
