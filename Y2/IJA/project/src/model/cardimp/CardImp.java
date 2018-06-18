package model.cardimp;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import model.cards.Card;
import java.awt.image.BufferedImage;
import java.net.URL;
import javax.imageio.ImageIO;
import java.io.IOException;



public class CardImp extends JButton implements Card{
	protected int val;
	protected boolean faceUp;
	protected Card.Color col;
	protected ImageIcon cardIcon;
	private volatile int draggedAtX, draggedAtY;
	protected int state;
	private int xCoord;
	private int yCoord;


	public CardImp(CardImp ci){
		this.val = ci.val;
		this.faceUp = ci.faceUp;
		this.col = ci.col;
		this.cardIcon = ci.cardIcon;
		this.draggedAtX = ci.draggedAtX;
		this.draggedAtY = ci.draggedAtY;
		this.state = ci.state;
		this.xCoord = ci.xCoord;
		this.yCoord = ci.yCoord;
	}

	public CardImp(Card.Color c, int value){
		this.col = c;
		this.val = value;
		this.faceUp = false;
		state = 0;
		
		if(c == Card.Color.HEARTS){
			cardIcon = new ImageIcon("lib/PNG-cards-1.3/" + value + "_of_hearts.png");
		}
		else if(c == Card.Color.SPADES){
			cardIcon = new ImageIcon("lib/PNG-cards-1.3/" + value + "_of_spades.png");
		}
		else if(c == Card.Color.CLUBS){
			cardIcon = new ImageIcon("lib/PNG-cards-1.3/" + value + "_of_clubs.png");
		}
		else if(c == Card.Color.DIAMONDS){
			cardIcon = new ImageIcon("lib/PNG-cards-1.3/" + value + "_of_diamonds.png");
		}

		ImageIcon icon;
		icon = new ImageIcon("lib/PNG-cards-1.3/Back.png");

		Image image = icon.getImage();
		Image newimg = image.getScaledInstance(120, 160,  java.awt.Image.SCALE_SMOOTH);
		icon = new ImageIcon(newimg); 
		setIcon(icon);
		setSize(120,160);
		setVisible(true);
	    
		setDoubleBuffered(false);
		setMargin(new Insets(0, 0, 0, 0));
		setPreferredSize(new Dimension(120, 160));
	
	}

	public Card.Color color(){
		return this.col;	
	}

	public int value(){
		return this.val;
	}

	public void setLoc(int x, int y){
		setBounds(x,y,120,160);
		xCoord = x;
		yCoord = y;
	}

	public int getXCoord(){
		return xCoord;
	}

	public int getYCoord(){
		return yCoord;
	}


	public void moveBack(){
		setLocation(xCoord,yCoord);
	}


	@Override
	public String toString() {
		if (val == 1)
			return "A("+col+")";
		else if(val < 11)
			return val+"("+col+")";
		else if(val == 11)
			return "J("+col+")";
		else if(val == 12)
			return "Q("+col+")";
		else
			return "K("+col+")";
	}


	@Override
	public int hashCode(){
		return this.val*200+this.col.ordinal();
	}


	public boolean equals(Object obj){
		if(this == obj)
			return true;
		if(obj instanceof Card){
			Card card = (Card) obj;
			if(card.toString().equals(this.toString()))
				return true;
		}
		return false;
	}

	public int compareValue(Card c){
		if(this.value() > c.value()){
			return (this.value() - c.value());
		}

		else if(this.value() == c.value()){
			return 0;
		}
		else{
			return -1;
		}
	}

	public boolean similarColorTo(Card c){
		if(this.color() == Card.Color.SPADES || this.color() == Card.Color.CLUBS){
			if(c.color() == Card.Color.SPADES || c.color() == Card.Color.CLUBS){
				return true;
			}
			else{
				return false;
			}
		}
		else if(this.color() == Card.Color.DIAMONDS || this.color() == Card.Color.HEARTS){
			 if(c.color() == Card.Color.DIAMONDS || c.color() == Card.Color.HEARTS){
				return true;
			 }
			 else{
			 	return false;
			 }
		}
		return false;

	}

	public boolean turnFaceUp(){
		if(this.faceUp == false){
			this.faceUp = true;
			Image image = cardIcon.getImage();
			Image newimg = image.getScaledInstance(120, 160,  java.awt.Image.SCALE_SMOOTH);
			cardIcon = new ImageIcon(newimg); 
			setIcon(cardIcon);
			setSize(120,160);
			setVisible(true);

			return true;
		}
		else{
			return false;
		}
	}

	public boolean turnFaceDown(){
		if(this.faceUp == true){
			this.faceUp = false;
			ImageIcon icon;
			icon = new ImageIcon("lib/PNG-cards-1.3/Back.png");

			Image image = icon.getImage();
			Image newimg = image.getScaledInstance(120, 160,  java.awt.Image.SCALE_SMOOTH);
			icon = new ImageIcon(newimg); 
			setIcon(icon);
			setSize(120,160);
			setVisible(true);

			return true;
		}
		else{
			return false;
		}
	}


	public boolean isTurnedFaceUp(){
		return this.faceUp;
	}

	public void makeInvisible(){
		setVisible(false);
	}

	public void makeVisible(){
		setVisible(true);
	}

	public void dragAt(int x, int y){
		draggedAtX = x;
		draggedAtY = y;
	}

	public int dXAt(){
		return draggedAtX;
	}

	public int dYAt(){
		return draggedAtY;
	}

	public void allowMovement(){
		addMouseListener(new MouseAdapter(){
			public void mousePressed(MouseEvent e){
				if(state == 1){
					draggedAtX = e.getX();
					draggedAtY = e.getY();
				}
			}
		});
		
		addMouseMotionListener(new MouseMotionAdapter(){
			public void mouseDragged(MouseEvent e){
				if(state == 1){
					setLocation(e.getX() - draggedAtX + getLocation().x,
							e.getY() - draggedAtY + getLocation().y);
				}
			}
		});
	}

	public void change(){
		if(state == 0)
			state = 1;
		else
			state = 0;
	}
	public void moveShow(){
		setLocation(200,50);
	}
	public void moveStock(){
		setLocation(50,50);
	}


}


