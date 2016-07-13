#include <stdio.h>
#include <windows.h>
#include <string>

#include "TextStruct.h"
#include "TTSSoundSynthesis.h"

using namespace std;

class TTSEngine
{
public:
	TTSEngine();
	~TTSEngine();
	void ReadText(string text);
	void ReadTextAndWait(string text);
	void ReadWord(string word);
	void ReadCharacter(char character);
	void ReadNumber(string number);
	void StopEngine();
	void ResumeEngine();
	void PauseEngine();
	void CreateOutput();
	void SetPuncRead(bool);
	bool GetPuncRead(){ return parser->GetPuncRead(); }
	friend void WINAPI Produce(LPVOID lpParam);
	friend void WINAPI Consume(LPVOID lpParam);
private:	
	string globalTextPtr;	/* used to hold a reference to a given text to read it using threads */
	HANDLE threads[THREAD_COUNT];
	Parser* parser;
	TTSSoundSynthesis* synthesizer;
	TTSQueue *queue;
	TTSTrie  *trie;
	HANDLE mutex;
	HANDLE stopMutex;
	void RestartEngine();
};