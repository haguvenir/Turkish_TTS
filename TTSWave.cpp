#include "TTSWave.h"

TTSWave::TTSWave() {

  data = NULL;
  leftPitchPeriod = 0;
  rightPitchPeriod = 0;
  size = 0;

}

TTSWave::~TTSWave() {

  if(data != NULL) {
    HeapFree(GetProcessHeap(), 0, data);
  }

}

void TTSWave::ReadFile(TCHAR * fileName) {

  FILE * fp;
  WAVE_CHUNKS head;

  int readBytes=0;
  
  //opening file
  if((fp = _tfopen(fileName, "rb")) == NULL) {
    TTSError::ShowError(TTSError::TTS_FILE_NOT_FOUND_ERR);
	  return;
//    exit(1);
  }

  
  fread(&head,1,WAVE_HEADER_SIZE,fp);

  size = head.dataChunk.dataLength; //ignore header, take just data

  if((data = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size)) == NULL) {
    TTSError::ShowError(TTSError::TTS_HEAP_ALLOC_ERR);
	  return;
//    exit(1);   
  }
  
  
  
  size = size - WAVE_HEADER_SIZE - WAVE_TAIL_SIZE;

  while(readBytes < size) {
    readBytes += fread(data+readBytes, 1, size - readBytes, fp);
  }

  for(int i = 0; i<size; i++) {
    if(data[i] == 'c' && data[i+1] == 'u' && data[i+2] == 'e') {
      cout<<"--------------------"<<i<<"---"<<size - i<<endl;
      break;
    }
  }
  fclose(fp);

}


/**
 * WriteFile: Used for writing wave output file for only specific format of:
 *                    22050 hz, mono, 8 bit per sample data.
 *
 */
void TTSWave::WriteFile(TCHAR *fileName, WAVE_CHUNKS chunks){
      
  FILE * fp;
  int written = 0;
  
  chunks.dataChunk.dataLength = size;
  //opening file
  if((fp = _tfopen(fileName, "wb")) == NULL) {
    TTSError::ShowError(TTSError::TTS_UNKNOWN_ERR);
	  return;
//    exit(1);
  }
  
  fwrite(&chunks, 1, WAVE_HEADER_SIZE, fp);
  while(written < size ) {
    written += fwrite(data+written, 1, size - written, fp);
  }
  fclose(fp);
}

/**
 * Concatenates given wav file with this one
 */
TTSWave & TTSWave::Concat(TTSWave & wave) {

  char * temp;
  int newSize = 0; 
  
  if(this->rightPitchPeriod != 0 && wave.GetLeftPitchPeriod() != 0) {

    int mergeAmount = this->rightPitchPeriod * MERGE_PERIOD_WIDTH;
    if( mergeAmount >= 3*this->size/2 ) 
      mergeAmount = this->size/2;
    
    newSize = this->size + wave.GetSize() - mergeAmount;
    
    if((temp = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, newSize)) == NULL) {
      TTSError::ShowError(TTSError::TTS_HEAP_ALLOC_ERR);
	  return *this;
//      exit(1);   
    }

    
    memcpy(temp, this->data, this->size - mergeAmount);
    memcpy(temp + this->size - mergeAmount, wave.GetData(), wave.GetSize());
    
  }
  else {
    newSize = this->size + wave.GetSize();
    if((temp = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, newSize)) == NULL) {
        TTSError::ShowError(TTSError::TTS_HEAP_ALLOC_ERR);
	  return *this;
//        exit(1);   
      }
    if(this->data != NULL) {
      memcpy(temp, this->data, this->size);
      memcpy(temp+this->size, wave.GetData(), wave.GetSize());
    }
    else {
      memcpy(temp, wave.GetData(), wave.GetSize());
    }  
    
  }

  SetData(temp, newSize);
  this->rightPitchPeriod = wave.GetRightPitchPeriod();

  return *this;
}

void TTSWave::AddWave(TTSWave &wave) {

  char * temp;
  int newSize = 0; 
  
  newSize = this->size + wave.GetSize();
  
  if((temp = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, newSize)) == NULL) {
      TTSError::ShowError(TTSError::TTS_HEAP_ALLOC_ERR);
	  return;
//      exit(1);   
  }

  if(this->data != NULL) {
    memcpy(temp, this->data, this->size);
    memcpy(temp+this->size, wave.GetData(), wave.GetSize());
  }
  else {
    memcpy(temp, wave.GetData(), wave.GetSize());
  }  

  SetData(temp, newSize);
  this->rightPitchPeriod = wave.GetRightPitchPeriod();
}


void TTSWave::SetData(char *newData, int newSize) {

  if(data!=NULL) {
    HeapFree(GetProcessHeap(), 0, this->data);
  }

  this->data = newData;
  this->size = newSize;

}