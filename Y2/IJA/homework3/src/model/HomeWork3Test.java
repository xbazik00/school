/*
 */
package model;

import model.board.AbstractFactorySolitaire;
import model.board.FactoryKlondike;
import model.cards.Card;
import model.cards.CardDeck;
import model.cards.CardStack;
import java.util.HashSet;
import java.util.Set;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

/**
 * IJA 2016/2017: Testovacia trieda pre ulohu c. 3.
 *
 * @author koci xbazik00
 */
public class HomeWork3Test {
    
    protected AbstractFactorySolitaire factory;

    @Before
    public void setUp() {
         factory = new FactoryKlondike();
    }
    
    @After
    public void tearDown() {
    }

    @Test
    public void testCard() {
        Card c1 = factory.createCard(Card.Color.HEARTS, 0);
        Card c2 = factory.createCard(Card.Color.DIAMONDS, 14);
        Card c3 = factory.createCard(Card.Color.SPADES, 5);
        Card c4 = factory.createCard(Card.Color.CLUBS, 6);
        Card c5 = factory.createCard(Card.Color.CLUBS, 6);
	Card c6 = factory.createCard(Card.Color.HEARTS, 6);

	Assert.assertNull("Karta c1 nemoze obsahovat hodnotu 0", c1);
	Assert.assertNull("Karta c2 nemoze obsahovat hodnotu 14", c2);
	Assert.assertFalse("Karta c3 nie je primarne otocena licom nahor", c3.isTurnedFaceUp());
	Assert.assertTrue("Karta c3 sa otocila", c3.turnFaceUp());
	Assert.assertTrue("Karta c3 je otocena licom nahor.", c3.isTurnedFaceUp());
	Assert.assertFalse("Karta c3 nie je mozne znovu otocit.", c3.turnFaceUp());
	Assert.assertTrue("Karty c4 a c5 musia mat zhodne hodnoty.", c4.compareValue(c5) == 0);
	Assert.assertTrue("Karty c3 a c5 nemaju zhodne hodnoty, rodiel 1.", c5.compareValue(c3) == 1);
	Assert.assertEquals("Karta c4 a c5 su zhodne.", c4, c5);
	Assert.assertTrue("Karta c4 a c5 musia mat rovnaku farbu.", c4.similarColorTo(c5));
	Assert.assertTrue("Karta c3 a c4 musia mat podobnu farbu.", c4.similarColorTo(c3));
	Assert.assertFalse("Karta c5 a c6 nemsu mat podobnu farbu.", c6.similarColorTo(c5));
	Assert.assertTrue("Karta c3 ma hodnotu 5", c3.value() == 5);
	Assert.assertTrue("Karta c3 ma farbu SPADES", c3.color() == Card.Color.SPADES);

    }

    @Test
    public void testCardDeck() {
        CardDeck deck = factory.createCardDeck();

        Assert.assertEquals("Pocet karet je 52", 52, deck.size());

        Set<Card> testCards = new HashSet<>();
        for (int i = 1; i <= 13; i++) { testCards.add(factory.createCard(Card.Color.CLUBS,i)); }
        for (int i = 1; i <= 13; i++) { testCards.add(factory.createCard(Card.Color.DIAMONDS,i)); }
        for (int i = 1; i <= 13; i++) { testCards.add(factory.createCard(Card.Color.HEARTS,i)); }
        for (int i = 1; i <= 13; i++) { testCards.add(factory.createCard(Card.Color.SPADES,i)); }

        for (int i = 0; i < 52; i++) {
            Card c = deck.pop();
            Assert.assertTrue("Spravna karta.", testCards.remove(c));
        }

        Assert.assertTrue("Testova sada musi byt prazdna", testCards.isEmpty());
    }

    
    @Test
    public void testTargetPack() {
        
        CardDeck pack = factory.createTargetPack(Card.Color.CLUBS);
        
        Card c1 = factory.createCard(Card.Color.DIAMONDS, 11);
        Card c2 = factory.createCard(Card.Color.DIAMONDS, 1);
        Card c3 = factory.createCard(Card.Color.CLUBS, 11);
        Card c4 = factory.createCard(Card.Color.CLUBS, 1);
        Card c5 = factory.createCard(Card.Color.CLUBS, 2);

        Assert.assertFalse("Nelze vlozit kartu", pack.put(c1));
	Assert.assertEquals("Pocet karet v baliku je 0.", 0, pack.size());
        Assert.assertFalse("Nelze vlozit kartu", pack.put(c2));
        Assert.assertEquals("Pocet karet v baliku je 0.", 0, pack.size());
        Assert.assertFalse("Nelze vlozit kartu", pack.put(c3));
        Assert.assertTrue("Pocet karet v baliku je 0.", pack.isEmpty());
        Assert.assertTrue("Lze vlozit kartu", pack.put(c4));
        Assert.assertEquals("Pocet karet v baliku je 1.", 1, pack.size());
        Assert.assertFalse("Nelze vlozit kartu", pack.put(c3));
        Assert.assertEquals("Pocet karet v baliku je 1.", 1, pack.size());
        Assert.assertTrue("Lze vlozit kartu", pack.put(c5));
        Assert.assertEquals("Pocet karet v baliku je 2.", 2, pack.size());
        
        Card c55 = factory.createCard(Card.Color.CLUBS, 2);
        Card c44 = factory.createCard(Card.Color.CLUBS, 1);
        
        Assert.assertEquals("Na vrcholu je karta c55", c55, pack.get());
        Assert.assertEquals("Pocet karet v baliku je 2.", 2, pack.size());
        Assert.assertEquals("Vyber karty z vrcholu", c55, pack.pop());
        Assert.assertEquals("Pocet karet v baliku je 1.", 1, pack.size());
        Assert.assertEquals("Na vrcholu je karta c44", c44, pack.get());
        Assert.assertEquals("Pocet karet v baliku je 1.", 1, pack.size());
    }

    @Test
    public void testTargetPack2(){
        CardDeck pack = factory.createTargetPack(Card.Color.CLUBS);

        for(int i = 1; i <= 13; i++){
             Assert.assertTrue("Kartu sa podarilo vlozit.", pack.put(factory.createCard(Card.Color.CLUBS, i)));
        }

        Assert.assertEquals("Pack je plny.",pack.size(),13);


    }
    
    @Test
    public void testWorkingPack() {
        
        CardStack pack = factory.createWorkingPack();
           
        Card c1 = factory.createCard(Card.Color.DIAMONDS, 11);
        Card c2 = factory.createCard(Card.Color.DIAMONDS, 13);
        Card c3 = factory.createCard(Card.Color.HEARTS, 12);
        Card c4 = factory.createCard(Card.Color.CLUBS, 12);
        Card c5 = factory.createCard(Card.Color.SPADES, 11);
        Card c6 = factory.createCard(Card.Color.HEARTS, 11);

        Assert.assertEquals("Pracovni balicek je prazdny.", 0, pack.size());
        Assert.assertFalse("Na prazdny pracovni balicek lze vlozit pouze krale.", pack.put(c1));
        Assert.assertTrue("Na prazdny pracovni balicek vkladame krale.", pack.put(c2));
        Assert.assertFalse("Na cerveneho krale lze vlozit pouze cernou damu.", pack.put(c3));
        Assert.assertEquals("Pracovni balicek obsahuje 1 kartu.", 1, pack.size());
        Assert.assertTrue("Na cerveneho krale vkladame cernou damu.", pack.put(c4));
        Assert.assertEquals("Pracovni balicek obsahuje 2 karty.", 2, pack.size());

        Assert.assertFalse("Na cernou damu lze vlozit pouze cerveneho kluka.", pack.put(c5));
        Assert.assertEquals("Pracovni balicek obsahuje 2 karty.", 2, pack.size());
        Assert.assertTrue("Na cernou damu vkladame cerveneho kluka.", pack.put(c6));
        Assert.assertEquals("Pracovni balicek obsahuje 3 karty.", 3, pack.size());
        
        CardStack s = pack.pop(factory.createCard(Card.Color.CLUBS, 12));
        Assert.assertEquals("Pracovni balicek obsahuje 1 kartu.", 1, pack.size());
        Assert.assertEquals("Pocet odebranych karet je 2.", 2, s.size());
        
        Assert.assertEquals("Na vrcholu je H(11).", factory.createCard(Card.Color.HEARTS, 11), s.pop());
        Assert.assertEquals("Na vrcholu je C(12).", factory.createCard(Card.Color.CLUBS, 12), s.pop());
        Assert.assertEquals("Odebrany balicek je prazdny.", 0, s.size());   
    }
    
    @Test
    public void testWorkingPack2() {
        
        CardStack pack1 = factory.createWorkingPack();
        CardStack pack2 = factory.createWorkingPack();
           
        pack1.put(factory.createCard(Card.Color.DIAMONDS, 13));
        pack1.put(factory.createCard(Card.Color.CLUBS, 12));
        pack1.put(factory.createCard(Card.Color.HEARTS, 11));
     
        CardStack s = pack1.pop(factory.createCard(Card.Color.CLUBS, 12));
        
        Assert.assertFalse("Nelze vlozit odebranou mnozinu (pracovni balicek je prazdny)", pack2.put(s));
        
        Assert.assertTrue("Vkladame cerveneho krale na prazdny balicek.", 
        pack2.put(factory.createCard(Card.Color.HEARTS, 13)));
       
	Assert.assertTrue("Vkladame odebranou mnozinu.", pack2.put(s));
        
        Assert.assertEquals("Pracovni balicek c. 2 obsahuje 3 karty.", 3, pack2.size());
    }

    @Test
    public void testWorkingPack3(){
        CardStack pack1 = factory.createWorkingPack();
        CardStack pack2 = factory.createWorkingPack();
        CardStack pack3 = factory.createWorkingPack();



        for(int i = 12; i >= 1; i -= 2){
	    Assert.assertTrue("Could not put red card",pack1.put(factory.createCard(Card.Color.HEARTS, i+1)));
	    Assert.assertTrue("Could not put black card.",pack1.put(factory.createCard(Card.Color.CLUBS, i)));
	}
	Assert.assertTrue("Could not put red card",pack1.put(factory.createCard(Card.Color.HEARTS, 1)));
        Assert.assertEquals("Working deck is full", pack1.size(), 13);

        for(int i = 12; i >= 1; i -= 2){
	    Assert.assertTrue("Could not put black card",pack2.put(factory.createCard(Card.Color.SPADES, i+1)));
	    Assert.assertTrue("Could not put red card.",pack2.put(factory.createCard(Card.Color.DIAMONDS, i)));
	}

        Assert.assertTrue("Could not put black card",pack2.put(factory.createCard(Card.Color.SPADES, 1)));
        Assert.assertEquals("Working deck is full", pack2.size(), 13);

        for(int i = 12; i >= 6; i -= 2){
	    Assert.assertTrue("Could not put black card",pack3.put(factory.createCard(Card.Color.SPADES, i+1)));
	    Assert.assertTrue("Could not put red card.",pack3.put(factory.createCard(Card.Color.DIAMONDS, i)));
	}

	Assert.assertEquals("Working deck is full", pack3.size(), 8);

	CardStack s = pack2.pop(factory.createCard(Card.Color.SPADES,5 ));

        Assert.assertTrue("Bol vlozeny odobraty stack.", pack2.put(s));






    }


}
