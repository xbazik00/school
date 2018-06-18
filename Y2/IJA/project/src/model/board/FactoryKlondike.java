package model.board;

import model.cards.Card;
import model.cards.CardDeck;
import model.cards.CardStack;

import model.cardimp.CardImp;
import model.cardimp.CardDeckImp;
import model.cardimp.CardStackImp;

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
	public CardDeckImp createShuffledCardDeck(){

		CardDeckImp cd =  CardDeckImp.createStandardDeck();
		cd.shuffleDeck();
		return cd;
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
