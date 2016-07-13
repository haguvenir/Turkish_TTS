
#ifndef SOUND_SYNTHESIS
#define SOUND_SYNTHESIS 

#include "TTSTrie.h"
#include "TTSSound.h"
#include "TextSynthesis.h"
#include "TTSIncludes.h"
#include "TTSWave.h"
#include "TextStruct.h"


///////////////////////////////////////////
//////////Defines For TTSSoundSynthesis////
///////////////////////////////////////////

#define SOUND_DATA_FILE "veriler\\sesler.dat" //sound data file dir&name

#define TTS_SAVE_MODE 1

class TTSSoundSynthesis {
public:
	TTSSoundSynthesis(TTSTrie*);
	~TTSSoundSynthesis();
	void SayWord(TCHAR * word);
	void SayWord(Word *);
	void StopSynthesizer(){ts->Stop();}
	bool IsBusy() { return ts->IsBusy();}
	void SetBusy(bool flag){ ts->SetBusy(flag); }
	void SaveOutput(){ total.WriteFile("veriler\\output.wav", header); }

private:

  TTSWave total, silence;
  WAVE_CHUNKS header;
  char *soundBuffer;
  int totalSize;
  TTSTrie *t;
  TTSSound *ts;
  
  void UpdateConfigVariables(Word*);

  int LoadSounds();
  void GetPhoneInfo(TCHAR  phones[],  TTSWave & wave); 
  void GetPhoneSound(TCHAR *phoneBuffer, int phoneOffset, int phoneSize);
  void GetWordSound(TCHAR phones[][6], int count, TTSWave & wave); 
  bool IsVowel(TCHAR ch);

};
#endif
