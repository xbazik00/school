
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
import model.visual.SaveGame;
import model.visual.LocalSaveGame;
import javax.swing.border.LineBorder;
import java.io.File;

/**
 * Solitaire GUI class. 
 *
 * This is the main class of Solitaire GUI. It contains all the important parts to be displayed.
 * 
 *  
 * @author Martin Bazik (xbazik00)
 * @version 1.0
 * @since   2017-05-08 
 */

public class SolitaireVisual implements Serializable{

	/*
	 * Attributes
	 */
	private JFrame frame;	// Main frame
	private JLabel label;
		
	private ButtonGroup buttonGroup1;
	private JPanel ParentPanel;	// The background panel
	private JPanel MenuPanel;	// Panel containing menu
	private JPanel StartGamePanel;	// Panel before starting the game
	
	private JButton NewGameButton;	// Go to StartGamePanel
	private JButton LeaveGameButton;// Leave button
	private JButton LoadGameButton;	// Load the game // TBD
	private JSlider NumberGamesSlide;	//Slider for number of games to start // TBD
	private JLabel NumberGamesLabel;

	private JButton StartGameButton;	// Starts game
	
	private JMenuBar menuBar;		// Menubar
	private JMenu menu, submenu;		// menu components
	private JMenuItem menuItem;
	private JRadioButtonMenuItem rbMenuItem;
	private JCheckBoxMenuItem cbMenuItem;

	private JLayeredPane GamePanel;	// Panel for the game itself

	private ArrayList<CardDeckImp> TargetDecks;	// Array of target decks

	private	ArrayDeque<CardImp>  StockDeck;		// Array of stock cards
	private ArrayDeque<CardImp>  ShowDeck;		// Array of displayed stock cards
	private CardDeckImp FullDeck;			// For generating the full deck of cards	

	private ArrayList<ArrayDeque<CardImp>> CardArr;	// Lower part - arrays of cards - face down

	private JLabel lastCard;	// Label for empty stock deck

	private ArrayList<CardStackImp> CardStacks;	// Stacks for working stacks of cards

	private Point endPoint;

	private ArrayList<JLabel> TLabel;	// Label for empty target deck

	private ArrayList<JLabel> WLabel;	// Label for working deck

	private CardStackImp MoveStack;		// Stack for moving more than 1 card 
	
	private int moveIndex;			// Index where the moving stack was originaly located

	private AbstractFactorySolitaire factory; // Abstract factory for cards

	private SaveGame Save;

	private LocalSaveGame LocSave;

	private ArrayList<CardImp> UndoCardB;
	private ArrayList<CardImp> UndoCardA;

	private ArrayList<Integer> UndoType;
	private ArrayList<Integer> UndoPos1;
	private ArrayList<Integer> UndoPos2;

	private int numSavedGames;

	private String file;

	private JMenu savedMenu;

	private CardImp HighLight;


	/**
	 * Creates the basis of the application. 
	 * It start here.
	 *
	 */
	private void createAndShowGUI() {
        	
		// Creates main components
        	frame = new JFrame("Solitaire");
        	frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		buttonGroup1 = new javax.swing.ButtonGroup();
		ParentPanel = new javax.swing.JPanel();
		MenuPanel = new javax.swing.JPanel();
		StartGamePanel = new javax.swing.JPanel();
		NewGameButton = new javax.swing.JButton();
		LeaveGameButton = new javax.swing.JButton();
		LoadGameButton = new javax.swing.JButton();
		NumberGamesSlide = new javax.swing.JSlider();
		NumberGamesLabel = new javax.swing.JLabel();
		StartGameButton = new javax.swing.JButton();
		menuBar = new JMenuBar();
		GamePanel = new JLayeredPane();
		lastCard = new JLabel();

		factory = new FactoryKlondike();

		TargetDecks = new ArrayList<CardDeckImp>();
		TargetDecks.add(factory.createTargetPack(Card.Color.HEARTS));
		TargetDecks.add(factory.createTargetPack(Card.Color.SPADES));
		TargetDecks.add(factory.createTargetPack(Card.Color.DIAMONDS));
		TargetDecks.add(factory.createTargetPack(Card.Color.CLUBS));
		TLabel = new ArrayList<JLabel>();
		WLabel = new ArrayList<JLabel>();


		StockDeck = new ArrayDeque<CardImp>();
		ShowDeck = new ArrayDeque<CardImp>();
		FullDeck = factory.createShuffledCardDeck();

		CardArr = new ArrayList<ArrayDeque<CardImp>>();

		CardStacks = new ArrayList<CardStackImp>();	

		MoveStack = new CardStackImp(13);

		Save = new SaveGame();

		LocSave = new LocalSaveGame();

		UndoCardB = new ArrayList<CardImp>(); 
		UndoCardA = new ArrayList<CardImp>(); 
		UndoType = new ArrayList<Integer>();
		UndoPos1 = new ArrayList<Integer>();
		UndoPos2 = new ArrayList<Integer>();
		HighLight = null;


		// Menu 
		menu = new JMenu("Game");
		menu.setMnemonic(KeyEvent.VK_A);
		menu.getAccessibleContext().setAccessibleDescription(
        		"The only menu in this program that has menu items");
		menuBar.add(menu);

		// A group of JMenuItems
		menuItem = new JMenuItem("New game",KeyEvent.VK_T);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_1, ActionEvent.ALT_MASK));
		menuItem.getAccessibleContext().setAccessibleDescription(
			"Creates new game.");
		menu.add(menuItem);

		menuItem.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {
				GamePanel = new JLayeredPane();
				lastCard = new JLabel();

				factory = new FactoryKlondike();

				TargetDecks = new ArrayList<CardDeckImp>();
				TargetDecks.add(factory.createTargetPack(Card.Color.HEARTS));
				TargetDecks.add(factory.createTargetPack(Card.Color.SPADES));
				TargetDecks.add(factory.createTargetPack(Card.Color.DIAMONDS));
				TargetDecks.add(factory.createTargetPack(Card.Color.CLUBS));
				TLabel = new ArrayList<JLabel>();
				WLabel = new ArrayList<JLabel>();

	
				StockDeck = new ArrayDeque<CardImp>();
				ShowDeck = new ArrayDeque<CardImp>();
				FullDeck = factory.createShuffledCardDeck();
		
				CardArr = new ArrayList<ArrayDeque<CardImp>>();
	
				CardStacks = new ArrayList<CardStackImp>();	
		
				MoveStack = new CardStackImp(13);
				GamePanel.removeAll();
				ParentPanel.removeAll();
				ParentPanel.repaint();
				ParentPanel.revalidate();

				StartGameActionPerformed(evt);
			}
		});

		menuItem = new JMenuItem("Add game",KeyEvent.VK_T);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_2, ActionEvent.ALT_MASK));
		menuItem.getAccessibleContext().setAccessibleDescription(
			"Adds a new game.");
		menu.add(menuItem);

		menuItem.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {
				//AddGameActionPerformed(evt);
			}
		});

		menuItem = new JMenuItem("Load game",KeyEvent.VK_T);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_3, ActionEvent.ALT_MASK));
		menuItem.getAccessibleContext().setAccessibleDescription(
			"Loads a saved game.");
		menu.add(menuItem);

		menuItem.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {
				LoadGameActionPerformed(evt,"save0.ser");
			}
		});

		menuItem = new JMenuItem("Save game",KeyEvent.VK_T);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_4, ActionEvent.ALT_MASK));
		menuItem.getAccessibleContext().setAccessibleDescription(
			"Saves a game.");
		menu.add(menuItem);

		menuItem.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {
				SaveGameActionPerformed(evt);
			}
		});

		menuItem = new JMenuItem("Undo",KeyEvent.VK_T);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_5, ActionEvent.ALT_MASK));
		menuItem.getAccessibleContext().setAccessibleDescription(
			"Undo a play.");
		menu.add(menuItem);

		menuItem.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {
				UndoActionPerformed(evt);
			}
		});
		
		menuItem = new JMenuItem("Hint",KeyEvent.VK_T);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_5, ActionEvent.ALT_MASK));
		menuItem.getAccessibleContext().setAccessibleDescription(
			"Gives a hint.");
		menu.add(menuItem);

		menuItem.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {
				HintActionPerformed(evt);
			}
		});

		menuItem = new JMenuItem("Quit game",KeyEvent.VK_T);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
			KeyEvent.VK_5, ActionEvent.ALT_MASK));
		menuItem.getAccessibleContext().setAccessibleDescription(
			"Quits a game.");
		menu.add(menuItem);

		menuItem.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {
				LeaveGameActionPerformed(evt);
			}
		});

		//a submenu
		menu.addSeparator();
		savedMenu = new JMenu("Saved Games");
		savedMenu.setMnemonic(KeyEvent.VK_S);

		File tmp = new File("saves");
		if(tmp.exists()){	
			File[] files = new File("saves").listFiles();
			for (final File file : files) {
				menuItem = new JMenuItem(file.getName());
				menuItem.addActionListener(new java.awt.event.ActionListener() {
					public void actionPerformed(java.awt.event.ActionEvent evt) {
						LoadGameActionPerformed(evt,file.getName());
					}
				});
				savedMenu.add(menuItem);
			}
			menu.add(savedMenu);
		}



		/* End of menu items*/


		/* Setting up the board*/
		/* Individual panel for each part*/
		ParentPanel.setMinimumSize(new java.awt.Dimension(1024, 768));
		ParentPanel.setName("");
		ParentPanel.setLayout(new java.awt.CardLayout());
	

		MenuPanel.setMinimumSize(new java.awt.Dimension(1024, 768));
        	MenuPanel.setPreferredSize(new java.awt.Dimension(1024, 768));
		MenuPanel.setLayout(null);
		
		NewGameButton.setFont(new java.awt.Font("Century", 1, 14));
		NewGameButton.setText("NEW GAME");
		NewGameButton.setMaximumSize(new java.awt.Dimension(80, 30));
		NewGameButton.setMinimumSize(new java.awt.Dimension(80, 30));
		NewGameButton.setPreferredSize(new java.awt.Dimension(80, 30));
		NewGameButton.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {
				NewGameActionPerformed(evt);
			}
		});
		MenuPanel.add(NewGameButton);
		NewGameButton.setBounds(370, 200, 260, 90);

		LeaveGameButton.setFont(new java.awt.Font("Century", 1, 14));
		LeaveGameButton.setText("LEAVE GAME");
		LeaveGameButton.setMaximumSize(new java.awt.Dimension(80, 30));
		LeaveGameButton.setMinimumSize(new java.awt.Dimension(80, 30));
		LeaveGameButton.setPreferredSize(new java.awt.Dimension(80, 30));
		LeaveGameButton.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {
				LeaveGameActionPerformed(evt);
			}
		});
		MenuPanel.add(LeaveGameButton);
		LeaveGameButton.setBounds(370, 300, 260, 90);

		LoadGameButton.setFont(new java.awt.Font("Century", 1, 14));
		LoadGameButton.setText("LOAD GAME");
		LoadGameButton.setMaximumSize(new java.awt.Dimension(80, 30));
		LoadGameButton.setMinimumSize(new java.awt.Dimension(80, 30));
		LoadGameButton.setPreferredSize(new java.awt.Dimension(80, 30));
		LoadGameButton.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {
				StartGameActionPerformed(evt);
				LoadGameActionPerformed(evt,"save0.ser");
			}
		});
		MenuPanel.add(LoadGameButton);
		LoadGameButton.setBounds(370, 400, 260, 90);
		
		ParentPanel.add(MenuPanel);
		
		StartGamePanel.setMinimumSize(new java.awt.Dimension(1024, 768));
        	StartGamePanel.setPreferredSize(new java.awt.Dimension(1024, 768));
		StartGamePanel.setLayout(null);

		NumberGamesSlide.setMinimum(1);
		NumberGamesSlide.setMaximum(4);
		NumberGamesSlide.setMajorTickSpacing(3);
		NumberGamesSlide.setMinorTickSpacing(1);
		NumberGamesSlide.setPaintTicks(true);
		NumberGamesSlide.setPaintLabels(true);
		
		NumberGamesLabel.setText("Number of games:");

		NumberGamesSlide.setFont(new java.awt.Font("Century", 2, 15));

		StartGamePanel.add(NumberGamesSlide);
		StartGamePanel.add(NumberGamesLabel);
		NumberGamesSlide.setBounds(370, 180, 260, 50);
		NumberGamesLabel.setBounds(420, 140, 260, 50);

		StartGameButton.setFont(new java.awt.Font("Century", 1, 14));
		StartGameButton.setText("START GAME");
		StartGameButton.setMaximumSize(new java.awt.Dimension(80, 30));
		StartGameButton.setMinimumSize(new java.awt.Dimension(80, 30));
		StartGameButton.setPreferredSize(new java.awt.Dimension(80, 30));
		StartGameButton.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {
				StartGameActionPerformed(evt);
			}
		});
		StartGamePanel.add(StartGameButton);
		StartGameButton.setBounds(370, 250, 260, 90);


		ParentPanel.add(StartGamePanel);

		GamePanel.setMinimumSize(new java.awt.Dimension(1200, 768));
        	GamePanel.setPreferredSize(new java.awt.Dimension(1200, 768));
		GamePanel.setLayout(null);


		ParentPanel.add(GamePanel);
		
		/* All panel are added*/
		/* Adds to frame*/
		frame.add(ParentPanel);
		frame.setJMenuBar(menuBar);

		/* Display the window.*/
		frame.pack();
		frame.setVisible(true);
	}

	private void HintActionPerformed(java.awt.event.ActionEvent evt){
		boolean found = false;
		for(CardImp card : StockDeck){
			GamePanel.add(card,0);
			card.turnFaceUp();
			card.change();
			card.moveShow();
			StockDeck.pop();
			ShowDeck.add(card);
			for(CardStackImp cd : CardStacks){
				if(cd.put(card)){
					cd.pop();
					HighLight = cd.get();
					HighLight.setBorder(new LineBorder(Color.RED));
					found = true;
					break;
				}
			}
			if(StockDeck.isEmpty()){
				lastCard.addMouseListener(new MouseAdapter() {
					public void mouseClicked(MouseEvent e){
						EmptyTurnActionPerformed(e);
					}
				});
			}
			if(found){
				break;
			}
		}
	}

	private void UndoActionPerformed(java.awt.event.ActionEvent evt){
		if(UndoType.get(0) == 0){
			for(CardImp card : ShowDeck){
				if(card == UndoCardA.get(0)){
					ShowDeck.removeLast();
					card.moveStock();
					card.turnFaceDown();
					card.change();
					StockDeck.addFirst(card);
					GamePanel.add(card,1);
					UndoCardA.remove(0);
					UndoCardB.remove(0);
					UndoType.remove(0);
					ParentPanel.repaint();
					ParentPanel.revalidate();
					return;
				}
			}
		}
		else if(UndoType.get(0) == 1){
			for(ArrayDeque<CardImp> cd : CardArr){
				if(!cd.isEmpty()){
					CardImp card = cd.getLast();
					if(card.equals(UndoCardA.get(0))){
						cd.getLast().turnFaceDown();
						UndoCardA.remove(0);
						UndoCardB.remove(0);
						UndoType.remove(0);
						GamePanel.add(cd.getLast(),1);
						ParentPanel.repaint();
						ParentPanel.revalidate();
						return;
					}
				}
			}
		}
		else if(UndoType.get(0) == 2){
			int pos1 = UndoPos1.get(0);
			int pos2 = UndoPos2.get(0);

			CardStacks.get(pos1).put_any(CardStacks.get(pos2).pop());


			int x = UndoCardB.get(0).getXCoord();
			int y = UndoCardB.get(0).getYCoord();

			CardStacks.get(pos1).get().setLoc(x,y);

			UndoCardA.remove(0);
			UndoCardB.remove(0);
			UndoType.remove(0);
			UndoPos2.remove(0);
			UndoPos1.remove(0);

			GamePanel.add(CardStacks.get(pos1).get(),1);
			ParentPanel.add(GamePanel);
			ParentPanel.repaint();
			ParentPanel.revalidate();
			return;

		}
		else if(UndoType.get(0) == 3){
			int pos1 = UndoPos1.get(0);
			int pos2 = UndoPos2.get(0);

			CardStacks.get(pos1).put_any(TargetDecks.get(pos2).pop());


			int x = UndoCardB.get(0).getXCoord();
			int y = UndoCardB.get(0).getYCoord();

			CardStacks.get(pos1).get().setLoc(x,y);

			UndoCardA.remove(0);
			UndoCardB.remove(0);
			UndoType.remove(0);
			UndoPos2.remove(0);
			UndoPos1.remove(0);
			GamePanel.add(CardStacks.get(pos1).get(),1);
			ParentPanel.add(GamePanel);
			ParentPanel.repaint();
			ParentPanel.revalidate();
			return;

		}
		else if(UndoType.get(0) == 4){
			int pos2 = UndoPos2.get(0);

			ShowDeck.add(CardStacks.get(pos2).pop());


			int x = UndoCardB.get(0).getXCoord();
			int y = UndoCardB.get(0).getYCoord();

			ShowDeck.getLast().moveShow();

			UndoCardA.remove(0);
			UndoCardB.remove(0);
			UndoType.remove(0);
			UndoPos2.remove(0);
			ParentPanel.add(GamePanel);
			ParentPanel.repaint();
			ParentPanel.revalidate();
			return;
		}
		else if(UndoType.get(0) == 5){
			int pos2 = UndoPos2.get(0);

			ShowDeck.add(TargetDecks.get(pos2).pop());


			int x = UndoCardB.get(0).getXCoord();
			int y = UndoCardB.get(0).getYCoord();

			ShowDeck.getLast().moveShow();

			UndoCardA.remove(0);
			UndoCardB.remove(0);
			UndoType.remove(0);
			UndoPos2.remove(0);
			ParentPanel.add(GamePanel);
			ParentPanel.repaint();
			ParentPanel.revalidate();
			return;
		}
		else if(UndoType.get(0) == 6){
			int pos1 = UndoPos1.get(0)-1;
			int pos2 = UndoPos2.get(0);

			TargetDecks.get(pos1).put_any(CardStacks.get(pos2).pop());


			int x = UndoCardB.get(0).getXCoord();
			int y = UndoCardB.get(0).getYCoord();

			TargetDecks.get(pos1).get().setLoc(x,y);

			UndoCardA.remove(0);
			UndoCardB.remove(0);
			UndoType.remove(0);
			UndoPos2.remove(0);
			UndoPos2.remove(0);
			ParentPanel.add(GamePanel);
			ParentPanel.repaint();
			ParentPanel.revalidate();
			return;
		}
	}

	private void SaveGameActionPerformed(java.awt.event.ActionEvent evt){
		Save.setGamePanel(GamePanel);
		Save.setTargetDecks(TargetDecks);
		Save.setStockDeck(StockDeck);
		Save.setShowDeck(ShowDeck);
		Save.setCardArr(CardArr);	
		Save.setlastCard(lastCard);	
		Save.setCardStacks(CardStacks);
		Save.setTLabel(TLabel);
		Save.setWLabel(WLabel);
		Save.setMoveStack(MoveStack);
		Save.setmoveIndex(moveIndex);

		File theDir = new File("saves");

		// if the directory does not exist, create it
		if (!theDir.exists()) {
			System.out.println("creating directory: " + theDir.getName());
			boolean result = false;

			try{
				theDir.mkdir();
				result = true;
			} 
			catch(SecurityException se){
				//handle it
			}        
			if(result) {    
				System.out.println("DIR created");  
			}
		}
		int number = -1;
		File[] files = new File("saves").listFiles();
		for (File file : files) {
			String str = file.getName();
			String numberOnly= str.replaceAll("[^0-9]", "");
			if(number < Integer.parseInt(numberOnly))
				number = Integer.parseInt(numberOnly);
		}
		number++;
		try {
			FileOutputStream fileOut = new FileOutputStream("saves/save"+number+".ser");
			ObjectOutputStream out = new ObjectOutputStream(fileOut);
			out.writeObject(Save);
			out.close();
			fileOut.close();
			System.out.printf("Serialized data is saved in save.ser");
			final int num = number;
			menuItem = new JMenuItem("save"+number+".ser");
			menuItem.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent evt) {
					LoadGameActionPerformed(evt,"save"+num+".ser");
				}
			});
			savedMenu.add(menuItem);
			menu.add(savedMenu);

		}catch(IOException i) {
			i.printStackTrace();
		}
	}

	private void LoadGameActionPerformed(java.awt.event.ActionEvent evt, String filename){
		GamePanel.removeAll();
		ParentPanel.removeAll();
		ParentPanel.repaint();
		ParentPanel.revalidate();

		Save = null; 
		try {
			FileInputStream fileIn = new FileInputStream("saves/"+filename);
			ObjectInputStream in = new ObjectInputStream(fileIn);
			Save = (SaveGame) in.readObject();
			in.close();
			fileIn.close();
		}catch(IOException i) {
			i.printStackTrace();
			return;
		}catch(ClassNotFoundException c) {
			System.out.println("Save class not found");
			c.printStackTrace();
			return;
		}

		GamePanel = Save.getGamePanel();
		TargetDecks = Save.getTargetDecks();
		StockDeck = Save.getStockDeck();
		ShowDeck = Save.getShowDeck();
		CardArr = Save.getCardArr();	
		lastCard = Save.getlastCard();	
		CardStacks = Save.getCardStacks();
		TLabel = Save.getTLabel();
		WLabel = Save.getWLabel();
		MoveStack = Save.getMoveStack();
		moveIndex = Save.getmoveIndex();


		for(CardImp c : StockDeck){
			c.addMouseListener(new myMouseAdapter(c));
			c.addMouseMotionListener(new myMotionMouseAdapter(c));	
		}

		int i = 0;
		for(ArrayDeque<CardImp> cd : CardArr){
			int j = 0;
			for(CardImp c : cd){
				c.addMouseListener(new myMouseAdapter(c));
				c.addMouseMotionListener(new myMotionMouseAdapter(c));	
				j++;
			}
			i++;
		}

		for(CardDeckImp cd : TargetDecks){
			for(int k = 0; k < cd.size(); k++){
				cd.get(k).addMouseListener(new myMouseAdapter(cd.get(k)));
				cd.get(k).addMouseMotionListener(new myMotionMouseAdapter(cd.get(k)));	
			}
		}
		for(CardImp c : ShowDeck){
			c.addMouseListener(new myMouseAdapter(c));
			c.addMouseMotionListener(new myMotionMouseAdapter(c));	
		}
		for(CardStackImp cd : CardStacks){
			for(int k = 0; k < cd.size()-1; k++){
				cd.get(k).addMouseListener(new myMouseAdapter(cd.get(k)));
				cd.get(k).addMouseMotionListener(new myMotionMouseAdapter(cd.get(k)));	
			}
	
		}



		GamePanel.setVisible(true);
		ParentPanel.add(GamePanel);
		ParentPanel.repaint();
		ParentPanel.revalidate();

	}


	/**
	 * If the stock deck is empty, the show deck is returned to stock deck.
	 */
	private void EmptyTurnActionPerformed(java.awt.event.MouseEvent evt){
		while(!ShowDeck.isEmpty()){
			CardImp ca = ShowDeck.removeLast();
			ca.moveStock();
			ca.change();
			ca.turnFaceDown();
			StockDeck.addFirst(ca);
			GamePanel.add(ca,0);
		}

	}

	/**
	 * Move card from stock deck to show deck and turns it face up.
	 */
	protected void TurnActionPerformed(java.awt.event.MouseEvent evt,CardImp c){
		GamePanel.add(c,0);
		c.turnFaceUp();
		c.change();
		c.moveShow();
		StockDeck.pop();
		ShowDeck.add(c);
		if(StockDeck.isEmpty()){
			lastCard.addMouseListener(new MouseAdapter() {
				public void mouseClicked(MouseEvent e){
					EmptyTurnActionPerformed(e);
				}		
			});
		}
	}

	/**
	 * Does everything to be done before the start of the game
	 */
	private void StartGameActionPerformed(java.awt.event.ActionEvent evt){
		/* last stock card*/
       		lastCard.setText("Cards");
		lastCard.setBounds(50, 50, 120, 160);
		lastCard.setBorder(BorderFactory.createLineBorder(Color.black));
		GamePanel.add(lastCard);
		lastCard.setVisible(true);

		/* Adds stock cards*/
		for(int i = 0; i < 24 ; i++){
			CardImp c = FullDeck.get();
			StockDeck.add(FullDeck.pop());
			c.setBounds( 50, 50, 120, 160);
			c.addMouseListener(new myMouseAdapter(c));
			c.addMouseMotionListener(new myMotionMouseAdapter(c));	
			GamePanel.add(c);
		}

		/* Adds working stack cards */
		for(int i = 0; i < 7; i++){
			int j;
			for(j = 0; j < i+1; j++){
				CardArr.add(new ArrayDeque<CardImp>());
				CardArr.get(i).add(FullDeck.pop());
				CardArr.get(i).getLast().addMouseListener(new myMouseAdapter(CardArr.get(i).getLast()));
				CardArr.get(i).getLast().addMouseMotionListener(new myMotionMouseAdapter(CardArr.get(i).getLast()));	
				CardArr.get(i).getLast().setLoc( 50 + i * 150, 300 + j * 10);
				GamePanel.add(CardArr.get(i).getLast(), new Integer(j-i));

			}
			CardArr.get(i).getLast().turnFaceUp();
			CardStacks.add(new CardStackImp(13));
			CardStacks.get(i).put_any(CardArr.get(i).getLast());
		}

		/* Adds label for target decks */
		TLabel.add(new JLabel());
       		TLabel.get(0).setText("Hearts");
		TLabel.get(0).setBounds(400, 50, 120, 160);
		TLabel.get(0).setBorder(BorderFactory.createLineBorder(Color.black));
		GamePanel.add(TLabel.get(0));
		TLabel.get(0).setVisible(true);

		TLabel.add(new JLabel());
		TLabel.get(1).setText("Spades");
		TLabel.get(1).setBounds(600, 50, 120, 160);
		TLabel.get(1).setBorder(BorderFactory.createLineBorder(Color.black));
		GamePanel.add(TLabel.get(1));
		TLabel.get(1).setVisible(true);
		
		TLabel.add(new JLabel());
		TLabel.get(2).setText("Diamonds");
		TLabel.get(2).setBounds(800, 50, 120, 160);
		TLabel.get(2).setBorder(BorderFactory.createLineBorder(Color.black));
		GamePanel.add(TLabel.get(2));
		TLabel.get(2).setVisible(true);
		
		TLabel.add(new JLabel());
		TLabel.get(3).setText("Clubs");
		TLabel.get(3).setBounds(1000, 50, 120, 160);
		TLabel.get(3).setBorder(BorderFactory.createLineBorder(Color.black));
		GamePanel.add(TLabel.get(3));
		TLabel.get(3).setVisible(true);

		/* Adds labels for empty working stacks*/
		for(int i = 0; i < 7; i++){
			WLabel.add(new JLabel());
			WLabel.get(i).setBounds(50 + i * 150, 300, 120, 160);
			WLabel.get(i).setBorder(BorderFactory.createLineBorder(Color.black));
			GamePanel.add(WLabel.get(i),-10);
			WLabel.get(i).setVisible(true);
		}
		/* Color*/
		//ParentPanel.setBackground( java.awt.Color.GREEN.darker() );
		
		ParentPanel.removeAll();
		ParentPanel.add(GamePanel);
		ParentPanel.repaint();
		ParentPanel.revalidate();
		

	}

	/**
	 * Ends game
	 */
	private void LeaveGameActionPerformed(java.awt.event.ActionEvent evt) {
 		System.exit(0);
	}

	/**
	 * Starts Panel for starting a new game
	 */
	private void NewGameActionPerformed(java.awt.event.ActionEvent evt) {
 		ParentPanel.removeAll();
		ParentPanel.add(StartGamePanel);
		ParentPanel.repaint();
		ParentPanel.revalidate();
	}



	/**
	 * This object creates the whole logic of the game and card exchange. Magic happens here.
	 * All the functions are declared for the game.
	 */
	class myMouseAdapter extends MouseAdapter {
		private CardImp card; // The card is the only attribute

		/* Constructor stores the card*/	
		public myMouseAdapter(CardImp c){
			card = c;
		}

		/* Mouseclick is declared for the stock deck and turning face up the working deck card.*/
		@Override
		public void mouseClicked(MouseEvent event){
			if(HighLight != null){
				HighLight.setBorderPainted(false);
				HighLight = null;
			}
			if(!card.isTurnedFaceUp())
				if(!StockDeck.isEmpty() && card == StockDeck.getFirst()){	// stock deck
					UndoCardB.add(0,new CardImp(card));
					TurnActionPerformed(event,card);
					UndoCardA.add(0,card);
					UndoType.add(0,0);
				}
				else{	// working deck
					for (ArrayDeque st : CardArr) {
						if(!st.isEmpty() && card == st.getLast()){
							UndoCardB.add(0,new CardImp(card));
							card.turnFaceUp();
							UndoCardA.add(0,card);
							UndoType.add(0,1);
						}
					}
				}
			else{	// for moving the cards
				card.dragAt(event.getX(),event.getY());
			}
		}

		/* 
		 * When the mouse is pressed. It defines what happens to the moving card stck and 
		 * the location of the card for movement.
		 */
		@Override
		public void mousePressed(MouseEvent event){
			if(HighLight != null){
				HighLight.setBorderPainted(false);
				HighLight = null;
			}
			boolean found = false;
			if(card.isTurnedFaceUp()){
				for (CardStackImp st : CardStacks){	// Looks for card at the begining of working stacks
					if(st.get() == card){
						found = true;
						break;
					}
				}
				if(found){	// If found saves the position
					card.dragAt(event.getX(),event.getY());
					GamePanel.setPosition(card,1);
					return;
				}
				// The stacks is moved
				found = false;
				int index = 0;
				for (CardStackImp st : CardStacks){
					for(int i = 0; i < st.size(); i++){	// Looks for the card
						if(st.get(i) == card){
							found = true;
							break;
						}
					}
					if(found){	// If found
						moveIndex = index;	// Saves the index of the stack from which we are taking cards
						CardStackImp pom = st.pop(card);	// Pops stack to the given card
						if(MoveStack.put_any(pom) == true){	// Puts the popped stack into temporary stack
							for(int i = 0; i < MoveStack.size(); i++){
								MoveStack.get(i).dragAt(event.getX(),event.getY());
							}
						}
					}

					index++;
					found = false;
				}
			}			
		}


		/* The magic. It decides what to do, depending on the position the card is released at.*/
		@Override
		public void mouseReleased(MouseEvent event){
			int x = card.getLocation().x;	// location of the card
			int y = card.getLocation().y;
			int xc;				// location of the output
			int yc;
			boolean moveB = false;
			boolean tfound = false;
			int tar_index = 0;
			for(CardDeckImp td : TargetDecks){
				if(td.get() == card){
					tfound = true;	
				}
				tar_index++;
			}
			// From target deck
			if(tfound){
				int index = 0;
				for (CardStackImp st : CardStacks) {	// To working deck
					if(!st.isEmpty()){
						xc = st.get().getXCoord();
						yc = st.get().getYCoord();
						CardImp ci = null;
						if(x >= xc-60 && x <= xc+80 && y >= yc-80 && y <= yc + 100){
							if(st.put(card)){
								UndoCardB.add(0,new CardImp(card));
								card.setLoc(xc,yc+10);
								for(CardDeckImp td : TargetDecks){
									if(td.get() == card){
										td.pop();
									}
								}
								UndoCardA.add(0,card);
								UndoType.add(0,6);
								UndoPos1.add(0,tar_index);
								UndoPos2.add(0,index);
							}
							else{
								card.moveBack();
							}
						}
						else{
							card.moveBack();
						}		
					}
					index++;
				}
				index = 0;
				for (CardStackImp st : CardStacks) { // To empty working stack
					xc = WLabel.get(index).getLocation().x;
					yc = WLabel.get(index).getLocation().y;

					CardImp ci = null;
					if(x >= xc-60 && x <= xc+80 && y >= yc-80 && y <= yc + 100){
						if(st.isEmpty() && st.put(card)){
							UndoCardB.add(0,new CardImp(card));
							card.setLoc(xc,yc);
							for(CardDeckImp td : TargetDecks){
								if(td.get() == card){
									td.pop();
								}
							}
							UndoCardA.add(0,card);
							UndoType.add(0,6);
							UndoPos1.add(0,tar_index);
							UndoPos2.add(0,index);

						}
						else{
							card.moveBack();
						}
					}
					else{
						card.moveBack();
					}
					index++;
				}

				GamePanel.setLayer(card,0);
				GamePanel.setPosition(card,0);


			}
			// From stock deck.
			else if(!ShowDeck.isEmpty() && card == ShowDeck.getLast()){
				int index = 0;
				for (CardStackImp st : CardStacks) {	// To working deck
					if(!st.isEmpty()){
						xc = st.get().getXCoord();
						yc = st.get().getYCoord();
					
						if(x >= xc-60 && x <= xc+80 && y >= yc-80 && y <= yc + 100){	// Checks output location.
							if(!st.isEmpty() &&st.put(card)){
								UndoCardB.add(0,new CardImp(card));
								card.setLoc(xc,yc+10);	// Sets location of output
								ShowDeck.remove(card);	// Removes from show deck
								UndoCardA.add(0,card);
								UndoType.add(0,4);
								UndoPos2.add(0,index);
								moveB = false;	// Do not move the card back
								break;
							}
							else{
								moveB = true;
							}
						}
						else{
							moveB = true;
						}		
					}
					index++;
				}
				if(moveB){
					index = 0;
					for (CardDeckImp dc : TargetDecks) {	// To target deck
						xc = TLabel.get(index).getLocation().x;
						yc = TLabel.get(index).getLocation().y;
						if(x >= xc-60 && x <= xc+80 && y >= yc-80 && y <= yc + 100){
							if(dc.put(card)){
								UndoCardB.add(0,card);
								card.setLoc(xc,yc);
								ShowDeck.remove(card);
								moveB = false;						
								UndoCardA.add(0,card);
								UndoType.add(0,5);
								UndoPos2.add(0,index);
								break;
							}
							else{
								moveB = true;
							}
						}
						else{
							moveB = true;
						}
						index++;
					}
				}
				if(moveB){
					index = 0;
					for (CardStackImp st : CardStacks) { // To empty working deck
						xc = WLabel.get(index).getLocation().x;
						yc = WLabel.get(index).getLocation().y;
						if(x >= xc-60 && x <= xc+80 && y >= yc-80 && y <= yc + 100){
							if(st.put(card)){
								UndoCardB.add(0,new CardImp(card));
								card.setLoc(xc,yc);
								ShowDeck.remove(card);
								moveB = false;
								UndoCardA.add(0,card);
								UndoType.add(0,4);
								UndoPos2.add(0,index);
								break;
							}
							else{
								moveB = true;
							}
						}
						else{
							moveB = true;
						}
						index++;		
					}

				}
				if(moveB){	// move it back
					card.moveShow();
				}
				GamePanel.setLayer(card,0);
				GamePanel.setPosition(card,0);	
			}
			// Move a single card from working deck
			else if(card.isTurnedFaceUp() && MoveStack.isEmpty()){
				int index2 = 0; 
				int index = 0;
				for (CardStackImp st : CardStacks) {	// To working deck
					if(!st.isEmpty()){
						xc = st.get().getXCoord();
						yc = st.get().getYCoord();
						CardImp ci = null;
						if(x >= xc-60 && x <= xc+80 && y >= yc-80 && y <= yc + 100){
							if(st.put(card)){
								UndoCardB.add(0,new CardImp(card));
								card.setLoc(xc,yc+10);
								for (ArrayDeque<CardImp> list: CardArr) { // Removes from unturned array of carsd
									if (list.contains(card)){
										list.remove(card);
										if(!list.isEmpty())
											ci = list.getLast();
										break;
									}
								}
								for (CardStackImp sa : CardStacks) {	// Removes from working stacks
									if(sa != st && sa.get() == card){
										sa.pop();
										if(ci != null)
											sa.put_any(ci);
										break;
									}
									index2++;
								}
								UndoCardA.add(0,card);
								UndoType.add(0,2);
								UndoPos1.add(0,index2);
								UndoPos2.add(0,index);

							}
							else{
								card.moveBack();
							}
						}
						else{
							card.moveBack();
						}		
					}
					index++;
				}
				index2 = 0;
				index = 0;
				for (CardDeckImp dc : TargetDecks) {	// To target deck
					xc = TLabel.get(index).getLocation().x;
					yc = TLabel.get(index).getLocation().y;
					CardImp ci = null;
					if(x >= xc-60 && x <= xc+80 && y >= yc-80 && y <= yc + 100){
						if(dc.put(card)){
							UndoCardB.add(0,new CardImp(card));
							card.setLoc(xc,yc);
							for (ArrayDeque<CardImp> list: CardArr) {
								if (list.contains(card)){
							      		list.remove(card);
									if(!list.isEmpty())
										ci = list.getLast();
								}
							}
							for (CardStackImp sa : CardStacks) {
								if(sa.get() == card){
									sa.pop();
									if(ci != null)
										sa.put_any(ci);
									break;
								}
								index2++;
							}
							UndoCardA.add(0,card);
							UndoType.add(0,3);
							UndoPos1.add(0,index2);
							UndoPos2.add(0,index);

						}
						else{
							card.moveBack();
						}
					}
					else{
						card.moveBack();
					}
					index++;
				}
				index2 = 0;
				index = 0;
				for (CardStackImp st : CardStacks) { // To empty working stack
					xc = WLabel.get(index).getLocation().x;
					yc = WLabel.get(index).getLocation().y;
					CardImp ci = null;
					if(x >= xc-60 && x <= xc+80 && y >= yc-80 && y <= yc + 100){
						if(st.isEmpty() && st.put(card)){
							UndoCardB.add(0,new CardImp(card));
							card.setLoc(xc,yc);
							for (ArrayDeque<CardImp> list: CardArr) {
								if (list.contains(card)){
									list.remove(card);
									if(!list.isEmpty())
										ci = list.getLast();
									break;
								}
							}
							for (CardStackImp sa : CardStacks) {
								if(sa != st && sa.get() == card && ci != null){
									sa.pop();
									if(ci!=null)
										sa.put_any(ci);
									break;
								}
								index2++;
							}
							UndoCardA.add(0,card);
							UndoType.add(0,2);
							UndoPos1.add(0,index2);
							UndoPos2.add(0,index);
						}
						else{
							card.moveBack();
						}
					}
					else{
						card.moveBack();
					}
					index++;
				}

				GamePanel.setLayer(card,0);
				GamePanel.setPosition(card,0);
			}
			// Moves the stack of cards from working stack
			else if(card.isTurnedFaceUp() && !MoveStack.isEmpty()){
				boolean found = false;
				int index = 0;
				for(int i = 0; i < MoveStack.size(); i++){	// Card by card
					index = 0;
					CardImp c = MoveStack.get(i);	// Gets the given card
					for (CardStackImp st : CardStacks) {	//To another working stack
						if(!st.isEmpty()){
							xc = st.get().getXCoord();
							yc = st.get().getYCoord();
							CardImp ci = null;
							if(x >= xc-60 && x <= xc+80 && y >= yc-80 && y <= yc + 100){
								if(moveIndex != index && st.put(c)){
									//UndoCardB.add(0,new CardImp(card));
									found = true;
									c.setLoc(xc,yc+10);
									for (ArrayDeque<CardImp> list: CardArr) {
										if (list.contains(card)){
							      				list.remove(card);
											if(!list.isEmpty())
												ci = list.getLast();
										}
									}
									int in = 0;
									for (CardStackImp sa : CardStacks) {
										if(in == moveIndex && sa != st){
											if(ci != null)
												sa.put_any(ci);

											break;
										}
										in++;
									}
									/*UndoCardA.add(0,card);
									UndoType.add(0,2);
									UndoPos1.add(0,in);
									UndoPos2.add(0,index);*/

									in = 0;
								}
							}
						}
						else{	// To Empty working stack
							xc = WLabel.get(index).getLocation().x;
							yc = WLabel.get(index).getLocation().y;
							CardImp ci = null;
							if(x >= xc-60 && x <= xc+80 && y >= yc-80 && y <= yc + 100){
								if(moveIndex != index && st.put(c)){
									found = true;
									c.setLoc(xc,yc);
									for (ArrayDeque<CardImp> list: CardArr) {
										if (list.contains(card)){
							      				list.remove(card);
											if(!list.isEmpty())
												ci = list.getLast();
										}
									}
									for (CardStackImp sa : CardStacks) {
										if(sa != st && index == 0){
											if(ci != null)
												sa.put_any(ci);

											break;
										}
									}
								}
							}
						}
						index++;

					}

					if(!found){
						CardStacks.get(moveIndex).put_any(c);	
					}
					c.moveBack();
					GamePanel.setLayer(c,0);
					GamePanel.setPosition(c,0);
				}
				int size = MoveStack.size();
				for(int i = 0; i < size; i++){
					MoveStack.pop();
				}
				moveIndex = 0;
			}
		}
	}

	/**
	 * Class for dragging the cards.
	 */
	class myMotionMouseAdapter extends MouseMotionAdapter {
		private CardImp card; 
		public myMotionMouseAdapter(CardImp c){
			card = c;
		}
		@Override
		public void mouseDragged(MouseEvent event){
			boolean found = false;
			if(card.isTurnedFaceUp()){
				for (CardStackImp st : CardStacks){
					if(st.get() == card){
						found = true;
					}
				}
				for (CardDeckImp td : TargetDecks){
					if(td.get() == card){
						found = true;
					}
				}


				if(found || (!ShowDeck.isEmpty() && card == ShowDeck.getLast())){
					card.setLocation(event.getX() - card.dXAt() + card.getLocation().x,
					event.getY() - card.dYAt() + card.getLocation().y);
					return;
				}
				else{
					for(int i = 0; i < MoveStack.size(); i++){

						MoveStack.get(i).setLocation(event.getX() - MoveStack.get(i).dXAt() + MoveStack.get(i).getLocation().x,
							event.getY() - 	MoveStack.get(i).dYAt() + MoveStack.get(i).getLocation().y);

					}

				}
			}
		}
	}


	/**
	 * Constructor starts whole GUI.
	 */
	public SolitaireVisual() {
		javax.swing.SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				createAndShowGUI();
			}
		});
	}


}
