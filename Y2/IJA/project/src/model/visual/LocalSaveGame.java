package model.visual;

import javax.swing.*;        
import java.awt.*;
import java.awt.event.*;
import model.cards.*;
import model.board.*;
import model.cardimp.*;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Iterator;
import java.io.Serializable;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.io.InputStream;


public class LocalSaveGame{	
	private JLayeredPane GamePanel;	// Panel for the game itself
	private ArrayList<CardDeckImp> TargetDecks;	// Array of target decks
	private	ArrayDeque<CardImp>  StockD;		// Array of stock cards
	private ArrayDeque<CardImp>  ShowD;		// Array of displayed stock cards
	private ArrayList<ArrayDeque<CardImp>> CardArr;	// Lower part - arrays of cards - face down
	private JLabel lastCard;	// Label for empty stock deck
	private ArrayList<CardStackImp> CardStacks;	// Stacks for working stacks of cards
	private ArrayList<JLabel> TLabel;	// Label for empty target deck
	private ArrayList<JLabel> WLabel;	// Label for working deck
	private CardStackImp MoveStack;		// Stack for moving more than 1 card 
	private int moveIndex;			// Index where the moving stack was originaly located

	public void setGamePanel(JLayeredPane jp) {
		this.GamePanel = jp;
	}

	public void setTargetDecks(ArrayList<CardDeckImp> td) {
		this.TargetDecks = new ArrayList<CardDeckImp>();
		for(CardDeckImp cd : td){
			this.TargetDecks.add(cd);	
		}
		//this.TargetDecks = td;
	}

	public void setStockDeck(ArrayDeque<CardImp>  sd) {
		this.StockD = new ArrayDeque<CardImp>();
		for(CardImp cd : sd){
			this.StockD.add(new CardImp(cd));
		}
	}

	public void setShowDeck(ArrayDeque<CardImp>  sd) {
		this.ShowD = new ArrayDeque<CardImp>();
		for(CardImp cd : sd){
			this.ShowD.add(new CardImp(cd));
		}
	}

	public void setCardArr(ArrayList<ArrayDeque<CardImp>>   sd) {
		this.CardArr = sd;
	}
		
	public void setlastCard(JLabel  sd) {
		this.lastCard = sd;
	}
		
	public void setCardStacks(ArrayList<CardStackImp>    sd) {
		this.CardStacks = sd;
	}
	
	public void setTLabel(ArrayList<JLabel>    sd) {
		this.TLabel = sd;
	}

	public void setWLabel(ArrayList<JLabel>    sd) {
		this.WLabel = sd;
	}

	public void setMoveStack(CardStackImp    sd) {
		this.MoveStack = sd;
	}

	public void setmoveIndex(int    sd) {
		this.moveIndex = sd;
	}

	public JLayeredPane  getGamePanel() {
		return this.GamePanel;
	}

	public ArrayList<CardDeckImp>  getTargetDecks() {
		return this.TargetDecks;
	}

	public ArrayDeque<CardImp>  getStockDeck() {			
		return this.StockD;
	}

	public ArrayDeque<CardImp> getShowDeck() {
		return this.ShowD;
	}

	public ArrayList<ArrayDeque<CardImp>> getCardArr() {
		return this.CardArr;
	}
		
	public JLabel getlastCard() {
		return this.lastCard;
	}
		
	public ArrayList<CardStackImp>  getCardStacks() {
		return this.CardStacks;
	}
		
	public ArrayList<JLabel>  getTLabel() {
		return this.TLabel;
	}

	public ArrayList<JLabel> getWLabel() {
		return this.WLabel;
	}

	public CardStackImp getMoveStack() {
		return this.MoveStack;
	}

	public int getmoveIndex() {
		return this.moveIndex;
	}

}

