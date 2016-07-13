#ifndef _TTS_ERROR_
#define _TTS_ERROR_

#include <windows.h>
#include <Mmsystem.h>
#include <TCHAR.h>

///////////////////////////////////////////
//////////Defines For TTSError/////////////
///////////////////////////////////////////

#define MAX_ERR_STR_SIZE 300  //maximum error string size

//error strings
#define TTS_FILE_NOT_FOUND_ERR_STR _T("Dosya Bulunamadi")
#define TTS_INVALID_FILE_ERR_STR _T("Hatali Dosya")
#define TTS_UNKNOWN_ERR_STR _T("Bilinmeyen bir hata olustu.")
#define TTS_HEAP_ALLOC_ERR_STR _T("Hafiza Kullanim Hatasi")
#define TTS_BADDEVICEID_ERR_STR _T("Hatali Donanim Numarasi")
#define TTS_NOTENABLED_ERR_STR _T("Donanim Kullanilir Hale Getirilemedi")
#define TTS_ALLOCATED_ERR_STR _T("Ses Kartiniz Su An Kullanim Halinde. Karti Kullanan Diger Programlari Kapatip Tekrar Deneyiniz")
#define TTS_NODRIVER_ERR_STR _T("Ses Donaniminiza Ait Sürücü (Driver) Bulunamadi")
#define TTS_NOTSUPPORTED_ERR_STR _T("Fonksiyon Ses Donanimi Tarafindan Desteklenmiyor")
#define TTS_INVALFLAG_ERR_STR _T("Ses Fonksiyonuna Yanlis Belirteç(Flag) Girildi")
#define TTS_INVALPARAM_ERR_STR _T("Ses Fonksiyonuna Yanlis Parametre Girildi")

namespace TTSError {


  
  const static int   TTS_NO_ERROR = 0;
  const static int   TTS_FILE_NOT_FOUND_ERR = -1;
  const static int   TTS_FILE_STAT_ERR = -2;
  const static int   TTS_THREAD_CREATE_ERR = -3;
  const static int   TTS_UNKNOWN_ERR = -4;
  const static int   TTS_HEAP_ALLOC_ERR = MMSYSERR_NOMEM;
  const static int   TTS_INVALID_FILE_ERR = -5;

  //error importance types
  const static int   TTS_ERR_TYPE_NORMAL = 1; //just print the error and go on
  const static int   TTS_ERR_TYPE_FATAL_EXIT = 2;  //program should be closed
  const static int   TTS_ERR_TYPE_FATAL_NO_EXIT = 3; //related program parts won't work
  
  void ShowError(int errorNo);
  void GetErrorStr(int errorNo, TCHAR *errStr); 
  void HandleError(int errNo);
  int GetErrorType(int errNo);

};
  
#endif