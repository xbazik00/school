package model.cards;

import java.util.Stack;
import model.cardimp.*;
import java.io.Serializable;

public interface CardStack extends CardDeck{
	boolean put(CardStackImp stack);
	boolean put_any(CardStackImp stack);
	CardStackImp pop(CardImp card);
}
