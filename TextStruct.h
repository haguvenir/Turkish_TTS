#ifndef TEXT_STRUCT
#define TEXT_STRUCT

#include "TextConstants.h"
#include "TTSTrie.h"
#include <atlstr.h>

class Word;
class Parser;
class Splitter;
class Rules;
class RuleStack;
class TTSHashTrie;
class TTSQueue;

struct token;
struct ruleUpdate;
struct TrieNode;

struct token 
{
	struct token * next;
	string cont;
	void SetContent(string str){ cont = str; }
	string GetCont() { return cont; }
	void Append(token *tk)
	{
		Last()->next		= tk;
		tk->Last()->next	= NULL;
	}
	void Append(string str)
	{
		token *ptr = new token;
		ptr->cont = str;
		ptr->next = NULL;
		Append(ptr);
	}
	token * Last()
	{
		token *ptr = this;
		while(ptr->next != NULL)
			ptr = ptr->next;
		return ptr;
	}
	int Len() 
	{
		int len = 0;
		token *ptr = this;
		while(ptr->next != NULL)
		{
			len++;
			ptr = ptr->next;
		}
		return len;
	}

	void display()
	{
		token *ptr = this;
		while(ptr != NULL)
		{
			cout<<ptr->cont<<" ";
			ptr = ptr->next;
		}
	}
};

struct ruleUpdate
{
	int ruleNo;
	int data;
	int valid;
	void displayMe() 
	{ 
		if(DEBUG_0) cout<<endl;
		if(DEBUG_0) cout<<"Rule no :"<<ruleNo<<endl;
		if(DEBUG_0) cout<<"Data    :"<<data<<endl;
		if(DEBUG_0) cout<<endl;
	}
};

struct TTSHashTrieNode
{
	TRIE_NODE_TYPE type;
	string		compare;
	string		* info;
	string		* data;
	TTSHashTrieNode	* left;
	TTSHashTrieNode	* right;
	int			elements;
	int			index;
};

class RuleStack
{
	public:
		RuleStack(int size,int);
		~RuleStack();
		void Push(int);
		int Pop();
		const int TopE();
		void Resize(int nsize);
		void Display();
		int GetNoElem(){ return top+1; }
		int GetRuleno(){ return ruleno; }
		void MakeEmpty();
	private:
		int size;
		int top;
		int ruleno;
		int * stackPtr;

		bool isEmpty() const {return top == -1; }
		bool isFull()  const {return top == size-1; }
};

class TTSQueue 
{
	public:
		TTSQueue();
		~TTSQueue();
		void EmptyQueue();
		void Enqueue(Word *);
		Word * Dequeue();
		void Display();
		void DisplayP();
		void DisplayS();
		int QueueSize(){ return size; }
		bool    isBusy;
	private:
		bool finished;
		int size;
		Word * header;
		Word * last;
		HANDLE mutex;
		HANDLE full;
		HANDLE empty;
};

class Word
{
	public:
		Word(string);
		~Word();

		WORD_TYPE GetType() { return type; }
		ruleUpdate * GetRule() { return rule; }

		void SetType(WORD_TYPE newtype){ type = newtype; }
		// call for updates {NUMBER, ABBREVIATION, ...}
		void Update(Parser*);
		
		void Display();
		void DisplayP();	// display phonemes
		void DisplayS();	// display syllables
		// formatting part
		void FormatNumber();
		void FormatAbbrExc(Parser*);
		void FormatPhone();
		void FormatSymbol();
		void FindTokens(Splitter *, TTSTrie *);
		token * HandleUnknownPhonem(Splitter *,string str, TTSTrie * trie);
		void Split(string);
		void SetContent(string cont){ content = cont; }
		string GetContent()		{ return content; }
		void SetNext(Word *nxt)	{ next = nxt; }
		Word * GetNext()		{ return next; }
		int  GetPCount()		{ return tokencount_p; }
		token * GetPHeader()	{ return tokens_phonemes; }
		
		int  GetSCount()		{ return tokencount_s; }
		token * GetSHeader()	{ return tokens_syllables; }

	private:
		WORD_TYPE type;
		string content;
		string original;
		token *tokens_phonemes;
		token *tokens_syllables;
		ruleUpdate *rule;
		int tokencount_p;
		int tokencount_s;

		Word *next;
		Word *back;
		Word *internalqueue;
		
		friend class Parser;
		friend class TTSQueue;
};

class Rules 
{
public:
	Rules();
	~Rules();

	void RuleEmph(ruleUpdate*,string);
	void RuleBreak(ruleUpdate*,string);
	void RulePitch(ruleUpdate*,string);
	void RuleRate(ruleUpdate*,string);
	void RuleVolume(ruleUpdate*,string);
	void RuleEngine(ruleUpdate*,string);
	void RuleSable();
	void RulePron(ruleUpdate*,string);
	void RuleSayas(ruleUpdate*,string);
	void RuleLang(ruleUpdate*,string);
	void RuleSpeaker(ruleUpdate*,string);
	void RuleAudio(ruleUpdate*,string);
	void RuleMarker(ruleUpdate*,string);
	void RuleDiv(ruleUpdate*,string);
	Word* RulePronClosable(string);
	// -------------------------------------------------------
	// we will not have close tags of
	// break and pron tags, since break will be similar to set command,
	// closing break do not mean anything
	// similarly, pron will cover the first string after the tag !, no need to
	// look for a close tag.
	// -------------------------------------------------------
	void RuleEmphC(ruleUpdate*);
	void RulePitchC(ruleUpdate*);
	void RuleRateC(ruleUpdate*);
	void RuleVolumeC(ruleUpdate*);
	void RuleEngineC(ruleUpdate*);
	void RuleSableC();
	void RuleSayasC(ruleUpdate*);
	void RuleLangC(ruleUpdate*);
	void RuleSpeakerC(ruleUpdate*);
	void RuleAudioC(ruleUpdate*);
	void RuleMarkerC(ruleUpdate*);
	void RuleDivC(ruleUpdate*);
	void RulePronClosableC();

	void defaults();
	string pronounce;
	string pronouncec;
private:
	int sableOn;

	RuleStack 
		* emphs, 
		* pitchs,
		* rates, 
		* volumes,
		* engines, 
		* sayass,
		* langs, 
		* speakers,
		* audios, 
		* markers,
		* divs;
	friend class Parser;
	friend class Word;
};

class Parser 
{
	public:
		Parser(TTSQueue *, TTSTrie *);
		~Parser();
		void Parse();
		void LoadString(string);
		void LoadFile(ifstream*);
		void InsertWord(Word *);
		Word * GetWord();
		void SetPuncRead(bool flag){ puncEnabled = flag; }
		bool GetPuncRead(){ return puncEnabled; }
		void EnableRules()  {ruleEnabled = true; }
		void DisableRules() {ruleEnabled = false; }
		void Display();
		void DisplaySyllables();
		void DisplayPhonemes();
		int	 IsBusy(){ return isBusy; }
		int TagControl(string , int );
		int GetQueueSize() { return queue->QueueSize(); }
		Splitter * GetSplitter(){ return splitter; }
		string PrintTagFree();
	private:
		void LoadExceptions(TTSHashTrie *);
		void LoadAbbreviations(TTSHashTrie *);
		int			isBusy;
		TTSQueue	*queue;
		TTSTrie		*trie;
		string		content;
		Splitter	*splitter;
		Rules		* rules;
		TTSHashTrie	* abbr_exc;
		bool ruleEnabled;
		bool puncEnabled;
		friend class Word;
};

class Splitter
{
	public:
		Splitter();
		~Splitter();
		string SplitToPhonemes(string);
		string Syllabify(string s, TTSTrie * trie);
		bool IsVowel(TCHAR);
		bool IsConsonant(TCHAR);
	private:
		vector<string> PhonemeVector(string str);
		bool HasVowel(string str);
		int CountVowels(string);
		string SplitSyllable(string);
};

class TTSHashTrie 
{
	public:
		TTSHashTrie();
		~TTSHashTrie();
		void Insert(string str,string data);
		string Exists(string str);
		void Display();
		void DeleteAll();
	private:
		TTSHashTrieNode *Create(TRIE_NODE_TYPE);
		void Check(TTSHashTrieNode *);
		string Exists(string str,TTSHashTrieNode *);
		void Insert(TTSHashTrieNode *,string str,string data);
		void Display(TTSHashTrieNode *);
		void Delete(TTSHashTrieNode *);
		TTSHashTrieNode * root;
};

CHAR_TYPE GetTypeOf(TCHAR thechar);
string Tens(TCHAR number);
string Ones(TCHAR number);
string Sayiderece(TCHAR number);
string Tolower(string str);

Word * GetChar(TCHAR);
token * GetCharTokens(TCHAR);
token * GetCharTokensToRead(TCHAR);

void	GetParameter(string,string,string *);
int		Compare(string,string);

#endif TEXT_STRUCT