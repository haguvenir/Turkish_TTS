#include "TTSError.h"

////////////////////////////////////////////////
//errors kjdfkldfa
////////////////////////////////////////////////

void TTSError::GetErrorStr(int errorNo, TCHAR *errStr) {

  ZeroMemory(errStr, MAX_ERR_STR_SIZE);
  //multimedia related errors
  switch(errorNo) {

  case TTS_UNKNOWN_ERR:
  
  case MMSYSERR_ERROR:
  
  case MMSYSERR_HANDLEBUSY:

  case MMSYSERR_INVALHANDLE: 

  case MMSYSERR_BADERRNUM:
    // unspecified error 
    _tcscpy(errStr, TTS_UNKNOWN_ERR_STR);  
    break;
  
  case MMSYSERR_BADDEVICEID: 
    //device ID out of range
    _tcscpy(errStr, TTS_BADDEVICEID_ERR_STR);  
    break;
  
  case MMSYSERR_NOTENABLED: 
    //driver failed enable  
    _tcscpy(errStr, TTS_NOTENABLED_ERR_STR); 
    break;
  
  case MMSYSERR_ALLOCATED:  
    //device already allocated 
    _tcscpy(errStr, TTS_ALLOCATED_ERR_STR);
    break;
  
  case MMSYSERR_NODRIVER:
    //no device driver present
    _tcscpy(errStr, TTS_NODRIVER_ERR_STR);
    break;

  case MMSYSERR_NOMEM: 
    //memory allocation error
    _tcscpy(errStr, TTS_HEAP_ALLOC_ERR_STR);
    break;    

  case MMSYSERR_NOTSUPPORTED: 
    //function isn't supported
    _tcscpy(errStr, TTS_NOTSUPPORTED_ERR_STR);
    break;

  case MMSYSERR_INVALFLAG: 
    //invalid flag passed
    _tcscpy(errStr, TTS_INVALFLAG_ERR_STR);
    break;
  
  case MMSYSERR_INVALPARAM: 
    //invalid parameter passed
    _tcscpy(errStr, TTS_INVALPARAM_ERR_STR);
    break;

  case TTS_FILE_NOT_FOUND_ERR:
    //File Not Found
    _tcscpy(errStr, TTS_FILE_NOT_FOUND_ERR_STR);
    break;

  case TTS_INVALID_FILE_ERR:
    //invalid file
    _tcscpy(errStr, TTS_INVALID_FILE_ERR_STR);

  }//end switch
  

}

void TTSError::ShowError(int errNo) {
  /**
  TCHAR errStr[MAX_ERR_STR_SIZE];
  GetErrorStr(errNo, errStr);
  MessageBox(NULL, errStr, "Hata Olustu", MB_OK);
  /**/
}





void TTSError::HandleError(int errNum) {
  ;
}

int TTSError::GetErrorType(int errNum) {
  return 0;
}
