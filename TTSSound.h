/**
 * TTSSound.h: Includes TTSSound class that provides sound playback utilies for TTS engine.
 *
 *
 */

#ifndef _TTSSOUND_
#define _TTSSOUND_

#include "TTSError.h"
#include "TTSIncludes.h"

///////////////////////////////////////////
//////////Defines For TTSSound/////////////
///////////////////////////////////////////
#define BLOCK_SIZE 8192   //size of each wave data block in each wave header
#define BLOCK_COUNT 5     //sound data block count, also wave header count

//global definitions for wave file attributes

#define DEFAULT_BITS_PER_SAMPLE		8		//bits per sample
//#define DEFAULT_SAMPLES_PER_SECOND	44100	//sample per second in hertz
#define DEFAULT_SAMPLES_PER_SECOND	11025	//sample per second in hertz
#define DEFAULT_NUM_OF_CHANNELS		1 		//number of channels 1 for mono, 2 for stereo

class TTSSound {
public:

  TTSSound();
  TTSSound(int bPS, int sPS, int noC);
  ~TTSSound();
  
  int Stop();
  int Pause();
  int Restart();
  int SetPitch(unsigned long int newPitch);
  unsigned long int GetPitch();
  int SetVolume(unsigned long int newVol);
  unsigned long int GetVolume();
  int SetPlaybackRate(unsigned long int newRate);
  unsigned long int GetPlaybackRate();
  int WriteSndData(char * block, int size);
  bool IsBusy();
  void SetBusy(bool flag){ isBusy = flag; }

private:
  
  int InitSound(int bitsPerSample = DEFAULT_BITS_PER_SAMPLE, 
                int samplesPerSecond = DEFAULT_SAMPLES_PER_SECOND, 
                int numOfChannels = DEFAULT_NUM_OF_CHANNELS) ;
  int ExitSound();

  void PrintError_Exit(int errNum);
  void PrintError(int errNum);
  int OpenSndDevice(int bitsPerSample, int samplesPerSecond, int numOfChannels); 
  int CloseSndDevice(); 


  HWAVEOUT HWaveOut; //device handle
  char errorStr[100];
  CRITICAL_SECTION waveCriticalSection;
  bool isStopped ;
  int isBusy;
  
};

static void CALLBACK SoundCallback(HWAVEOUT hWaveOut, 
                                   UINT uMsg, 
                                   DWORD dwInstance, 
                                   DWORD dwParam1,
                                   DWORD dwParam2 );

#endif

