/** 
* TTSSoundSynthesis.cpp: Implementation for TTSSoundSynthesis methods.
*
*/
#include "TTSSoundSynthesis.h"


TTSSoundSynthesis::TTSSoundSynthesis(TTSTrie * trie)
{
	t = trie;
	LoadSounds();
	//ts = new TTSSound;
	ts = new TTSSound(header.formatChunk.bitPerSample, header.formatChunk.samplingRate, header.formatChunk.numOfChannels);
}

TTSSoundSynthesis::~TTSSoundSynthesis()
{
	//cout<<"destr TTSSound"<<endl;
	if(ts != NULL) delete ts;

	//cout<<"deleted TTSSound"<<endl;
	if(soundBuffer != NULL) 
	{
		HeapFree(GetProcessHeap(), 0, soundBuffer);
	}

}

unsigned int
getIntFromFourByteLE( unsigned char *bytes )
{
	return(    (unsigned int) bytes[0] 
	+ ( (unsigned int) bytes[1] << 8  )
		+ ( (unsigned int) bytes[2] << 16 )
		+ ( (unsigned int) bytes[3] << 24 ) );  
} // getIntFromFourByteLE


int TTSSoundSynthesis::LoadSounds() {
	FILE * fp, *fpIndex;
	struct _stat fileStat;
	int size=0;
	int readBytes=0;
	char *silenceData;
	char input[256];

	int letterCount,//letter count in a phone path 
		result, 
		offset,
		ssize;

	//opening file
	if( (fpIndex = _tfopen(TTS_TRIE_DATA_FILE, "r")) ==NULL ) 
	{
		TTSError::ShowError(TTSError::TTS_FILE_NOT_FOUND_ERR);
		return 0;
	}

	result =  _ftscanf(fpIndex, "%d", &letterCount); //number of letters in the phone path
	_ftscanf(fpIndex, "%s", input) ;  //get phone/diphone/triphone
	_ftscanf(fpIndex, "%d", &offset); //get offset
	_ftscanf(fpIndex, "%d", &ssize);   //get size

	fclose(fpIndex);


	//printf("loading...\n");
	//opening file
	if((fp = fopen(SOUND_DATA_FILE, "rb")) == NULL) {
		TTSError::ShowError(TTSError::TTS_FILE_NOT_FOUND_ERR);
		return 0;
	}
	//printf("%s loaded..., getting stats...\n", SOUND_DATA_FILE);
	//getting size of file
	if( _tstat(SOUND_DATA_FILE, &fileStat) != 0 ) {
		TTSError::ShowError(TTSError::TTS_FILE_STAT_ERR);
		return 0;
	}

	totalSize = size = fileStat.st_size;
	printf("totalSize = %d\n", totalSize);
	if((soundBuffer = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size)) == NULL) {
		TTSError::ShowError(TTSError::TTS_HEAP_ALLOC_ERR);
		return 0; 
	}

	fread((void *)&header, 1, 44, fp);
	cout << "header.riffChunk.packageLength = " << header.riffChunk.packageLength << endl;
	cout << "header.dataChunk.dataLength = " << header.dataChunk.dataLength << endl;

	if((silenceData = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, offset-44)) == NULL) {
		TTSError::ShowError(TTSError::TTS_HEAP_ALLOC_ERR);
		return 0; 
	}
	readBytes = fread(silenceData, 1, (offset-44), fp);
	silence.SetData(silenceData,offset-44);

	fseek(fp,44L,SEEK_SET);

	readBytes = fread(soundBuffer, 1, (size-44), fp);

	fclose(fp);

	return size;
}



///not complete
void TTSSoundSynthesis::GetPhoneInfo(TCHAR  phones[], TTSWave & wave) {


	TTSTrieElement *te;

	char * phoneBuffer;
	int size = -1,  offset = -1 ;



	te = t->GetTrieElement(phones);

	if (te == NULL){
		wave.SetData(NULL,0);
		printf("\n***************Phone Not Found: %s... \n", phones);
		return;

	}

	size = te->size;
	offset = te->fileOffset;

	printf("Offset  %d    Size:   %d\n", te->fileOffset, te->size);


	/**/
	//if phone not found in database
	//divide subphones

	if(size <= 0 || offset < 0) {  
		wave.SetData(NULL,0);
		printf("\n***************Phone Not Found: %s... \n", phones);
		return;
		//    exit(1);    
	}



	/**/
	//size = size -100;
	if((phoneBuffer = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size)) == NULL) {

		TTSError::ShowError(TTSError::TTS_HEAP_ALLOC_ERR);
		return;
		//        exit(1);   
	}


	memcpy(phoneBuffer, (soundBuffer+offset), size );
	wave.SetData(phoneBuffer, (size));

}

void TTSSoundSynthesis::GetWordSound(TCHAR phones[][6], int count, TTSWave &wave) {

	TTSWave myWave, temp;

	bool isLeftVowel, isRightVowel, prevRightVowel;
	prevRightVowel = isLeftVowel = isRightVowel = false;
	TCHAR last;

	//reading first phone
	//GetPhoneInfo(phones[0], &size, &offset, myWave);
	//printf("\nFirst phone complete!!!");

	for(int i = 0; i<count; i++) {

		//printf("Fetch phonem: %s \n",phones[i]);
		GetPhoneInfo(phones[i], temp);

		// if phone is not found in the trie
		if (temp.GetData() == NULL){
			return;
		}

		//printf(" phone complete!!! ");

		isLeftVowel = IsVowel(phones[i][0]);
		last = (phones[i][2]==0)?phones[i][1]:phones[i][2];
		isRightVowel = IsVowel(last);

		if(isLeftVowel) { 
			//printf("Left Vowel"); 
			temp.SetLeftPitchPeriod(0); 
		}
		if(isRightVowel){ 
			//printf("Right Vowel"); 
			temp.SetRightPitchPeriod(0); 
		}

		printf("\n");

		//GetPhoneSound(temp, offset, size);
		myWave.Concat(temp);
	}
	wave.Concat(myWave);

}




void TTSSoundSynthesis::SayWord(TCHAR * word) {

	TTSWave totalWave;
	int phoneCount = 0;
	int size = 0, totalSize = 0, offset = 0;
	TCHAR phones[MAX_PHONE_COUNT][MAX_PHONE_LENGTH];

	GetPhones(word, phones, &phoneCount);
	while(IsBusy());
	GetWordSound(phones, phoneCount, totalWave);

	totalWave.Concat(silence);

	/**/
#ifdef TTS_SAVE_MODE

	total.AddWave(totalWave);

#else

	ts->WriteSndData(totalWave.GetData(), totalWave.GetSize());

#endif

}

// display the content of a rule update
//
void TTSSoundSynthesis::UpdateConfigVariables(Word *word)
{

	if(word->GetRule()->ruleNo == 4)
	{
		if(word->GetRule()->data == 1)
		{
			unsigned long int volume = ts->GetVolume();
			cout<<"vol: "<<volume<<endl;
			ts->SetVolume(volume * 3);
			volume = ts->GetVolume();
			cout<<"vol: "<<volume<<endl;

		}
	}
}

void TTSSoundSynthesis::SayWord(Word * word) 
{
	if(word->GetType() == INFO)
	{ 
		// Rule update..., take possible actions.
		cout<<"Rule update message info"<<endl;
		UpdateConfigVariables(word);
	}
	else
	{
		// Not a rule...
		TTSWave totalWave;
		token *wordtoken;
		int phoneCount = 0;
		int size = 0, totalSize = 0;
		if (SPLIT_BY_PHONEME) {
			phoneCount = word->GetPCount();
		} else {
			phoneCount = word->GetSCount();
		}

		TCHAR phones[MAX_PHONE_COUNT][MAX_PHONE_LENGTH];


		if (SPLIT_BY_PHONEME) {
			wordtoken = word->GetPHeader();
		} else {
			wordtoken = word->GetSHeader();
		}

		for(int i=0; i<phoneCount; i++)
		{
			cout<<"token: "<<wordtoken->cont<<endl;
			strcpy(phones[i],&wordtoken->cont[0]);
			wordtoken = wordtoken->next;
		}
		while(IsBusy());

		GetWordSound(phones, phoneCount, totalWave);

		totalWave.AddWave(silence);
		/**/
#ifdef TTS_SAVE_MODE
		total.AddWave(totalWave);
		total.AddWave(silence);
#endif
		ts->WriteSndData(totalWave.GetData(), totalWave.GetSize());
	}
}

void TTSSoundSynthesis::GetPhoneSound(TCHAR *phoneBuffer, int phoneOffset, int phoneSize) {

	memcpy(phoneBuffer, (soundBuffer+phoneOffset), phoneSize );

}

bool TTSSoundSynthesis::IsVowel(TCHAR ch) {
	bool result = false;
	switch(ch) {
	case 'a': 
	case 'â':
	case 'e':
	case 'ý':
	case 'î':
	case 'i': 
	case 'o': 
	case 'ö': 
	case 'u':
	case 'û':
	case 'ü':
		result = true;
		break;
	default: ;
	}
	return result;
}

