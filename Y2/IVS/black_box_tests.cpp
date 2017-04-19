//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Martin Bazik <xbazik00@stud.fit.vutbr.cz>
// $Date:       $2017-02-25
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Martin Bazik
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//
/**
 * Testovacia trieda
 */
class BinaryTreeTest : public ::testing::Test{
	protected:
		BinaryTree *bt;

		virtual void SetUp(){
			bt = new BinaryTree();
		}

		virtual void TearDown(){
			delete bt;
		}
};


/*
 * Nad prazdnym
 */
TEST_F(BinaryTreeTest,Vytvorenie_prazdny){
	EXPECT_TRUE(bt->InsertNode(10).first);
}

TEST_F(BinaryTreeTest,Zrusenie_prazdny){
	EXPECT_FALSE(bt->DeleteNode(10));
}

TEST_F(BinaryTreeTest,Hladanie_prazdny){
	EXPECT_FALSE(bt->FindNode(10));
}


/**
 * Nad neprazdnym
 */
TEST_F(BinaryTreeTest,Vytvorenie_neprazdny){
	bt->InsertNode(10);
	EXPECT_TRUE(bt->InsertNode(11).first);
}

TEST_F(BinaryTreeTest,Vytvorenie_exist_neprazdny){
	bt->InsertNode(10);
	EXPECT_FALSE(bt->InsertNode(10).first);
}


TEST_F(BinaryTreeTest,Zrusenie_neprazdny){
	bt->InsertNode(10);
	EXPECT_TRUE(bt->DeleteNode(10));
}

TEST_F(BinaryTreeTest,Zrusenie_neexist_neprazdny){
	bt->InsertNode(10);
	EXPECT_FALSE(bt->DeleteNode(11));
}


TEST_F(BinaryTreeTest,Hladanie_neprazdny){
	bt->InsertNode(10);
	EXPECT_TRUE(bt->FindNode(10));
}

TEST_F(BinaryTreeTest,Hladanie_neexist_neprazdny){
	bt->InsertNode(10);
	EXPECT_FALSE(bt->FindNode(11));
}

/*
 * Axiomy 
 */
TEST_F(BinaryTreeTest,AXIOM_1){
	std::vector<BinaryTree::Node_t *> *outLeafNodes = new std::vector<BinaryTree::Node_t *>();
	for(int i = 0; i < 10; i++)
		bt->InsertNode(i);
	bt->GetLeafNodes(*outLeafNodes);
	
	for (auto it : *outLeafNodes)
	{	
		EXPECT_EQ(it->color,BinaryTree::BLACK);	
	}
	delete outLeafNodes;
}

TEST_F(BinaryTreeTest,AXIOM_2){
	std::vector<BinaryTree::Node_t *> *outNonLeafNodes = new std::vector<BinaryTree::Node_t *>();
	for(int i = 0; i < 10; i++)
		bt->InsertNode(i);
	bt->GetNonLeafNodes(*outNonLeafNodes);
	
	for (auto it : *outNonLeafNodes)
	{	
		if(it->color == BinaryTree::RED){
			EXPECT_EQ(it->pLeft->color,BinaryTree::BLACK);	
			EXPECT_EQ(it->pRight->color,BinaryTree::BLACK);	
		}
	}
	delete outNonLeafNodes;
}

TEST_F(BinaryTreeTest,AXIOM_3){
	std::vector<BinaryTree::Node_t *> *outLeafNodes = new std::vector<BinaryTree::Node_t *>();
	BinaryTree::Node_t *root = bt->GetRoot();
	int count=-1;
	int count_inside=0;
	for(int i = 0; i < 10; i++)
		bt->InsertNode(i);
	bt->GetLeafNodes(*outLeafNodes);
	
	for (auto it : *outLeafNodes)
	{	
		for(;it!=root;it = it->pParent){
			if(it->color == BinaryTree::BLACK)
				count_inside++;	
		}
		if(count != -1){
			EXPECT_EQ(count,count_inside);
		}
		count = count_inside;
		count_inside = 0;
	}
	delete outLeafNodes;
}


/*
 * Ciastkove testy
 */
TEST_F(BinaryTreeTest,Ziskat_koren){
	EXPECT_NO_THROW(bt->InsertNode(10));
	EXPECT_EQ(bt->GetRoot()->key,10);
}



/*** Konec souboru black_box_tests.cpp ***/
