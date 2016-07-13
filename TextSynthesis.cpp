
#include "TextSynthesis.h"


void GetPhones(TCHAR * word, TCHAR phones[][MAX_PHONE_LENGTH], int* count)
{
  
  
  int wordLength = _tcsclen(word);
  int phoneCount = 0;
  int current, previousPhoneEnd;
  current = previousPhoneEnd = 0;
  memset(phones, 0, MAX_PHONE_LENGTH * MAX_PHONE_COUNT);
  
 
  for(int i = 0; i< wordLength; i+= _tclen(word+i) ) {
    current = i;
      
    if(current == (wordLength -1) ) {
      
      if( (current - previousPhoneEnd+1) > 3) {
        if(!IsVowel(word[previousPhoneEnd+2]) && !IsVowel(word[previousPhoneEnd+3])) {
          memcpy(&(phones[phoneCount]), &word[previousPhoneEnd],3);
          previousPhoneEnd+=3;
        }
        else {
          memcpy(&(phones[phoneCount]), &word[previousPhoneEnd],2);
          previousPhoneEnd+=2;
        }
        
        phoneCount++;
      }
      
      memcpy(&(phones[phoneCount]), &word[previousPhoneEnd],current - previousPhoneEnd+1);
      phoneCount++;
      break;
    }
    
    else if(IsVowel(word[i])) {
      
      if(i==0) {
        ;
      }

      else if( IsVowel(word[i+1])) {
        memcpy(&phones[phoneCount], &word[previousPhoneEnd], current - previousPhoneEnd+1);
        previousPhoneEnd = current+1;
        phoneCount++;
      }

      else if( IsVowel(word[i-1])) {
        ;
      }

      //if both sides are vowel
      else if(!IsVowel(word[i+1]) ) {
        
        if( (current - previousPhoneEnd+1) > 3) {
          if(!IsVowel(word[previousPhoneEnd+2]) && !IsVowel(word[previousPhoneEnd+3])) {
            memcpy(&(phones[phoneCount]), &word[previousPhoneEnd],3);
            previousPhoneEnd+=3;
            
          }
          else {
            memcpy(&(phones[phoneCount]), &word[previousPhoneEnd],2);
            previousPhoneEnd+=2;
          }
          phoneCount++;
        }
        memcpy(&phones[phoneCount], &word[previousPhoneEnd], current - previousPhoneEnd+1);
        previousPhoneEnd = current;
        phoneCount++;
      }
      
    }
  }

  
  *count = phoneCount;
}

int IsVowel(TCHAR ch) {
  
  TCHAR vowels[] = {'a', 'â', 'e', 'ý' , 'î', 'i', 'o', 'ö', 'u', 'û', 'ü'};
  for (int i =0; i< (int)_tcsclen(vowels); i+= (int)_tclen(vowels) ) {
    if(vowels[i] == ch) return 1;
  }
   return 0;
}
