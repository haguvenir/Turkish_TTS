
#include "TextStruct.h"

#define		ABBREVIATION	"veriler\\kisaltmalar.txt"
#define		EXCEPTIONS		"veriler\\istisnalar.txt"

Parser ::Parser(TTSQueue * queue, TTSTrie * trie)
{
	splitter = new Splitter();
	rules = new Rules();

	ruleEnabled = true;
	puncEnabled = true;

	abbr_exc	= new TTSHashTrie();
	this->queue	= queue;
	this->trie	= trie;
	LoadAbbreviations(abbr_exc);
	LoadExceptions(abbr_exc);
}

Parser::~Parser()
{
	delete splitter;
	delete rules;
	delete abbr_exc;
}

void Parser::LoadAbbreviations(TTSHashTrie * trie)
{
	ifstream abbrv(ABBREVIATION,ios::in);
	if(abbrv == NULL)
		cout<<"error loading abbreviations file"<<endl;
	string line;
	string data;
	int lineno=0;
	while(abbrv >> line)
	{
		lineno++;
		abbrv >> data;
		if(line.length() == 0 || data.length() == 0)
		{
			cout<<"Invalid exceptions file format at line "<<lineno<<endl;
		}
		trie->Insert(line,data);
	}
	cout<<"Loaded "<<lineno<<" abbreviation(s)"<<endl;
}

void Parser::LoadExceptions(TTSHashTrie * trie)
{
	ifstream except(EXCEPTIONS,ios::in);
	if(except == NULL)
		cout<<"error loading exceptions file"<<endl;
	string line;
	string data;
	int lineno=0;
	while(except >> line)
	{
		lineno++;
		except >> data;
		if(line.length() == 0 || data.length() == 0)
		{
			cout<<"Invalid exceptions file format at line "<<lineno<<endl;
		}
		trie->Insert(line,data);
	}
	cout<<"Loaded "<<lineno<<" exception(s)"<<endl;
}

void Parser::InsertWord(Word * thenew)
{
	if(thenew->type != INFO) thenew->FindTokens(splitter,trie);
	queue->Enqueue(thenew);
}

// rt silinmeli, islendikten sonra
Word * Parser::GetWord()
{
	return queue->Dequeue();
}

string Parser::PrintTagFree()
{
	int index = 0;
	int tendlast=0;
	int tstart;
	string str = content;
	string rt ="";
	while(index < (int)str.length())
	{
		if(str[index] == TAGSTART)
		{
			tstart = index;
			while(index < (int)str.length() && str[index] != TAGEND)
				index++;
			index++;
			rt += str.substr(tendlast,tstart-tendlast);
			rt += " ";
			tendlast = index;
		}
		else index++;
	}
	return rt;
}

void Parser::LoadString(string str)
{
	content = str;
}

void Parser::LoadFile(ifstream * file)
{
	string toparse;
	string str;
	if(file == NULL)
	{
		cout<<"File is not opened?, check address"<<endl;
		return;
	}

	toparse="";

	while( *file >> str )
	{
		toparse += str + " ";
	}

	toparse = toparse.substr(0,toparse.size()-1);
	content = toparse;
	file->close();
}

void Parser::Parse()
{
	string thecontent = content;
	int index=0, pos = 0, size = 0;
	int len = (int)thecontent.length();
	Word * tmpWord;
	isBusy = TRUE;
	CString sItem;
	double dNumber;
	char sNumber[256];


	while(index <len)
	{
		//
		//	KELIMELER
		if(GetTypeOf(thecontent[index]) == ALPHA)
		{
			int begin = index;
			index++;
			while(index<len && 
				GetTypeOf(thecontent[index]) == ALPHA) index++;

			tmpWord = new Word(thecontent.substr(begin,(index-begin)));
			tmpWord->type = CUMLE;

			if(rules->pronounce.size() !=0)
			{
				tmpWord->Split(rules->pronounce);
				rules->pronounce.erase();
				while(tmpWord != NULL)
				{
					tmpWord->Update(this);
					InsertWord(tmpWord);
					tmpWord = tmpWord->next;
				}
			}
			else if(rules->pronouncec.size() != 0)
			{
				//
				continue;
			}
			else
			{
				tmpWord->Update(this);
				InsertWord(tmpWord); 
			}
		}
		//
		//	SAYILAR
		else if(GetTypeOf(thecontent[index]) == NUMERIC)
		{
			int begin = index;

			if (thecontent[index] == '0') {
				tmpWord = new Word(thecontent.substr(index,1));
				tmpWord->type = SAYI;
				index++;			
			} else {

				/*
				//index++;
				pos = index;

				size = 0;
				memset(sNumber,0,256*sizeof(char));
				while(pos<len && ((GetTypeOf(thecontent[pos]) == NUMERIC) || 
					(thecontent[pos] == '.') ) ) 
				{
					if (thecontent[pos] == '.'){
						pos++;
						continue;
					}
					 sNumber[size++] = thecontent[pos];
					 pos++;

				}

				dNumber = atof(sNumber);
				if (dNumber != 0) {
					index = pos;
					sItem.Format("%f",dNumber);
					tmpWord = new Word(sItem.GetString());

				} else {
				
					pos = index;
					while(pos<len && GetTypeOf(thecontent[pos]) == NUMERIC)
						index++;
	
					tmpWord = new Word(thecontent.substr(begin,(index-begin)));
				}

				tmpWord->type = SAYI;

				if(rules->pronouncec.size() != 0)
				{
					// skip these word(s)
					continue;
				}

				int isdot = index;
				bool rankingnumber = FALSE;
				if(thecontent[isdot]=='.')
				{
					isdot++;
					while(thecontent[isdot]==' ') isdot++;
					if(thecontent[isdot] >'a' && thecontent[isdot] <'z')
					{
						if(DEBUG_0) cout<<"Ranking number"<<endl;
						rankingnumber = TRUE;
						tmpWord->type = DERECELISAYI;
						index++;
					}				
				}

				*/
				while(index<len && 
					GetTypeOf(thecontent[index]) == NUMERIC) index++;

				tmpWord = new Word(thecontent.substr(begin,(index-begin)));
				tmpWord->type = SAYI;

				if(rules->pronouncec.size() != 0)
				{
					// skip these word(s)
					continue;
				}

				int isdot = index;
				bool rankingnumber = FALSE;
				if(thecontent[isdot]=='.')
				{
					isdot++;
					while(thecontent[isdot]==' ') isdot++;
					if(thecontent[isdot] >'a' && thecontent[isdot] <'z')
					{
						if(DEBUG_0) cout<<"Ranking number"<<endl;
						rankingnumber = TRUE;
						tmpWord->type = DERECELISAYI;
						index++;
					}				
				}
			
			}
			tmpWord->Update(this);
			while(tmpWord != NULL)
			{
				InsertWord(tmpWord);
				tmpWord = tmpWord->next;
			}
		}
		//
		//	SEMBOLLER
		else if(GetTypeOf(thecontent[index]) == SYMBOLIC)
		{
			int begin = index;
			// control tags
			if(thecontent[begin] == ' ')
			{
				index++;
				continue;
			}

			if(ruleEnabled)
				if(thecontent[begin] == TAGSTART)
				{
					int newin = TagControl(thecontent,index);
					if(begin != newin) 
					{
						index = newin;
						continue;
					}
				}
				index++;
				// capture ...'s
				while(GetTypeOf(thecontent[index]) == SYMBOLIC &&
					thecontent[index] == '.' ) index++;

				if(rules->pronouncec.size() != 0)
				{
					// skip these word(s)
					continue;
				}

				tmpWord = new Word(thecontent.substr(begin,(index-begin)));
				tmpWord->type = SEMBOL;
				tmpWord->Update(this);

				if(tmpWord->GetType() == SEMBOL && puncEnabled)
				{
					while(tmpWord != NULL)
					{
						InsertWord(tmpWord);
						tmpWord = tmpWord->next;
					}
				}
		}
	} /* End of while ... */

	isBusy = FALSE;
}

int Parser::TagControl(string thecontent, int oldindex)
{
	int tage;
	for(tage = oldindex; 
		tage<oldindex+MAX_TAG_LEN && tage<(int)thecontent.length(); tage++)
		if(thecontent[tage] == TAGEND)
			break;

	if(tage == MAX_TAG_LEN || tage == thecontent.length())
	{
		if(DEBUG_0) cout<<"sonda bulamadi"<<endl;
		return oldindex;
	}

	string tolook = Tolower(thecontent.substr(oldindex+1,tage-1-oldindex));

	int index = 0;
	bool tagend = FALSE;
	// tAG Close!! therefore return tagend as new index
	if(thecontent[oldindex+1] == TAGCLOSE)
	{ 
		index++; 
		tagend = TRUE; 
	}

	int begin = index;	
	while(tolook[index] != NULL && GetTypeOf(tolook[index]) == ALPHA) index++;
	int end = index;
	string rulename = tolook.substr(begin,end-begin);

	int i;
	for(i=0; i<NO_OF_TAGS; i++)
		if(TAGS[i] == rulename)
			break;

	// If sable is not enabled, do not consider any rule
	if(i != 6 && !rules->sableOn) return oldindex;

	ruleUpdate * ruleupdate = (ruleUpdate *) malloc(sizeof(ruleUpdate));
	ruleupdate->valid = TRUE;

	if(tagend)
	{
		switch(i)
		{
		case 0:		rules->RuleEmphC(ruleupdate);		break;
		case 2: 	rules->RulePitchC(ruleupdate);		break;
		case 3: 	rules->RuleRateC(ruleupdate);		break;
		case 4: 	rules->RuleVolumeC(ruleupdate);		break;
		case 5: 	rules->RuleEngineC(ruleupdate);		break;
		case 6: 	rules->RuleSableC();				break;
		case 8: 	rules->RuleSayasC(ruleupdate);		break;
		case 9: 	rules->RuleLangC(ruleupdate);		break;
		case 10: 	rules->RuleSpeakerC(ruleupdate);	break;
		case 11: 	rules->RulePronClosableC();			break;
		case 12: 	rules->RuleAudioC(ruleupdate);		break;
		case 13: 	rules->RuleMarkerC(ruleupdate);		break;
		case 14: 	rules->RuleDivC(ruleupdate);		break;
		case 15:	puncEnabled = true;					break;
		default:
			if(DEBUG_0) cout<<"NOn recognized tag"<<endl;
			return oldindex;
		}
		switch(i)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 9:
		case 10:
		case 12:
		case 13:
			if(ruleupdate->valid)
			{
				Word * tmpWord = new Word(" ");
				tmpWord->type = INFO;
				tmpWord->rule = ruleupdate;
				InsertWord(tmpWord);
			}
			else
				if(DEBUG_0) cout<<"--* Non valid rule or not implemented..."<<endl;
			break;
		}
		// we are done with the tag
		return tage+1;
	}
	else
	{
		switch(i)
		{
		case 0:		rules->RuleEmph(ruleupdate,tolook);		break;
		case 1: 	rules->RuleBreak(ruleupdate,tolook);	break;
		case 2: 	rules->RulePitch(ruleupdate,tolook);	break;
		case 3: 	rules->RuleRate(ruleupdate,tolook);		break;
		case 4: 	rules->RuleVolume(ruleupdate,tolook);	break;
		case 5: 	rules->RuleEngine(ruleupdate,tolook);	break;
		case 6: 	rules->RuleSable();						break;
		case 7: 	rules->RulePron(ruleupdate,tolook);		break;
		case 8: 	rules->RuleSayas(ruleupdate,tolook);	break;
		case 9: 	rules->RuleLang(ruleupdate,tolook);		break;
		case 10: 	rules->RuleSpeaker(ruleupdate,tolook);	break;
		case 11: 
			{

				Word * tmpWord = rules->RulePronClosable(tolook);	
				while(tmpWord != NULL)
				{
					tmpWord->Update(this);
					InsertWord(tmpWord);
					tmpWord = tmpWord->next;
				}

				break;
			}
		case 12: 	rules->RuleAudio(ruleupdate,tolook);	break;
		case 13: 	rules->RuleMarker(ruleupdate,tolook);	break;
		case 14: 	rules->RuleDiv(ruleupdate,tolook);		break;
		case 15:	puncEnabled = false;					break;

		default:
			if(DEBUG_0) cout<<"Non recognized tag"<<endl;
			return oldindex;
		}
		switch(i)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 9:
		case 10:

		case 12:
		case 13:
		case 14:
			if(ruleupdate->valid)
			{
				Word * tmpWord = new Word(" ");
				tmpWord->type = INFO;
				tmpWord->rule = ruleupdate;
				InsertWord(tmpWord);
			}
			else
				if(DEBUG_0) cout<<"--* Non valid rule or not implemented..."<<endl;

			break;
		}
		return tage+1;
	}
}

void Parser::Display()
{
	queue->Display();
}

void Parser::DisplayPhonemes()
{
	queue->DisplayP();
}

void Parser::DisplaySyllables()
{
	queue->DisplayS();
}

TCHAR toLowerSpecial(TCHAR ch)
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

string Tolower(string str)
{
	int i=0;
	string tort;
	tort.resize(str.length());

	for(; i<(int)str.length(); i++)
		tort[i] = toLowerSpecial((TCHAR)str[i]);
	return tort;
}

void GetParameter(string find, string str, string * towrite)
{
	int index = 0;
	int len = (int)str.length();
	int lenfind = (int)find.length();
	int level = 0;
	int data_s, data_e;
	bool DONE = FALSE;
	find = Tolower(find);

	while(index < len)
	{
		switch (level)
		{
		case 0:
			if(str[index] == find[0])
			{
				int newin = index;
				int lenin=0;
				while(str[newin] == find[lenin] 
				&& lenin <lenfind)
				{ newin++; lenin++; }

				if(lenin == lenfind)
				{
					index = newin;
					level++;
					continue;
				}
			}
			index++;
			break;
		case 1:
			if(str[index] == '=')
				level++;
			index++;
			break;
		case 2:
			if(str[index] == '\"')
			{ level++; data_s = index+1; }
			index++;
			break;
		case 3:
			if(str[index] == '\"')
			{	data_e = index; DONE=TRUE; }
			index++; 
			break;
		}
		if(DONE) break;
	}
	if(DONE) 
		*towrite = str.substr(data_s,data_e-data_s);
}

// Supporting methods
CHAR_TYPE GetTypeOf(TCHAR thechar)
{
	string str = "aâbcçdefgðhýîijklmnoöprsþtuûüvyzwxq";
	string STR = "AÂBCÇDEFGÐHIÎÝJKLMNOÖPRSÞTUÛÜVYZWXQ";

	string num = "0123456789";

	int i;
	for(i=0; i<(int)str.length(); i++)
	{
		if(str[i] == thechar) return ALPHA;
		if(STR[i] == thechar) return ALPHA;
	}
	for(i=0; i<(int)num.length(); i++)
	{
		if(num[i] == thechar) return NUMERIC;
	}

	return SYMBOLIC;
}
