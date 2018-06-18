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

/**
 *  Class used for saving th games.
 *
 *  This class holds all the important classes that need to be saved.
 *  @author Martin Bazik (xbazik00)
 */
public class SaveGame implements Serializable {	
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

	/**
	 *  GamePanel setter.
	 *
	 *  This class holds GamePanel object.
	 */
	public void setGamePanel(JLayeredPane jp) {
		this.GamePanel = jp;
	}
	
	/**
	 *  TargetDecks setter.
	 *
	 *  This class holds TargetDecks object.
	 */
	public void setTargetDecks(ArrayList<CardDeckImp> td) {
		this.TargetDecks = td;
	}

	/**
	 *  StockDeck setter.
	 *
	 *  This class holds StockDeck object.
	 */
	public void setStockDeck(ArrayDeque<CardImp>  sd) {
		this.StockD = sd;
	}

	/**
	 *  ShowDeck setter.
	 *
	 *  This class holds ShowDeck object.
	 */
	public void setShowDeck(ArrayDeque<CardImp>  sd) {
		this.ShowD = sd;
	}

	/**
	 *  CardArr setter.
	 *
	 *  This class holds CardArr object.
	 */
	public void setCardArr(ArrayList<ArrayDeque<CardImp>>   sd) {
		this.CardArr = sd;
	}

	/**
	 *  lastCard setter.
	 *
	 *  This class holds lastCard object.
	 */	
	public void setlastCard(JLabel  sd) {
		this.lastCard = sd;
	}

	/**
	 *  CardStacks setter.
	 *
	 *  This class holds CardStacks object.
	 */	
	public void setCardStacks(ArrayList<CardStackImp>    sd) {
		this.CardStacks = sd;
	}

	/**
	 *  TLabel setter.
	 *
	 *  This class holds TLabel object.
	 */
	public void setTLabel(ArrayList<JLabel>    sd) {
		this.TLabel = sd;
	}

	/**
	 *  WLabel setter.
	 *
	 *  This class holds WLabel object.
	 */
	public void setWLabel(ArrayList<JLabel>    sd) {
		this.WLabel = sd;
	}

	/**
	 *  MoveStack setter.
	 *
	 *  This class holds MoveStack object.
	 */
	public void setMoveStack(CardStackImp    sd) {
		this.MoveStack = sd;
	}

	/**
	 *  moiveIndex setter.
	 *
	 *  This class holds moveIndex object.
	 */
	public void setmoveIndex(int    sd) {
		this.moveIndex = sd;
	}

	/**
	 *  GamePanel getter.
	 *
	 *  This class returns GamePanel object.
	 */
	public JLayeredPane  getGamePanel() {
		return this.GamePanel;
	}

	/**
	 *  TargetDecks getter.
	 *
	 *  This class returns TargetDecks object.
	 */
	public ArrayList<CardDeckImp>  getTargetDecks() {
		return this.TargetDecks;
	}

	/**
	 *  StockDeck getter.
	 *
	 *  This class returns StockDeck object.
	 */
	public ArrayDeque<CardImp>  getStockDeck() {			
		return this.StockD;
	}

	/**
	 *  ShowDeck getter.
	 *
	 *  This class returns ShowDeck object.
	 */
	public ArrayDeque<CardImp> getShowDeck() {
		return this.ShowD;
	}

	/**
	 *  CardArr getter.
	 *
	 *  This class returns CardArr object.
	 */
	public ArrayList<ArrayDeque<CardImp>> getCardArr() {
		return this.CardArr;
	}
		
	/**
	 *  lastCard getter.
	 *
	 *  This class returns lastCard object.
	 */
	public JLabel getlastCard() {
		return this.lastCard;
	}
		
	/**
	 *  CardStacks getter.
	 *
	 *  This class returns CardStacks object.
	 */
	public ArrayList<CardStackImp>  getCardStacks() {
		return this.CardStacks;
	}	
	
	/**
	 *  TLabel getter.
	 *
	 *  This class returns TLabel object.
	 */	
	public ArrayList<JLabel>  getTLabel() {
		return this.TLabel;
	}

	/**
	 *  WLabel getter.
	 *
	 *  This class returns WLabel object.
	 */
	public ArrayList<JLabel> getWLabel() {
		return this.WLabel;
	}

	/**
	 *  MoveStack getter.
	 *
	 *  This class returns MoveStack object.
	 */
	public CardStackImp getMoveStack() {
		return this.MoveStack;
	}

	/**
	 *  moveIndex getter.
	 *
	 *  This class returns moveIndex object.
	 */
	public int getmoveIndex() {
		return this.moveIndex;
	}

}

