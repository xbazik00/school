package model.cards;

import model.cardimp.*;
import java.io.Serializable;

public interface CardDeck extends Serializable{
	public int size();
	public boolean put(CardImp card);
	public boolean put_any(CardImp card);
	public CardImp pop();
	public boolean isEmpty();
	public CardImp get(int index);
	public CardImp get();
}
