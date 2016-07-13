/**
 * TTSSound.cpp: Includes TTSSound class that provides sound playback utilies for TTS engine.
 *
 *
 */
#include "TTSSound.h"

TTSSound::TTSSound() {
  InitSound();
} 

TTSSound::TTSSound(int bPS, int sPS, int nOC) {
  InitSound(bPS, sPS, nOC);
} 

TTSSound::~TTSSound() {
  printf("Delete TTSSound \n");
  ExitSound();
}

/**
 * SoundInit():
 * Initializes necessary data, opens the wave output device by calling OpenSndDevice function.
 * Returns: TTS_NO_ERROR value when initialization is successful, 
 *          error value when initialization fails.
 */
int TTSSound::InitSound(int bitsPerSample , int samplesPerSecond, int numOfChannels) {

  int result = TTSError::TTS_NO_ERROR;
  
  isStopped = false;
  isBusy = 0; //not busy
  InitializeCriticalSection(&waveCriticalSection);
  result = OpenSndDevice(bitsPerSample, samplesPerSecond, numOfChannels);
  return result;
}

/**
 * SoundExit():
 * Closes the wave output device by calling CloseSndDevice function.
 * Returns: TTS_NO_ERROR value when initialization is successful, 
 *          error value when initialization fails.
 */

int TTSSound::ExitSound() {

  printf("Exit sound \n");;
  int result ;

  while(IsBusy());

  DeleteCriticalSection(&waveCriticalSection);
  result = CloseSndDevice();
  return result;
} // ExitSound

int TTSSound::Stop() {
  
  int result = TTSError::TTS_NO_ERROR;
  isStopped = true;
  result = waveOutReset(HWaveOut);
  if(result != MMSYSERR_NOERROR)
    return result;
  return result;
} // Stop

int TTSSound::Pause() {

  int result = TTSError::TTS_NO_ERROR;
  result = waveOutPause(HWaveOut);
  if(result != MMSYSERR_NOERROR)
    return result;

  return result;
} // Pause

int TTSSound::Restart() {

  int result = TTSError::TTS_NO_ERROR;
  result = waveOutRestart(HWaveOut);
  if(result != MMSYSERR_NOERROR)
    return result;

  return result;
} // Restart

unsigned long int TTSSound::GetPitch() {

  int error = TTSError::TTS_NO_ERROR;
  unsigned long int result = 0;
  error = waveOutGetPitch(HWaveOut, &result);
  if(error != MMSYSERR_NOERROR) {
    return error;
  }
  return result;
} // GetPitch

int TTSSound::SetPitch(unsigned long int newPitch) {

  int result = TTSError::TTS_NO_ERROR;
  result = waveOutSetPitch(HWaveOut, newPitch);
  if(result != MMSYSERR_NOERROR)
    return result;

  return result;
} // SetPitch

unsigned long int TTSSound::GetVolume() {

  int error = TTSError::TTS_NO_ERROR;
  unsigned long int result = 0;
  error = waveOutGetVolume(HWaveOut, &result);
  if(error != MMSYSERR_NOERROR) {
      printf("hata get");
	  return error;
  }
  return result;
} //GetVolume

int TTSSound::SetVolume(unsigned long int newVol) {
  int result = TTSError::TTS_NO_ERROR;
  result = waveOutSetVolume(HWaveOut, newVol);
  if(result != MMSYSERR_NOERROR)
  {
	return result;
  }
  return result;
}

unsigned long int TTSSound::GetPlaybackRate() {

  int error = TTSError::TTS_NO_ERROR;
  unsigned long int result = 0;
  error = waveOutGetPlaybackRate(HWaveOut, &result);
  if(error != MMSYSERR_NOERROR) {
    return error;
  }
  return result;
}

int TTSSound::SetPlaybackRate(unsigned long int newPRate) {

  int result = TTSError::TTS_NO_ERROR;
  result = waveOutSetPlaybackRate(HWaveOut, newPRate);
  if(result != MMSYSERR_NOERROR)
    return result;

  return result;
}

void TTSSound::PrintError_Exit(int errNum) {

  int code = 0;
  memset(errorStr, 0, 100);
  code = waveOutGetErrorText(errNum, errorStr, 100);
  if( code == MMSYSERR_NOERROR) printf("ERROR CODE: %s\n", errorStr);
  else {
    if(code != MMSYSERR_BADERRNUM) printf("Unknown Error\n");
  }
  ExitSound();
  //ExitProcess(1);
}


void TTSSound::PrintError(int errNum) {

  int code = 0;
  memset(errorStr, 0, 100);
  code = waveOutGetErrorText(errNum, errorStr, 100);
  if( code == MMSYSERR_NOERROR) printf("ERROR CODE: %s\n", errorStr);
  else {
    if(code != MMSYSERR_BADERRNUM) printf("Unknown Error\n");
  }
}

/**
 *
 */
int TTSSound::WriteSndData(/*WAVEHDR * waveHdr,*/ char * block, int size) {

  int result = TTSError::TTS_NO_ERROR;  
  WAVEHDR * waveHeader = new WAVEHDR;

  // while(isBusy == 1); //white until sound device finishes playing current
  EnterCriticalSection(&waveCriticalSection);
    isBusy = 1;
  LeaveCriticalSection(&waveCriticalSection);
  
  ZeroMemory(waveHeader, sizeof(WAVEHDR));
  waveHeader->lpData = block;
  waveHeader->dwBufferLength = size;
  //preaparing data and writing to sound device
  result = waveOutPrepareHeader(HWaveOut, waveHeader, sizeof(WAVEHDR));
  if(result != MMSYSERR_NOERROR) return result;

  result = waveOutWrite(HWaveOut, waveHeader, sizeof(WAVEHDR));
  if(result != MMSYSERR_NOERROR) return result;
  while(isBusy == 1);
  delete waveHeader;
  return result;
}

/**
 * OpenSndDevice():
 * Opens wave output device
 * Returns: TTS_NO_ERROR value on success, 
 *          error value on failure.
 */

int TTSSound::OpenSndDevice(int bitsPerSample, int samplesPerSecond, int numOfChannels) {

  WAVEFORMATEX wfx;   //wave format structure  
  int result = TTSError::TTS_NO_ERROR;   //mm function result

  //initializing wave format with defined values in the header
  wfx.wFormatTag = WAVE_FORMAT_PCM;
  wfx.nSamplesPerSec = samplesPerSecond;
  wfx.nChannels = numOfChannels;
  wfx.wBitsPerSample = bitsPerSample ;
  wfx.nAvgBytesPerSec = samplesPerSecond * numOfChannels * (bitsPerSample/8);
  wfx.nBlockAlign = (bitsPerSample/8) * numOfChannels;
  wfx.cbSize = 0;

  //opening the device with callback function
  result = waveOutOpen(&HWaveOut, 
                         WAVE_MAPPER, 
                         &wfx, 
                         (long int)SoundCallback,
                         (long int)&isBusy, 
                         CALLBACK_FUNCTION );    
  if(result != MMSYSERR_NOERROR)return result;
  return result;
}

/**
 * CloseSndDevice():
 * Closes wave output device
 * Returns: TTS_NO_ERROR value on success, 
 *          error value on failure.
 */

int TTSSound::CloseSndDevice() {

  int result = TTSError::TTS_NO_ERROR;

  //if device is still playing do not close device
  while( (result =  waveOutClose(HWaveOut)) == WAVERR_STILLPLAYING ) {
    Stop();
    }

  if(result != MMSYSERR_NOERROR) return result;
  return result;
  
}

bool TTSSound::IsBusy() {
  bool busy;
  EnterCriticalSection(&waveCriticalSection);
    busy = (isBusy == 1);
  LeaveCriticalSection(&waveCriticalSection);
  return busy;
}

/**
 * SoundCallback():
 *
 */
static void CALLBACK SoundCallback(HWAVEOUT HWaveOut, 
                                     UINT uMsg, 
                                     DWORD dwInstance, 
                                     DWORD dwParam1,
                                     DWORD dwParam2 ) {
  
  int * isBusy = (int *)dwInstance;
  
  //if playback complete for any block
  if(uMsg == WOM_DONE) {
    (*isBusy) = 0;
  }//endif
}
