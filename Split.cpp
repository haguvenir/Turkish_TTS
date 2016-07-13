
#include "TextStruct.h"

Splitter::Splitter()
{
}

Splitter::~Splitter()
{
}

bool Splitter::IsVowel(TCHAR ch)
{
	if( ch == 'a' || ch == 'A' ||
		ch == 'â' || ch == 'Â' ||
	    ch == 'e' || ch == 'E' ||
		ch == 'ý' || ch == 'I' ||
		ch == 'î' || ch == 'Î' ||
		ch == 'i' || ch == 'Ý' ||
		ch == 'o' || ch == 'O' ||
		ch == 'ö' || ch == 'Ö' ||
		ch == 'u' || ch == 'U' ||
		ch == 'û' || ch == 'Û' ||
		ch == 'ü' || ch == 'Ü'
	   )
		return true;

	return false;
}

bool Splitter::HasVowel(string str)
{
	for(int i=0; i<(int)str.size(); i++)
	{
		if(IsVowel(str.at(i)))
		   return true;
	}
	return false;
}



bool Splitter::IsConsonant(TCHAR ch)
{
	return !IsVowel(ch);
}

int Splitter::CountVowels(string word)
{
	int vowels = 0;
	for(int i=0; i<(int)word.length(); i++)
		if(IsVowel(word[i]))
			vowels++;

	return vowels;
}

string Splitter::SplitToPhonemes(string word)
{
	string buffer = "";
	int currentCharIndex = 0;
	int wordlength = word.length();
	for(;currentCharIndex<wordlength; currentCharIndex ++)
	{
		if(DEBUG_0) cout<<"CurrentChar = "<<word[currentCharIndex]<<"\n";
		if(IsConsonant(word[currentCharIndex ]))
		{
			if(DEBUG_0) cout<<word[currentCharIndex]<<" is consonant\n";
			buffer += word[currentCharIndex];
			if(DEBUG_0) cout<<"buffer = "<<buffer<<"\n";
		}
		else
		{
			if(DEBUG_0) cout<<word[currentCharIndex]<<" is vowel\n";
			
			if(currentCharIndex==0 || currentCharIndex==wordlength-1)
				buffer += word[currentCharIndex];
			else
			{
				if(IsConsonant(buffer[buffer.size()-1]))
				{
					buffer += word[currentCharIndex];
					buffer += "-";
					buffer += word[currentCharIndex];
				}
				else
				{
					buffer[buffer.size() - 1] = word[currentCharIndex]; 
				}
			}
			if(DEBUG_0) cout<<"buffer = "<<buffer<<"\n";
		}
	}
		
	//return buffer;
	return SplitSyllable(buffer);
}

string Splitter::SplitSyllable(string word)
{
	int partialLength = 0;
	string result = "";
	for(int i=0; i<(int)word.length();)
	{
		partialLength = 0;
		while(i<(int)word.length() && word[i] != DELIMITER_CHAR)
		{
			i++;
			partialLength++;
		}
		// now we know the length of a token in the word.
		string token = word.substr(i-partialLength, partialLength);
		if(DEBUG_0) cout<<"token = "<<token<<"\n";

		if(partialLength < 4)
		{
			if(DEBUG_0) cout<<"partialLength < 4\n";
			//if(DEBUG_0) cout<<"token = "<<token<<"\n";
			result += token;
			if(i != word.length())
				result += DELIMITER_STRING;
			if(DEBUG_0) cout<<"currentResult = "<<result<<"\n\n";
		}
		else if(partialLength == 4)
		{
			if(DEBUG_0) cout<<"partialLength = 4\n";
			//if(DEBUG_0) cout<<"token = "<<token<<"\n";
			result += token.substr(0,2);
			result += DELIMITER_STRING;
			result += token.substr(2,2);
			if(i != word.length())
				result += DELIMITER_STRING;
			if(DEBUG_0) cout<<"currentResult = "<<result<<"\n\n";
		}
		else if(partialLength == 5)
		{
			if(DEBUG_0) cout<<"partialLength = 5\n";
			//if(DEBUG_0) cout<<"token = "<<token<<"\n";
			result += token.substr(0,3);
			result += DELIMITER_STRING;
			result += token.substr(3,2);
			if(i != word.length())
				result += DELIMITER_STRING;
			if(DEBUG_0) cout<<"currentResult = "<<result<<"\n\n";
		}
		i++;
	}
	return result;
}

vector<string> Splitter::PhonemeVector(string str)
{
	TCHAR* tokenPtr = strtok(&(str.at(0)), "-");
	vector< string > stringVector;

	while(tokenPtr != NULL)
	{
		stringVector.push_back(tokenPtr);
		tokenPtr = strtok(NULL, "-");
	}
	
	return stringVector;
}

string Splitter::Syllabify(string str, TTSTrie * trie) {
	
	int j = 0;
	string format = "";
	string state = "";
	string syllables = "";
	int str_length = (int)str.length();

	char c;
	for (int i = 0; i < str_length; i++){
		c = str[i];
		if (IsVowel(c))
			format += 'O';
		else 
			format += 'X';
	}

	//if(DEBUG_0) cout <<endl<<"Dizilis Sekli: " << format << endl;
	cout <<endl<<"Format: " << format << endl;
	
	// word exist, read it directly //
	if (str_length > 1 && str_length < 5 && trie->exists(str)) {
		syllables += ("-");
		syllables += str;
		if(DEBUG_0) cout << "Syllables: " << syllables.substr(1) << endl;
		return syllables.substr(1);
	}

	//////////////////////////////////////////////
	//////////////////////////////////////////////
	/////FINITE STATE MACHINE/////////////////////
	//////////////////////////////////////////////
	//////////////////////////////////////////////

	state = "NULL";

	while (j <= str_length) {
		if (state == "NULL") {
			if (format[j] == 'O') {
				state = "o";
				j++;
			}
			else { // format[j] == 'X'
				state = "x";
				j++;
			}
		} // state == "NULL"

		else if (state == "o") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-1]);
				state = "o";
				j++;
			}
			else { // format[j] == 'X'
				state = "ox";
				j++;
			}
		} // state == "o"		
		
		else if (state == "x") {
			if (j == str_length) {
				state = "HATA";
			} // son
			else if (format[j] == 'O') {
				state = "xo";
				j++;
			}
			else { // format[j] == 'X'
				state = "xx";
				j++;
			}
		} // state == "x"

		else if (state == "ox") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-2]);
				state = "xo";
				j++;
			}
			else { // format[j] == 'X'
				state = "oxx";
				j++;
			}
		} // state == "ox"
		else if (state == "xo") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				state = "o";
				j++;
			}
			else { // format[j] == 'X'
				state = "xox";
				j++;
			}
		} // state == "xo"
		else if (state == "xx") {
			if (j == str_length) {
				state = "HATA";
			} // son
			else if (format[j] == 'O') {
				state = "xxo";
				j++;
			}
			else { // format[j] == 'X'
				state = "xxx";
				j++;
			}
		} // state == "xx"
		else if (state == "oxx") {
				if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				state = "xo";
				j++;
			}
			else { // format[j] == 'X'
				state = "oxxx";
				j++;
			}
		} // state == "oxx"
		else if (state == "xox") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				state = "xo";
				j++;
			}
			else { // format[j] == 'X'
				state = "xoxx";
				j++;
			}
		} // state == "xox"
		else if (state == "xxo") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				state = "o";
				j++;
			}
			else { // format[j] == 'X'
				state = "xxox";
				j++;
			}
		} // state == "xxo"
		else if (state == "xxx") {
			if (j == str_length) {
				state = "HATA";
			} // son
			else if (format[j] == 'O') {
				state = "xxxo";
				j++;
			}
			else { // format[j] == 'X'
				state = "HATA";
			}
		} // state == "xxo"
		else if (state == "oxxx") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				state = "xo";
				j++;
			}
			else { // format[j] == 'X'
				state = "oxxxx";
				j++;
			}
		} // state == "oxxx"
		else if (state == "xoxx") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				state = "xo";
				j++;
			}
			else { // format[j] == 'X'
				state = "xoxxx";
				j++;
			}
		} // state == "xoxx"
		else if (state == "xxox") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				state = "xo";
				j++;
			}
			else { // format[j] == 'X'
				state = "xxoxx";
				j++;
			}
		} // state == "xxox"
		else if (state == "xxxo") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				state = "o";
				j++;
			}
			else { // format[j] == 'X'
				state = "xxxox";
				j++;
			}
		} // state == "xxxo"
		else if (state == "oxxxx") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-5]);
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-5]);
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				state = "xxo";
				j++;
			}
			else { // format[j] == 'X'
				state = "HATA";
			}
		} // state == "oxxxx"
		else if (state == "xoxxx") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-5]);
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-5]);
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				state = "xo";
				j++;
			}
			else { // format[j] == 'X'
				syllables += ("-");
				syllables += (str[j-5]);
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				state = "xx";
				j++;
			}
		} // state == "xoxxx"
		else if (state == "xxoxx") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-5]);
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-5]);
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				state = "xo";
				j++;
			}
			else { // format[j] == 'X'
				state = "xxoxxx";
				j++;
			}
		} // state == "xxoxx"
		else if (state == "xxxox") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-5]);
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-5]);
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				state = "xo";
				j++;
			}
			else { // format[j] == 'X'
				syllables += ("-");
				syllables += (str[j-5]);
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				state = "x";
				j++;
			}
		} // state == "xxxox"
		else if (state == "xxoxxx") {
			if (j == str_length) {
				syllables += ("-");
				syllables += (str[j-6]);
				syllables += (str[j-5]);
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				syllables += (str[j-1]);
				break;
			} // son
			else if (format[j] == 'O') {
				syllables += ("-");
				syllables += (str[j-6]);
				syllables += (str[j-5]);
				syllables += (str[j-4]);
				syllables += (str[j-3]);
				syllables += (str[j-2]);
				state = "xo";
				j++;
			}
			else { // format[j] == 'X'
				state = "HATA";
			}
		} // state == "xxoxxx"

		else { // state == "HATA"
			if(DEBUG_0) cout << "HATA" << endl;
			syllables = "";
			for (j=0; j < str_length; j++) {
				syllables += ("-");
				if (format[j] == 'O') 
					syllables += (str[j]);
				else { // format[j] == 'X'
					if (str[j] == 'w')
						syllables += ("dub-lü-ve");
					else if (str[j] == 'q')
						syllables += ("kû");
					else if (str[j] == 'x')
						syllables += ("iks");
					else if (str[j] == 'k')
						syllables += ("ka");
					else {
						syllables += (str[j]);
						syllables += ("e");
					}
				} // format[j] == 'X'
			} // for each character
			break;
		} // state == "HATA"
	} // while 
	//////////////////////////////////////////////
	if(DEBUG_0) cout << "Syllables: " << syllables.substr(1) << endl;
	return syllables.substr(1);
} // Syllabify
