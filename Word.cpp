#include "TextStruct.h"
#include <string>
#include <iostream>

Word::Word(string thestr)
{
	content		= Tolower(thestr); 
	original	= thestr; 
	next		= NULL;
	back		= NULL;
	internalqueue = NULL;
	type		= NONE;
	tokencount_p	= 0;
	tokencount_s	= 0;
	tokens_phonemes = new token();
	tokens_syllables = new token();
}

void Word::Split(string toinsert)
{
	string full = toinsert;
	int len = (int)full.length();
	int index = 0;
	int begin = 0;
	
	cout<<toinsert<<endl;

	Word *last = this;
	Word *ptr = this;
	while(index < len)
	{
		while(index < len && GetTypeOf(full[index]) == ALPHA) index++;
		
		if(index != len && full[index] != ' ') 
		{ 
			cout<<"Non alphanumeric value or not space character"<<endl;
			this->next = NULL; 
			return; 
		}
		
		last->next = new Word(full.substr(begin,index-begin));
		last->next->type = CUMLE;
		last = last->next;
		index++;
		begin = index;
	}
	*this = *this->next;
}

void Word::Update(Parser * p)
{
	FormatNumber();
	FormatAbbrExc(p);
	FormatSymbol();
}

Word::~Word()
{
}

token * Word::HandleUnknownPhonem(Splitter * split, string tokenc, TTSTrie * trie)
{
	cout<<"HandleUnknown "<< tokenc<<endl;
	token * thetoken = new token;
	thetoken->next = NULL;
	int state = 1;
	
	for(int i=0; i<(int)tokenc.length(); i++)
	{
		if(split->IsVowel(tokenc[i]))
			state = 2 * state;
		else state = 2 * state + 1;
	}
	if (tokenc.length() == 6) {
		cout<<"6 uzunlugunda phonem yok"<<endl;
		cout<<"The state: "<<state<<endl;
		cout<<"Syllable: "<<tokenc<<endl;
		switch(state) {
			case 119:  // xxaxxx
				if (trie->exists(tokenc.substr(0,4))) {		
					thetoken->cont = tokenc.substr(0,4);
					thetoken->Append(tokenc.substr(4,1));
					thetoken->Append(tokenc.substr(5,1));
				}
		}
	}
	else if (tokenc.length() == 5) {
		cout<<"5 uzunlugunda phonem yok"<<endl;
		cout<<"The state: "<<state<<endl;
		cout<<"Syllable: "<<tokenc<<endl;
		switch(state) {
			case 55:  // xaxxx
				if (trie->exists(tokenc.substr(0,4))) {		
					thetoken->cont = tokenc.substr(0,4);
					thetoken->Append(tokenc.substr(4,1));
				}
				else if (trie->exists(tokenc.substr(0,3))) {
					thetoken->cont = tokenc.substr(0,3);
					thetoken->Append(tokenc.substr(3,1));
					thetoken->Append(tokenc.substr(4,1));
				}
				else {
					thetoken->cont = tokenc.substr(0,2);
					thetoken->Append(tokenc.substr(2,1));
					thetoken->Append(tokenc.substr(3,1));
					thetoken->Append(tokenc.substr(3,1));
				}
				break;
			case 59:  // xxaxx
				if (trie->exists(tokenc.substr(0,4))) {		
					thetoken->cont = tokenc.substr(0,4);
					thetoken->Append(tokenc.substr(4,1));
				}
				else if (trie->exists(tokenc.substr(0,3))) {
					if (trie->exists(tokenc.substr(2,3))) {	
						thetoken->cont = tokenc.substr(0,3);
						thetoken->Append(tokenc.substr(2,3));
					}
					else {
						thetoken->cont = tokenc.substr(0,3);
						thetoken->Append(tokenc.substr(2,2));
						thetoken->Append(tokenc.substr(4,1));
					}
				}
				else if (trie->exists(tokenc.substr(1,4))) {
					thetoken->cont = tokenc.substr(0,1);
					thetoken->Append(tokenc.substr(1,4));
				}
				else {
					thetoken->cont = tokenc.substr(0,1);
					thetoken->Append(tokenc.substr(1,2));
					thetoken->Append(tokenc.substr(2,2));
					thetoken->Append(tokenc.substr(4,1));
				}
				break;
			case 61:  // xxxox
				if (trie->exists(tokenc.substr(0,4))) {		
					thetoken->cont = tokenc.substr(0,4);
					thetoken->Append(tokenc.substr(4,1));
				}
				else if (trie->exists(tokenc.substr(1,4))) {		
					thetoken->cont = tokenc.substr(0,1);
					thetoken->Append(tokenc.substr(1,4));
				}
				break;

		} // switch
	}

	else if(tokenc.length() == 4) {
		cout<<"4 uzunlugunda phonem yok"<<endl;
		cout<<"The state: "<<state<<endl;
		cout<<"Syllable: "<<tokenc<<endl;

		switch(state) {
			case 23:  // axxx
				thetoken->cont = tokenc.substr(0,2);
				thetoken->Append(tokenc.substr(2,1));
				thetoken->Append(tokenc.substr(3,1));
				break;
			case 27: // xaxx
				if (trie->exists(tokenc.substr(0,3))) {
					thetoken->cont = tokenc.substr(0,3);
					thetoken->Append(tokenc.substr(3,1));
				} else if (trie->exists(tokenc.substr(1,3))) {
					thetoken->cont = tokenc.substr(0,1);
					thetoken->Append(tokenc.substr(1,3));
				}				
				else {
					thetoken->cont = tokenc.substr(0,2);
					thetoken->Append(tokenc.substr(2,1));
					thetoken->Append(tokenc.substr(3,1));
				}
				break;
			case 29: // xxax
				if (trie->exists(tokenc.substr(0,3))) {
					thetoken->cont = tokenc.substr(0,3);
					thetoken->Append(tokenc.substr(3,1));
				}
				else if (trie->exists(tokenc.substr(1,3))) {
					thetoken->cont = tokenc.substr(0,1);
					thetoken->Append(tokenc.substr(1,3));
				}
				else {
					thetoken->cont = tokenc.substr(0,1);
					thetoken->Append(tokenc.substr(1,2));
					thetoken->Append(tokenc.substr(3,1));
				}
				break;
			case 30: // xxxa
				thetoken->cont = tokenc.substr(0,1);
				thetoken->Append(tokenc.substr(1,1));
				thetoken->Append(tokenc.substr(2,2));
				break;
		}
	}
	else if(tokenc.length() == 3) {
		cout<<"3 uzunlugunda phonem yok"<<endl;
		cout<<"The state: "<<state<<endl;
		cout<<"Phonem: "<<tokenc<<endl;

		switch(state) {
			case 13: // xax
				thetoken->cont = tokenc.substr(0,2);
				thetoken->Append(tokenc.substr(2,1));
				break;
			case 14: // xxa
				thetoken->cont = tokenc.substr(0,1);
				thetoken->Append(tokenc.substr(1,2));
				break;
			case 11: // oxx
				thetoken->cont = tokenc.substr(0,2);
				thetoken->Append(tokenc.substr(2,1));
				break;
		}
	}
	else if (tokenc.length() == 2) {
		cout<<"2 uzunlukta phonem yok!!"<<endl;
		cout<<"The state: "<<state<<endl;
		cout<<"Phonem: "<<tokenc<<endl;
		thetoken->cont = tokenc.substr(0,1);
		thetoken->Append(tokenc.substr(1,1));
	}
	else if(tokenc.length() == 1) {
		cout<<"1 uzunlukta phonem yok!!!!"<<endl;
		cout<<"Yandik !!! "<<endl;
		thetoken = GetCharTokens(tokenc[0]);
	}
	return thetoken;
} // HandleUnknownPhonem

void Word::FindTokens(Splitter * split, TTSTrie * trie)
{
	if (SPLIT_BY_PHONEME == 1) {
		// PHONETIZATION
		content = split->SplitToPhonemes(content);
		string part;
		int len = (int)content.size();
		int count=0;
		int start=0;

		token *lasttoken = this->tokens_phonemes;
		lasttoken->next = NULL;
		cout<<content<<endl;
		while(count<len)
		{
			if(content[count] == DELIMITER_CHAR)
			{
				part = content.substr(start,(count-start));
				if(!trie->exists(part))
					lasttoken->Append(HandleUnknownPhonem(split,part,trie));
				else
					lasttoken->Append(part);
				lasttoken = lasttoken->Last();
				start = count+1;
			}
			count++;
		} //while

		part = content.substr(start,(count-start));
		if(!trie->exists(part))
			lasttoken->Append(HandleUnknownPhonem(split,part,trie));
		else
			lasttoken->Append(part);
		lasttoken = lasttoken->Last();

		// the first was dummy node, remove it from the list
		tokens_phonemes = tokens_phonemes->next;
		tokencount_p	= tokens_phonemes->Len()+1;

		this->tokens_syllables = NULL;

	} else {
		// SYLLABIFY
		content = split->Syllabify(content, trie);
		int len = (int)content.size();
		int count=0;
		int start=0;
		string part;

		token * lasttoken2 = this->tokens_syllables;
		lasttoken2->next = NULL;
cout << "content="<<content<<endl;
		while(count<len)
		{
			if(content[count] == DELIMITER_CHAR)
			{
				tokencount_s++;
				part = content.substr(start,(count-start));
				if(!trie->exists(part)) {
					lasttoken2->Append(HandleUnknownPhonem(split,part,trie));
				}
				else {
					lasttoken2->Append(part);
				}
				lasttoken2 = lasttoken2->Last();
				start = count+1;
			}
			count++;
		}
		tokencount_s++;
		part = content.substr(start,(count-start));
		if(!trie->exists(part)){
			lasttoken2->Append(HandleUnknownPhonem(split,part,trie));
		}
		else {
			lasttoken2->Append(part);
		}

		lasttoken2 = lasttoken2->Last();

		lasttoken2->next = NULL;
		// the first was dummy node, remove it from the list
		//token *dummy;
		//dummy = tokens_syllables;
		tokens_syllables = tokens_syllables->next;
		tokencount_s	= tokens_syllables->Len()+1;
	}
} // FindTokens

void Word::FormatSymbol()
{
	if(content == "!")
		content = "ünlem";
	else if(content == " ")
		content = "boþluk";
	else if(content == "+")
		content = "artý";
	else if(content == "-")
		content = "eksi";
	else if(content == "/")
		content = "bölü";
	else if(content == "\\")
	{
		content = "ters";
		this->next = new Word("bölü");
	}
	else if(content == "*")
		content = "yýldýz";
	else if(content == "=")
		content = "eþittir";
	else if(content == "%")
		content = "yüzde";
	else if(content == "_")
	{
		content = "alt";
		this->next = new Word("çizgi");
	}
	else if(content == "(")
	{
		content = "parantez";
		this->next = new Word("aç");
	}
	else if(content == "...")
	{
		content = "üç";
		this->next = new Word("nokta");
	}
	else if(content == ")")
	{
		content = "parantez";
		this->next = new Word("kapat");
	}
	else if(content == "?")
	{
		content = "soru";
		this->next = new Word("iþareti");
		this->next->next = NULL;
	}
	else if(content == "]")
	{
		content = "köþeli";
		this->next = new Word("parantez");
		this->next->next = new Word("kapat");
		this->next->next->next = NULL;
	}
	else if(content == "[")
	{
		content = "köþeli";
		this->next = new Word("parantez");
		this->next->next = new Word("aç");
		this->next->next->next = NULL;
	}
	else if(content == "<")
		content = "küçüktür";
	else if(content == ">")
		content = "büyüktür";
	else if(content == "{")
	{
		content = "küme";
		this->next = new Word("parantezi");
		this->next->next = new Word("aç");
	}
	else if(content == "}")
	{
		content = "küme";
		this->next = new Word("parantezi");
		this->next->next = new Word("kapat");
	}
	else if(content == ",")
		content = "virgül";
	else if(content == ".")
		content = "nokta";
	else if(content == ";")
	{	
		content = "noktalý";
		this->next = new Word("virgül");
	}
	else if(content == ":")
	{	
		content = "iki";
		this->next = new Word("nokta");
	}
	else if(content == "\'")
	{	
		content = "kesme";
		this->next = new Word("iþareti");
	}
	else if(content == "`")
	{	
		content = "tek";
		this->next = new Word("týrnak");
	}
	else if(content == "\"")
	{	
		content = "çift";
		this->next = new Word("týrnak");
	}
	else if(content == "@")
	{	
		content = "et";
	}
	else if(content == "$")
	{	
		content = "dolar";
	}
	else if(content == "#")
	{	
		content = "diyez";
	}
	else if(content == "^")
	{	
		content = "þapka";
	}
	else if(content == "&")
	{	
		content = "ve";
		this->next = new Word("iþareti");
	}
	else if(content == "~")
	{	
		content = "týrtýl";
	}
	else
	{
		type = NONE;
	}
}

void Word::FormatAbbrExc(Parser *parser)
{
	string newcont = parser->abbr_exc->Exists(content);
	if(newcont.length() != 0)
		content = newcont;
}

void Word::FormatPhone()
{
}

void Word::DisplayP()
{
	token *thetoken = this->tokens_phonemes;
	while(thetoken != NULL)
	{
		cout<<thetoken->cont<<" ";
		thetoken = thetoken->next;
	}
}

void Word::DisplayS()
{
	token *thetoken = this->tokens_syllables;
	while(thetoken != NULL)
	{
		cout<<thetoken->cont<<" ";
		thetoken = thetoken->next;
	}
}

void Word::Display()
{ 	
	if(type == INFO)
	{
		cout<<"\t\t\t\t\t=| Rule: "<<TAGS[rule->ruleNo]
			<<", data: "<<rule->data<<endl;
		return;
	}
	cout<<"------- o -------"<<endl;
	cout<<"("<<tokencount_p+1<<") Phonemes \t"<<endl;
	token *thetoken = this->tokens_phonemes;
	cout<<"    ";
	while(thetoken != NULL)
	{
		cout<<thetoken->cont<<" ";
		thetoken = thetoken->next;
	}
	cout<<endl;

	cout<<"("<<tokencount_s+1<<") Syllables \t"<<endl;
	token *thetoken2 = this->tokens_syllables;
	cout<<"    ";
	while(thetoken2 != NULL)
	{
		cout<<thetoken2->cont<<" ";
		thetoken2 = thetoken2->next;
	}
	cout<<endl;
}

////////////////////////////////////////////////////////////////////////
//////////////   SAYI HAKKINDA OLANLAR ... ////////////////////////////
////////////////////////////////////////////////////////////////////////
//
// depth is defined as (length-1)/3
//  ex: 100		--> 0
//		1233	--> 1
//		1234556 --> 2
// 
//	dirty is indication of validity of adding tags 
//	such as "yuz", "bin", "milyon"... to the number
//	ex: 10000000
//  if we have full zeros for a index, we should not add tag
//	otherwise this number should be read as; on milyon bin :(
void Word::FormatNumber()
{
	//setlocale(LC_ALL, "Turkish");

	if(type != SAYI && type!=DERECELISAYI ) return;

	/*
	CString sNumber;
	double dNumber = atof(content.c_str());
	sNumber.Format("%f",dNumber);	
	string number = sNumber.GetString();
	*/
	
	string number = content;
	int len = (int)content.size();
	int depth = (len-1) /3;
	int dirty=0;

	// special cases
	if(len == 1 && number[0]=='0')
	{
		if(type==SAYI)
			this->internalqueue = new Word("sýfýr");
		else
			this->internalqueue = new Word("sýfýrýncý");
		*this = *internalqueue;
		return;
	}

	Word *last = this->internalqueue = new Word("");
	// we will have an internal structure that 
	//	will be the case where we read numbers...
	while(depth >= 0)
	{
		dirty=0;
		// yuzler basamagi
		if(len >= (depth+1)*3){
			if(number[len-((depth+1)*3)] != '0'){
				if(number[len-((depth+1)*3)] != '1'){
					last->next = new Word(Ones(number[len-((depth+1)*3)]));
					last->next->back = last;
					last = last->next;
				}
				last->next = new Word("yüz");
				last = last->next;
				dirty=1;
			}
		}

		// onlar basamagi
		if(len >= (depth+1)*3-1)
			if(number[len-((depth+1)*3-1)] != '0'){
				last->next = new Word(Tens(number[len-((depth+1)*3-1)]));
				last = last->next;
				dirty=1;
			}

		// birler basamagi
		if(number[len-((depth+1)*3-2)] != '0')
		{
			// if depth is 1 meaning we are in "bin" part
			// we need to read 1001 as "bin bir" not "bir bin bir"
			// this if condition checks that conditions...
			if(depth == 1){
				if(dirty == 0 && number[len-((depth+1)*3-2)] != '1'){
					last->next = new Word(Ones(number[len-((depth+1)*3-2)]));
					last = last->next;
				}else
				if(dirty == 1){
					last->next = new Word(Ones(number[len-((depth+1)*3-2)]));
					last = last->next;
				}
			} else {
				last->next = new Word(Ones(number[len-((depth+1)*3-2)]));
				last->next->original = number[len-((depth+1)*3-2)];
				last = last->next;
			}
			dirty=1;
		}

		if(dirty)
		switch(depth){
			case 0: break;
			case 1: last->next = new Word("bin"); 
					last = last->next;
				break;
			case 2: last->next = new Word("milyon"); 
					last = last->next;
				break;
			case 3: last->next = new Word("milyar");
					last = last->next;
				break;
			case 4: last->next = new Word("trilyon"); 
					last = last->next;
				break;
			case 5: last->next = new Word("katrilyon"); 
					last = last->next;
				break;
			case 6: last->next = new Word("kentrilyon"); 
					last = last->next;
				break;
		}
		depth--;
	}

	if(type == DERECELISAYI)
	{
		if(last->original[0] == '4'){
			last->content = "dord";
		}
		last->content = last->content + Sayiderece(last->original[0]);
	}
	
	internalqueue = internalqueue->next;
	*this = *internalqueue;
}

string Ones(TCHAR number){
	switch(number){
		case '1': return "bir";
		case '2': return "iki";
		case '3': return "üç";
		case '4': return "dört";
		case '5': return "beþ";
		case '6': return "altý";
		case '7': return "yedi";
		case '8': return "sekiz";
		case '9': return "dokuz";
		default: cout<<"unknown number..."<<endl;
	}
	return "";
}

string Tens(TCHAR number){
	switch(number){
		case '1': return "on";
		case '2': return "yirmi";
		case '3': return "otuz";
		case '4': return "kýrk";
		case '5': return "elli";
		case '6': return "atmýþ";
		case '7': return "yetmiþ";
		case '8': return "seksen";
		case '9': return "doksan";
		default: cout<<"unknown number..."<<endl;
	}
	return "";
}

string Sayiderece(TCHAR c){
	switch(c){
		case '1': 
		case '5': 
		case '8': return "inci";
		case '6': return "ncý";
		case '2': 
		case '7': return "nci";
		case '3':
		case '4': return "üncü";
		case '9': return "uncu";
	}
	return "";
}

TCHAR toLowerCase(TCHAR ch)
{
	switch(ch)
	{
	case 'Â': return 'â';
	case 'Ð': return 'ð';
	case 'Ü': return 'ü';
	case 'Û': return 'û';
	case 'Þ': return 'þ';
	case 'I': return 'ý';
	case 'Ý': return 'i';
	case 'Î': return 'î';
	case 'Ö': return 'ö';
	case 'Ç': return 'ç';
	}
	return tolower(ch);
}


Word * GetChar(TCHAR ch)
{
	Word *rt = new Word("");

	char input[2];

	TCHAR chr = toLowerCase(ch);
	rt->SetNext(NULL);
	switch(chr)
	{
		case 'a': rt->SetContent("a"); break;
		case 'b': rt->SetContent("be"); break;
		case 'c': rt->SetContent("ce"); break;
		case 'ç': rt->SetContent("çe"); break;
		case 'd': rt->SetContent("de"); break;
		case 'e': rt->SetContent("e"); break;
		case 'f': rt->SetContent("fe"); break;
		case 'g': rt->SetContent("ge"); break;
		case 'ð': 
			rt->SetContent("yumuþak"); 
			rt->SetNext(new Word("ge"));
			break;
		case 'h': rt->SetContent("he"); break;
		case 'ý': rt->SetContent("ý"); break;
		case 'i': rt->SetContent("i"); break;
		case 'j': rt->SetContent("je"); break;
		case 'k': rt->SetContent("ka"); break;
		case 'l': rt->SetContent("le"); break;
		case 'm': rt->SetContent("me"); break;
		case 'n': rt->SetContent("ne"); break;
		case 'o': rt->SetContent("o"); break;
		case 'ö': rt->SetContent("ö"); break;
		case 'p': rt->SetContent("pe"); break;
		case 'r': rt->SetContent("re"); break;
		case 's': rt->SetContent("se"); break;
		case 'þ': rt->SetContent("þe"); break;
		case 't': rt->SetContent("te"); break;
		case 'u': rt->SetContent("u"); break;
		case 'ü': rt->SetContent("ü"); break;
		case 'v': rt->SetContent("ve"); break;
		case 'y': rt->SetContent("ye"); break;
		case 'z': rt->SetContent("ze"); break;
		// ingilizce karakterler
		case 'w': 
			rt->SetContent("dublüve"); 
			break; 
		case 'x': rt->SetContent("iks"); break;
		case 'q': rt->SetContent("kû"); break;
		default: {
					input[0] = ch;
					input[1] = '\0';
					rt->SetContent(input);
					if (isdigit(ch))
					{
						rt->SetType(SAYI);
						rt->FormatNumber();
					} 
					else
					{
						rt->FormatSymbol();
					}
				 }
	}
	return rt;
}

token * GetCharTokens(TCHAR ch)
{
	token *rt = new token;
	rt->next = NULL;

	TCHAR chr = toLowerCase(ch);

	switch(chr)
	{
		case 'a': rt->SetContent("a"); break;
		case 'b': rt->SetContent("b"); break;
		case 'c': rt->SetContent("c"); break;
		case 'ç': rt->SetContent("ç"); break;
		case 'd': rt->SetContent("d"); break;
		case 'e': rt->SetContent("e"); break;
		case 'f': rt->SetContent("f"); break;
		case 'g': rt->SetContent("g"); break;
		case 'ð': rt->SetContent("ð"); break;
		case 'h': rt->SetContent("h"); break;
		case 'ý': rt->SetContent("ý"); break;
		case 'i': rt->SetContent("i"); break;
		case 'j': rt->SetContent("j"); break;
		case 'k': rt->SetContent("k"); break;
		case 'l': rt->SetContent("l"); break;
		case 'm': rt->SetContent("m"); break;
		case 'n': rt->SetContent("n"); break;
		case 'o': rt->SetContent("o"); break;
		case 'ö': rt->SetContent("ö"); break;
		case 'p': rt->SetContent("p"); break;
		case 'r': rt->SetContent("r"); break;
		case 's': rt->SetContent("s"); break;
		case 'þ': rt->SetContent("þ"); break;
		case 't': rt->SetContent("t"); break;
		case 'u': rt->SetContent("u"); break;
		case 'ü': rt->SetContent("ü"); break;
		case 'v': rt->SetContent("v"); break;
		case 'y': rt->SetContent("y"); break;
		case 'z': rt->SetContent("z"); break;
		// ingilizce karakterler
		case 'w': rt->SetContent("dabýlyu"); break; 
		case 'x': rt->SetContent("iks"); break;
		case 'q': rt->SetContent("küu"); break;
	}
	return rt;
}

token * GetCharTokensToRead(TCHAR ch)
{
	token *rt = new token;
	rt->next = NULL;

	TCHAR chr = toLowerCase(ch);

	switch(chr)
	{
		case 'a': rt->SetContent("a"); break;
		case 'b': rt->SetContent("be"); break;
		case 'c': rt->SetContent("ce"); break;
		case 'ç': rt->SetContent("çe"); break;
		case 'd': rt->SetContent("de"); break;
		case 'e': rt->SetContent("e"); break;
		case 'f': rt->SetContent("fe"); break;
		case 'g': rt->SetContent("ge"); break;
		case 'ð': // yumuþakge
			rt->SetContent("yu");
			rt->Append("umu");
			rt->Append("uþa");
			rt->Append("ak");
			rt->Append("ge");
			break;
		case 'h': rt->SetContent("he"); break;
		case 'ý': rt->SetContent("ý"); break;
		case 'i': rt->SetContent("i"); break;
		case 'j': rt->SetContent("je"); break;
		case 'k': rt->SetContent("ke"); break;
		case 'l': rt->SetContent("le"); break;
		case 'm': rt->SetContent("me"); break;
		case 'n': rt->SetContent("ne"); break;
		case 'o': rt->SetContent("o"); break;
		case 'ö': rt->SetContent("ö"); break;
		case 'p': rt->SetContent("pe"); break;
		case 'r': rt->SetContent("re"); break;
		case 's': rt->SetContent("se"); break;
		case 'þ': rt->SetContent("þe"); break;
		case 't': rt->SetContent("te"); break;
		case 'u': rt->SetContent("u"); break;
		case 'ü': rt->SetContent("ü"); break;
		case 'v': rt->SetContent("ve"); break;
		case 'y': rt->SetContent("ye"); break;
		case 'z': rt->SetContent("ze"); break;
		// ingilizce karakterler
		case 'w': // dublüve
			rt->SetContent("da");
			rt->Append("abu");
			rt->Append("ul");
			rt->Append("yu");
			break; 
		case 'x': // iks
			rt->SetContent("iks");
			break;
		case 'q': // kuü
			rt->SetContent("kü");
			rt->Append("u");
			break;
	}
	return rt;
}