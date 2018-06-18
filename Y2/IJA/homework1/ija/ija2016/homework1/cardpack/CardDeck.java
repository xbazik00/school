package ija.ija2016.homework1.cardpack;

import java.util.Stack;

public class CardDeck{
	private int siz;
	private Stack<Card> card_deck;
	public CardDeck(int size){
		this.card_deck = new Stack<Card>();
		this.siz = size;
	}
	public static CardDeck createStandardDeck(){
		CardDeck cd = new CardDeck(52);
		for (int i = 1; i <=13 ; i++) cd.card_deck.push(new Card(Card.Color.CLUBS,i));
		for (int i = 1; i <=13 ; i++) cd.card_deck.push(new Card(Card.Color.DIAMONDS,i));
		for (int i = 1; i <=13 ; i++) cd.card_deck.push(new Card(Card.Color.HEARTS,i));
		for (int i = 1; i <=13 ; i++) cd.card_deck.push(new Card(Card.Color.SPADES,i));
		return cd;
	}
	public int size(){
		return this.siz;
	}
	public void put(Card card){
		this.card_deck.push(card);
	}
	public Card pop(){
		return this.card_deck.pop();	
	}

}
