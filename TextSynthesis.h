
#ifndef _TEXT_SYNTHESIS_
#define _TEXT_SYNTHESIS_ 

#include "TTSError.h"
#include <memory.h>
#include <stdio.h>
#include <TCHAR.H>

#define MAX_PHONE_COUNT 100

#define MAX_PHONE_LENGTH 6

int IsVowel(TCHAR ch);
void GetPhones(TCHAR * word, TCHAR phones[][MAX_PHONE_LENGTH], int* count);

#endif