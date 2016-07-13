#include "TTSEngine.h"
#include <iostream>
#include <stdio.h>

//#pragma setlocale("turkish")

void main(int argc, char **argv) {

	TTSEngine *tts = new TTSEngine();
	string line = "";
	char input[256];

	//setlocale( LC_ALL, "Turkish" );
	//cin.imbue(locale("turkish"));

	//ifstream input("data/input.txt",ios::in);

	//if(input == NULL)
	//	cout<<"error loading input text file"<<endl;
	while(line != "quit"){
		scanf("%s",input);
		line = input;
		tts->ReadText(line);		
	}

	delete tts;
}