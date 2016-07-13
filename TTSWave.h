#ifndef TTSWAVE_H
#define TTSWAVE_H

#include "TTSIncludes.h"

#define WAVE_HEADER_SIZE 44
#define WAVE_TAIL_SIZE 1000
#define MERGE_PERIOD_WIDTH 7

/**
 * Type definition for Wave File Format.
 * First 12 bytes of a wav file is RIFF CHUNK
 */
typedef struct _RIFF_CHUNK {

  char riff[4]; //always "RIFF"
  unsigned int packageLength;  
  char wave[4]; //always "WAVE"

}RIFF_CHUNK;

/**
 * Type definition for Wave File Format.
 * Second 24 bytes of a wav file is FORMAT CHUNK
 */

typedef struct _FORMAT_CHUNK {

  char fmt[4];  //always "fmt "
  unsigned int formatLength; //Binary, always 0x10
  unsigned short reserved;  //always 0x1
  unsigned short numOfChannels; //Channel Numbers (Always 0x01=Mono, 0x02=Stereo)
  unsigned int samplingRate;  //Sample Rate (Binary, in Hz)
  unsigned int bytePerSecond; //Bytes Per Second
  unsigned short bytePerSample;  //Bytes Per Sample: 1=8 bit Mono, 2=8 bit Stereo or 16 bit Mono, 4=16 bit Stereo
  unsigned short bitPerSample;  //Bits Per Sample
  
}FORMAT_CHUNK ;

/**
 * Type definition for Wave File Format.
 * Next 8 bytes of a wav file if DATA CHUNK
 * After this chunk wave data exists
 */
typedef struct _DATA_CHUNK {

  char data[4]; //always "data"
  unsigned int dataLength;  //length of wave data

}DATA_CHUNK;

/**
 * Type definition for Wave File Format.
 * Represents all wave file chunks
 */
typedef struct _WAVE_CHUNKS {

  RIFF_CHUNK riffChunk;
  FORMAT_CHUNK formatChunk;
  DATA_CHUNK dataChunk;
  
} WAVE_CHUNKS;


class TTSWave {

public:

  TTSWave();
  ~TTSWave();
  
  void ReadFile(TCHAR *fileName);
  void WriteFile(TCHAR *fileName, WAVE_CHUNKS header);
  
  int GetSize() { return size; };
  char * GetData() { return data; };
  void SetData(char *newData, int newSize);
  
  void SetRightPitchPeriod(int period) { rightPitchPeriod = period; };
  void SetLeftPitchPeriod(int period) { leftPitchPeriod = period; };
  int GetRightPitchPeriod() { return rightPitchPeriod; };
  int GetLeftPitchPeriod() { return leftPitchPeriod; };
  TTSWave & Concat(TTSWave & wave);
  void AddWave(TTSWave &wave);
  void AddSilence(int msec);

private:

  char * data;
  int leftPitchPeriod;
  int rightPitchPeriod;
  int size;

};

#endif