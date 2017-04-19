//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     Martin Bazik <xbazik00@stud.fit.vutbr.cz>
// $Date:       $2017-02-25
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Martin Bazik
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//

/**
 * Testovacia trieda
 */
class MatrixTest : public ::testing::Test{
	protected:
		Matrix *mx;

		virtual void SetUp(){
			mx = new Matrix();
		}

		virtual void TearDown(){
			delete mx;
		}
};

TEST_F(MatrixTest,Set_1x1){
	EXPECT_TRUE(mx->set(0,0,5));
}

TEST_F(MatrixTest,Set_1x1_mimo_maticu){
	EXPECT_FALSE(mx->set(0,1,5));
}

TEST_F(MatrixTest,Vyries_rovnicu){
	std::vector<double> result; 

	// Nastavenie mx 
	EXPECT_TRUE(mx->set(0,0,-5));

	// Prava strana
	std::vector<double> b(1);
	b[0]=10;

	// Eception thrown for not being square
	EXPECT_NO_THROW(result = mx->solveEquation(b));
}

TEST(Matrix_AxA_test,Vytvorenie_0x1_1x0_0x0){
	EXPECT_ANY_THROW(Matrix *m = new Matrix(0,1));
	EXPECT_ANY_THROW(Matrix *m = new Matrix(0,0));
	EXPECT_ANY_THROW(Matrix *m = new Matrix(1,0));
}

TEST(Matrix_AxA_test,Set){
	Matrix *m = new Matrix(5,5);
	EXPECT_TRUE(m->set(4,3,5));
	EXPECT_FALSE(m->set(5,0,5));
	delete m;
}

TEST(Matrix_AxA_test,Set_is_set){
	Matrix *m = new Matrix(5,5);
	EXPECT_TRUE(m->set(4,3,5));
	EXPECT_EQ(m->get(4,3),5);
	delete m;
}

TEST(Matrix_AxA_test,Get_mimo){
	Matrix *m = new Matrix(5,5);
	EXPECT_TRUE(m->set(4,3,5));
	EXPECT_TRUE(std::isnan(m->get(6,0)));
	delete m;
}

TEST(Matrix_AxA_test,Set_empty_vector){
	Matrix *m = new Matrix(5,5);
	std::vector<std::vector< double > > values;	
	EXPECT_FALSE(m->set(values));
	delete m;
}


TEST(Matrix_AxA_test,Set_size_vector){
	Matrix *m = new Matrix(5,5);
	std::vector<std::vector< double > > values;

	// velkost mensia
	values.resize(3,std::vector<double>(3, 0));
	EXPECT_FALSE(m->set(values));
	
	// velkost rovnaka
	values.resize(5,std::vector<double>(5, 0));
	EXPECT_TRUE(m->set(values));

	// velkost vacsia v iba jednom smere
	values.resize(4,std::vector<double>(6, 0));
	EXPECT_FALSE(m->set(values));

	delete m;
}

TEST(Matrix_AxA_test,Set_works){
	Matrix *m = new Matrix(5,5);
	std::vector<std::vector< double > > values;

	values.resize(5,std::vector<double>(5, 0));
	values[0][0] = 5;
	values[2][4] = 3;
	values[1][3] = 1;
	EXPECT_TRUE(m->set(values));
	EXPECT_EQ(m->get(0,0),5);
	EXPECT_EQ(m->get(2,4),3);
	EXPECT_EQ(m->get(1,3),1);
	EXPECT_EQ(m->get(1,1),0);
	delete m;
}

TEST(Matrix_AxA_test,EQ){
	Matrix *m1 = new Matrix(5,5);
	Matrix *m2 = new Matrix(4,4);
	Matrix *m3 = new Matrix(5,5);
	
	// Ina velkost
	EXPECT_ANY_THROW((*m1)==(*m2));
	delete m2;

	// Rovnaka velkost
	EXPECT_NO_THROW((*m1)==(*m3));
	EXPECT_TRUE((*m1)==(*m3));

	// Naplnene polom
	std::vector<std::vector< double > > values;
	
	values.resize(5,std::vector<double>(5, 0));
	values[0][0] = 5;
	values[2][4] = 3;
	values[1][3] = 1;
	EXPECT_TRUE(m1->set(values));
	EXPECT_TRUE(m3->set(values));
	EXPECT_TRUE((*m1)==(*m3));
	m3->set(0,0,4);
	EXPECT_FALSE((*m1)==(*m3));
	m1->set(0,0,4);
	EXPECT_TRUE((*m1)==(*m3));


	delete m1;
	delete m3;
}

TEST(Matrix_AxA_test,Plus){
	Matrix *m1 = new Matrix(5,5);
	Matrix *m2 = new Matrix(4,4);
	Matrix *m3 = new Matrix(5,5);

	// Ina velkost
	EXPECT_ANY_THROW((*m1)+(*m2));
	delete m2;

	// Rovnaka velkost
	EXPECT_NO_THROW((*m1)+(*m3));
	Matrix m4 = (*m1)+(*m3);

	// Musia byt zhodne
	EXPECT_TRUE((*m1)==m4);

	// Pomocne pole	
	std::vector<std::vector< double > > values;
	
	// Naplnenie pola
	values.resize(5,std::vector<double>(5, 0));
	values[0][0] = 5;
	values[2][4] = 3;
	values[1][3] = 1;

	// Naplnenie matic rovnakym polom
	EXPECT_TRUE(m1->set(values));
	EXPECT_TRUE(m3->set(values));

	m4 = (*m1)+(*m3);

	EXPECT_EQ(m4.get(0,0),10);
	EXPECT_EQ(m4.get(0,1),0);
	EXPECT_EQ(m4.get(2,4),6);
	EXPECT_EQ(m4.get(1,3),2);

	// Zmena jednej hodnoty
	m3->set(0,0,4);

	m4 = (*m1)+(*m3);

	EXPECT_EQ(m4.get(0,0),9);
	EXPECT_EQ(m4.get(0,1),0);
	EXPECT_EQ(m4.get(2,4),6);
	EXPECT_EQ(m4.get(1,3),2);

	delete m1;
	delete m3;
}

TEST(Matrix_AxA_test,Sucin){
	Matrix *m1 = new Matrix(5,5);

	// Nekompatibilne velkosti
	Matrix *m2 = new Matrix(4,4);
	EXPECT_ANY_THROW((*m1)*(*m2));
	delete m2;

	m2 = new Matrix(4,5);
	EXPECT_ANY_THROW((*m1)*(*m2));
	delete m2;

	//Kompatibilne velkosti
	m2 = new Matrix(5,4);
	EXPECT_NO_THROW((*m1)*(*m2));
	delete m1;

	m1 = new Matrix(2,5);
	
	// Nasobenie matic o rozmeroch (2,5)*(5,4)
	EXPECT_NO_THROW((*m1)*(*m2));
	Matrix m4 = (*m1)*(*m2);


	// Spravnost vypoctu
	EXPECT_TRUE(m4==(* new Matrix(2,4)));

	// Nastavenie m1
	EXPECT_TRUE(m1->set(0,0,5));
	EXPECT_TRUE(m1->set(0,2,3));
	EXPECT_TRUE(m1->set(1,0,4));
	EXPECT_TRUE(m1->set(1,3,1));
	EXPECT_TRUE(m1->set(1,4,8));

	// Nastavenie m2
	EXPECT_TRUE(m2->set(0,0,6));
	EXPECT_TRUE(m2->set(0,2,2));
	EXPECT_TRUE(m2->set(1,0,5));
	EXPECT_TRUE(m2->set(1,1,3));
	EXPECT_TRUE(m2->set(1,2,5));
	EXPECT_TRUE(m2->set(2,2,2));
	EXPECT_TRUE(m2->set(3,3,5));

	m4 = (*m1)*(*m2);

	// Kontrola
	EXPECT_EQ(m4.get(0,0),30);
	EXPECT_EQ(m4.get(0,1),0);
	EXPECT_EQ(m4.get(0,2),16);
	EXPECT_EQ(m4.get(0,3),0);
	EXPECT_EQ(m4.get(1,0),24);
	EXPECT_EQ(m4.get(1,1),0);
	EXPECT_EQ(m4.get(1,2),8);
	EXPECT_EQ(m4.get(1,3),5);

	delete m1;
	delete m2;
}


TEST(Matrix_AxA_test,Skal_sucin){
	Matrix *m1 = new Matrix(3,3);


	// Nastavenie m1
	EXPECT_TRUE(m1->set(0,0,5));
	EXPECT_TRUE(m1->set(0,1,3));
	EXPECT_TRUE(m1->set(0,2,4));
	EXPECT_TRUE(m1->set(1,0,1));
	EXPECT_TRUE(m1->set(1,1,0));
	EXPECT_TRUE(m1->set(1,2,5));
	EXPECT_TRUE(m1->set(2,0,0));
	EXPECT_TRUE(m1->set(2,1,4));
	EXPECT_TRUE(m1->set(2,2,1));

	// Skalarny sucin
	Matrix m2 = (*m1)*5;

	// Kontrola
	EXPECT_EQ(m2.get(0,0),25);
	EXPECT_EQ(m2.get(0,1),15);
	EXPECT_EQ(m2.get(0,2),20);
	EXPECT_EQ(m2.get(1,0),5);
	EXPECT_EQ(m2.get(1,1),0);
	EXPECT_EQ(m2.get(1,2),25);
	EXPECT_EQ(m2.get(2,0),0);
	EXPECT_EQ(m2.get(2,1),20);
	EXPECT_EQ(m2.get(2,2),5);

	// Skalarny sucin
	m2 = (*m1)*1.4;

	// Kontrola
	EXPECT_EQ(m2.get(0,0),7);
	EXPECT_DOUBLE_EQ(m2.get(0,1),4.2);
	EXPECT_DOUBLE_EQ(m2.get(0,2),5.6);
	EXPECT_DOUBLE_EQ(m2.get(1,0),1.4);
	EXPECT_EQ(m2.get(1,1),0);
	EXPECT_EQ(m2.get(1,2),7);
	EXPECT_EQ(m2.get(2,0),0);
	EXPECT_DOUBLE_EQ(m2.get(2,1),5.6);
	EXPECT_DOUBLE_EQ(m2.get(2,2),1.4);

	// Skalarny sucin
	m2 = (*m1)*0;

	// Kontrola
	EXPECT_EQ(m2.get(0,0),0);
	EXPECT_EQ(m2.get(0,1),0);
	EXPECT_EQ(m2.get(0,2),0);
	EXPECT_EQ(m2.get(1,0),0);
	EXPECT_EQ(m2.get(1,1),0);
	EXPECT_EQ(m2.get(1,2),0);
	EXPECT_EQ(m2.get(2,0),0);
	EXPECT_EQ(m2.get(2,1),0);
	EXPECT_EQ(m2.get(2,2),0);

	delete m1;
	
	m1 = new Matrix(1,4);
	EXPECT_TRUE(m1->set(0,0,2));
	EXPECT_TRUE(m1->set(0,1,4));
	EXPECT_TRUE(m1->set(0,2,7));
	EXPECT_TRUE(m1->set(0,3,9));

	m2 = (*m1)*9.5;

	EXPECT_EQ(m2.get(0,0),19);
	EXPECT_EQ(m2.get(0,1),38);
	EXPECT_EQ(m2.get(0,2),66.5);
	EXPECT_EQ(m2.get(0,3),85.5);

	delete m1;
}

TEST(Matrix_AxA_test,Vyries_rovnicu){
	std::vector<double> result; 

	// m1 nie je stvorcova
	Matrix *m1 = new Matrix(2,3);

	// Nastavenie m1 
	EXPECT_TRUE(m1->set(0,0,5));
	EXPECT_TRUE(m1->set(0,1,3));
	EXPECT_TRUE(m1->set(0,2,4));
	EXPECT_TRUE(m1->set(1,0,1));
	EXPECT_TRUE(m1->set(1,1,0));
	EXPECT_TRUE(m1->set(1,2,5));

	// Prava strana
	std::vector<double> b(3);
	b[0]=1;
	b[1]=2;
	b[2]=3;

	// Eception thrown for not being square
	EXPECT_ANY_THROW(result = m1->solveEquation(b));
	delete m1;

	// Stvorcova matica, ale nesedi pocet prvkov na pravej strane
	m1 = new Matrix(3,3);
	
	EXPECT_TRUE(m1->set(0,0,5));
	EXPECT_TRUE(m1->set(0,1,3));
	EXPECT_TRUE(m1->set(0,2,4));
	EXPECT_TRUE(m1->set(1,0,1));
	EXPECT_TRUE(m1->set(1,1,0));
	EXPECT_TRUE(m1->set(1,2,5));
	EXPECT_TRUE(m1->set(2,0,0));
	EXPECT_TRUE(m1->set(2,1,4));
	EXPECT_TRUE(m1->set(2,2,1));

	std::vector<double> d(2);
	b[0]=1;
	b[1]=2;


	// Eception thrown for not being square
	EXPECT_ANY_THROW(result = m1->solveEquation(d));

	// Correct size
	std::vector<double> c(3);
	c[0]=1;	
	c[1]=5;
	c[2]=6;
	EXPECT_NO_THROW(result = m1->solveEquation(c));
	
	// Kontrola
	EXPECT_DOUBLE_EQ(result[0],-1.5517241379310345);
	EXPECT_DOUBLE_EQ(result[1],1.1724137931034482);
	EXPECT_DOUBLE_EQ(result[2],1.3103448275862069);

	// Sustava bez riesenia (Determinant = 0)
	EXPECT_TRUE(m1->set(0,0,1));
	EXPECT_TRUE(m1->set(0,1,0));
	EXPECT_TRUE(m1->set(0,2,1));
	EXPECT_TRUE(m1->set(1,0,1));
	EXPECT_TRUE(m1->set(1,1,1));
	EXPECT_TRUE(m1->set(1,2,1));
	EXPECT_TRUE(m1->set(2,0,1));
	EXPECT_TRUE(m1->set(2,1,-1));
	EXPECT_TRUE(m1->set(2,2,1));

	c[0]=1;	
	c[1]=2;
	c[2]=1;

	EXPECT_ANY_THROW(result = m1->solveEquation(c));

	// Sustava s nekonecne vela rieseniami (Determinant = 0)
	EXPECT_TRUE(m1->set(0,0,1));
	EXPECT_TRUE(m1->set(0,1,0));
	EXPECT_TRUE(m1->set(0,2,-1));
	EXPECT_TRUE(m1->set(1,0,2));
	EXPECT_TRUE(m1->set(1,1,1));
	EXPECT_TRUE(m1->set(1,2,-5));
	EXPECT_TRUE(m1->set(2,0,0));
	EXPECT_TRUE(m1->set(2,1,-6));
	EXPECT_TRUE(m1->set(2,2,18));

	c[0]=-2;	
	c[1]=-8;
	c[2]=24;

	EXPECT_ANY_THROW(result = m1->solveEquation(c));
	delete m1;

	// Stvorcova matica (2 riadky)
	m1 = new Matrix(2,2);
	
	EXPECT_TRUE(m1->set(0,0,5));
	EXPECT_TRUE(m1->set(0,1,3));
	EXPECT_TRUE(m1->set(1,0,1));
	EXPECT_TRUE(m1->set(1,1,0));
	std::vector<double> e(2);
	e[0]=1;
	e[1]=2;


	// Eception thrown for not being square
	EXPECT_NO_THROW(result = m1->solveEquation(e));

	// Kontrola
	EXPECT_DOUBLE_EQ(result[0],2.0);
	EXPECT_DOUBLE_EQ(result[1],-3.0);
	delete m1;


	// Stvorcova matica (4 riadky)
	m1 = new Matrix(4,4);
	
	EXPECT_TRUE(m1->set(0,0,5));
	EXPECT_TRUE(m1->set(0,1,3));
	EXPECT_TRUE(m1->set(0,2,5));
	EXPECT_TRUE(m1->set(0,3,3));

	EXPECT_TRUE(m1->set(1,0,4));
	EXPECT_TRUE(m1->set(1,1,5));
	EXPECT_TRUE(m1->set(1,2,2));
	EXPECT_TRUE(m1->set(1,3,6));

	EXPECT_TRUE(m1->set(2,0,3));
	EXPECT_TRUE(m1->set(2,1,6));
	EXPECT_TRUE(m1->set(2,2,7));
	EXPECT_TRUE(m1->set(2,3,9));

	EXPECT_TRUE(m1->set(3,0,1));
	EXPECT_TRUE(m1->set(3,1,5));
	EXPECT_TRUE(m1->set(3,2,3));
	EXPECT_TRUE(m1->set(3,3,9));

	std::vector<double> f(4);
	f[0]=1;
	f[1]=2;
	f[2]=3;
	f[3]=4;


	// Eception thrown for not being square
	EXPECT_NO_THROW(result = m1->solveEquation(f));

	// Kontrola
	EXPECT_DOUBLE_EQ(result[0],0.40000000000000002);
	EXPECT_DOUBLE_EQ(result[1],-1.2);
	EXPECT_DOUBLE_EQ(result[2],-0.14999999999999999);
	EXPECT_DOUBLE_EQ(result[3],1.1166666666666667);

	delete m1;
	
}



/*** Konec souboru white_box_tests.cpp ***/
