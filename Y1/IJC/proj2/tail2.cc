// tail2.cc 
// Řešení IJC-DU2, příklad 1) b), 16.4.2016 
// Autor: Martin Bažík, FIT 
// Přeloženo: g++ 4.9.2
// Popis: Imituje posixovy program tail

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <stack>
#include <regex>		//kontrola cisel


using namespace std;


int main(int argc, char *argv[]){
	
	//Pre zrychlenie programu
	ios::sync_with_stdio(false);
	
	string s_cmp="-n";
	string filename="\0";

	//Plati pre 10 riadkov
	int lines = 11;
	int number_lines=0;
	int flag;

	//pomocne retazece
	string tmp;
	string x;


    ifstream fp;    


    //Rozpoznava argumenty, s ktorymi bol spusteny. Chybu zobrazuje na stderr.
    if(argc == 1){
    	flag=1;
    }

	else if(argc==2){
		filename=argv[1];
		flag=0;
		fp.open( filename );
		if (fp==NULL){
			cerr << "Subor " << filename << " nebol najdeny!" << endl;
			return 1;
		}
	}
	else if(argc==3){
		if(argv[1]==s_cmp){
			//Kontroluje spravnost vstupu na zaklade regex.
			if(!regex_match(argv[2], regex("[0-9]*"))){
				cerr << "Nespravny argument poctu!" << endl;
				return 1;
			}
			lines=stoi(argv[2]);
			lines++;
			flag=1;
		}
		else{
			cerr << "Chybne argumenty!" << endl;
			return 1;
		}
	}

	else if(argc==4){
		if(argv[1]==s_cmp){
			//Kontroluje spravnost vstupu na zaklade regex.
			if(!regex_match(argv[2], regex("[0-9]*"))){
				cerr << "Nespravny argument cisla!" << endl;
				return 1;
			}
			filename=argv[3];
			lines=stoi(argv[2]);
			lines++;
			flag=0;
			fp.open( filename );
			if (fp==NULL){
				cerr << "Subor " << filename << " nebol najdeny!" << endl;
				return 1;
			}
		}
		else{
			cerr << "Chybne argumenty!" << endl;
			return 1;
		}
	}

	else{
		cerr << "Nespravny pocet argumentov!" << endl;
		return 1;
	}

	//deklaracia zasobnikov
	stack <string> buffer;
	stack <string> buffer2;

	//Nacitava standardny vstup
	if(flag==1){
		 while( getline( cin, tmp)){
			buffer.push(tmp);
			number_lines++;	
		}
		number_lines++;		
	}

	//Nacitava vstup zo suboru
	if(flag==0){
		 while( getline( fp, tmp)){
			buffer.push(tmp);
			number_lines++;	
		}
		number_lines++;
		fp.close();		
	}


	if(lines>number_lines)
		lines=number_lines;

	//Nacita dany pocet riadkov do druheho zasobnika
	for(int i = lines-1;i>0;i--){
		buffer2.push(buffer.top ());
		buffer.pop();
	}

	//Riadky z druheho zasobnika idu na vystup
	for(int i = lines-1;i>0;i--){
		cout << buffer2.top () <<endl;
		buffer2.pop();
	}

	return 0;
}