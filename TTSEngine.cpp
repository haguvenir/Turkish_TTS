#include <stdio.h>
#include <windows.h>
#include <string>
#include "TTSEngine.h"

#define OUT

TTSEngine::TTSEngine()
{
	//cout<<"Locale set"<<endl;
	//setlocale(LC_ALL, "Turkish");
	//cout<<"TTSQueue create"<<endl;
	queue   = new TTSQueue();
	//cout<<"ok, TTSTrie create"<<endl;
	trie	= new TTSTrie();
	//cout<<"ok, Parser create"<<endl;
	parser = new Parser(queue,trie);
	//cout<<"ok, Synthesizer create"<<endl;
	synthesizer = new TTSSoundSynthesis(trie);
	threads[0] = NULL;
	threads[1] = NULL;
	mutex = CreateSemaphore(NULL, 1, 1, "mutex2");
	stopMutex = CreateSemaphore(NULL, 1, 1, "stopMutex");
	//cout<<"ok"<<endl;
}

TTSEngine::~TTSEngine()
{
	//cout<<"destr TTSEngine"<<endl;
	StopEngine();
	delete queue;
	delete trie;
	delete parser;
	delete synthesizer;
}

void TTSEngine::ReadText(string textToRead)
{
	try{
	//WaitForSingleObject(mutex, INFINITE);
	// while(queue->isBusy);
	StopEngine();
	//if(threads[0] != NULL)
	//{
	//	/**/
	//	// If the function succeeds, the return value is nonzero
	//	if(TerminateThread(threads[0], 0) == 0)
	//	{
	//		//MessageBox(NULL,"Producer thread could not be terminated...","Error",0);
	//		//exit(0);
	//	}
	//	/**/
	//	// an error occured if CloseHandle returns 0
	//	if(CloseHandle(threads[0]) == 0)
	//	{
	//		//MessageBox(NULL,"Error closing producer handle...","Error",0);
	//		//exit(0);
	//	}

	//}
	//if(threads[1] != NULL)
	//{
	//	// If the function succeeds, the return value is nonzero
	//	/**/
	//	if(TerminateThread(threads[1], 0) == 0)
	//	{
	//		//MessageBox(NULL,"Consumer thread could not be terminated...","Error",0);
	//		//exit(0);
	//	}
	//	/**/

	//	// an error occured if CloseHandle returns 0
	//	if(CloseHandle(threads[1]) == 0)
	//	{
	//		//MessageBox(NULL,"Error closing consumer handle...","Error",0);
	//		//exit(0);
	//	}
	//}
	
	globalTextPtr = textToRead;
	
	int param = 0;
	unsigned long iID=0;
	
	queue->isBusy = true;
	threads[0] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(Produce),this,CREATE_SUSPENDED,&iID);
	threads[1] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(Consume),this,CREATE_SUSPENDED,&iID);
	
	if(threads[0] != NULL && threads[1] != NULL)
	{
		/*********************************************************************
		 * if the threads are created succesfully, then no problem, initiate *
		 * them and wait for them to finish                                  *
		 *********************************************************************/
		// synthesizer->SetBusy(true);
		ResumeThread(threads[0]);
		ResumeThread(threads[1]);
	
		//cout<<"Waiting for threads"<<endl;
		//WaitForMultipleObjects(THREAD_COUNT, threads, true, INFINITE);
		//cout<<"Threads finished successfully"<<endl;
		//CloseHandle(threads[0]);
		//CloseHandle(threads[1]);
		// MessageBox(NULL,"Threads created","!",MB_OK);
	}
	else
	{
		/********************************************************************
		 * If not successfull creating threads, then read serially          *
		 ********************************************************************/
		parser->LoadString(globalTextPtr);
		parser->Parse();
		Word* w = parser->GetWord();
		while(w != NULL)
		{
			synthesizer->SayWord(w);
			w = parser->GetWord();
		}
		while(synthesizer->IsBusy());
	}
	//ReleaseSemaphore(mutex, 1, NULL);
	
	}catch(exception exc){/*RestartEngine();*/}
	
}

void TTSEngine::ReadTextAndWait(string textToRead)
{
	try{
	//WaitForSingleObject(mutex, INFINITE);
	StopEngine();
	// while(queue->isBusy);
	//if(threads[0] != NULL)
	//{
	//	/**/
	//	// If the function succeeds, the return value is nonzero
	//	if(TerminateThread(threads[0], 0) == 0)
	//	{
	//		//MessageBox(NULL,"Producer thread could not be terminated...","Error",0);
	//		//exit(0);
	//	}
	//	/**/
	//	// an error occured if CloseHandle returns 0
	//	if(CloseHandle(threads[0]) == 0)
	//	{
	//		//MessageBox(NULL,"Error closing producer handle...","Error",0);
	//		//exit(0);
	//	}

	//}
	//if(threads[1] != NULL)
	//{
	//	// If the function succeeds, the return value is nonzero
	//	/**/
	//	if(TerminateThread(threads[1], 0) == 0)
	//	{
	//		//MessageBox(NULL,"Consumer thread could not be terminated...","Error",0);
	//		//.exit(0);
	//	}
	//	/**/

	//	// an error occured if CloseHandle returns 0
	//	if(CloseHandle(threads[1]) == 0)
	//	{
	//		//MessageBox(NULL,"Error closing consumer handle...","Error",0);
	//		//exit(0);
	//	}
	//}

	globalTextPtr = textToRead;
	
	int param = 0;
	unsigned long iID=0;
	
	queue->isBusy = true;
	threads[0] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(Produce),this,CREATE_SUSPENDED,&iID);
	threads[1] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(Consume),this,CREATE_SUSPENDED,&iID);
	
	if(threads[0] != NULL && threads[1] != NULL)
	{
		/*********************************************************************
		 * if the threads are created succesfully, then no problem, initiate *
		 * them and wait for them to finish                                  *
		 *********************************************************************/
		// synthesizer->SetBusy(true);
		ResumeThread(threads[0]);
		ResumeThread(threads[1]);
	
		//cout<<"Waiting for threads"<<endl;
		WaitForMultipleObjects(THREAD_COUNT, threads, true, INFINITE);
		//cout<<"Threads fýnýshed successfully"<<endl;
		//CloseHandle(threads[0]);
		//CloseHandle(threads[1]);
		// MessageBox(NULL,"Threads created","!",MB_OK);
	}
	else
	{
		/********************************************************************
		 * If not successfull creating threads, then read serially          *
		 ********************************************************************/
		parser->LoadString(globalTextPtr);
		parser->Parse();
		Word* w = parser->GetWord();
		while(w != NULL)
		{
			synthesizer->SayWord(w);
			w = parser->GetWord();
		}
		while(synthesizer->IsBusy());
	}
	//ReleaseSemaphore(mutex, 1, NULL);
	}catch(exception exc){/*RestartEngine();*/}
	
}

void TTSEngine::ReadWord(string word)
{
	try{
	/********************************************************************
	 * Read the given word serially, because it is short                *
	 ********************************************************************/
	
	Word* w = new Word(word);
	w->SetType(CUMLE);
	w->Update(parser);
	w->FindTokens(parser->GetSplitter(),trie);
	synthesizer->SayWord(w);
	// wait until the word is read
	// by the soundcard
	while(synthesizer->IsBusy());
	}catch(exception exc){/*RestartEngine();*/}
}

void TTSEngine::ReadCharacter(char character)
{
	try{
	/********************************************************************
	 * Read the given character serially, because it is short           *
	 ********************************************************************/
	Word * word = GetChar(character);
	while(word != NULL)
	{
		ReadWord(word->GetContent());
		word = word->GetNext();
	}
	}catch(exception exc){/*RestartEngine();*/}
}

void TTSEngine::ReadNumber(string number)
{
	try{
	/********************************************************************
	 * Read the given number serially, because it is short              *
	 ********************************************************************/
	if(number.length() == 0)
	{
		//MessageBox(NULL,"sorun","sorun",0);
		return;
	}
	Word *word = new Word(number);
	word->SetType(SAYI);
	word->Update(parser);
	Word * tmp = word;
	while(tmp != NULL)
	{
		tmp->FindTokens(parser->GetSplitter(), trie);
		synthesizer->SayWord(tmp);
		tmp = tmp->GetNext();
	}
	}catch(exception exc){/*RestartEngine();*/}
}

void TTSEngine::CreateOutput()
{
	try{
	synthesizer->SaveOutput();
	}catch(exception exc){/*RestartEngine();*/}
}

void TTSEngine::StopEngine()
{	
	try{
	WaitForSingleObject(stopMutex, INFINITE);
	// terminate the threads
	// close their handles
	// empty the queue
	// empty the sound buffer in the sound card
	/**/
	if(threads[0] != NULL)
	{
		/**/
		// If the function succeeds, the return value is nonzero
		if(TerminateThread(threads[0], -1) == 0)
		{
			//MessageBox(NULL,"Producer thread could not be terminated...","Error",0);
			//exit(0);
		}
		/**
		else
			MessageBox(NULL,"Producer thread terminated...","Info",0);
		/**/
	
		/**/
		// an error occured if CloseHandle returns 0
		if(CloseHandle(threads[0]) == 0)
		{
			//MessageBox(NULL,"Error closing producer handle...","Error",0);
			//exit(0);
		}
		/**
		else
			MessageBox(NULL,"Producer handle closed...","Info",0);
		/**/
	
	}
	if(threads[1] != NULL)
	{
		// If the function succeeds, the return value is nonzero
		/**/
		if(TerminateThread(threads[1], -2) == 0)
		{
			//MessageBox(NULL,"Consumer thread could not be terminated...","Error",0);
			//exit(0);
		}
		/**
		else
			MessageBox(NULL,"Consumer thread terminated...","Info",0);
		/**/
		
		/**/
		// an error occured if CloseHandle returns 0
		if(CloseHandle(threads[1]) == 0)
		{
			//MessageBox(NULL,"Error closing consumer handle...","Error",0);
			//exit(0);
		}
		/**
		else
			MessageBox(NULL,"Consumer handle closed...","Info",0);
		/**/
	}
	
	// make the threads null for later control
	if(threads[0] != NULL)
		threads[0] = NULL;
	if(threads[1] != NULL)
		threads[1] = NULL;
			
	queue->EmptyQueue();
	synthesizer->StopSynthesizer();
	//ReleaseSemaphore(mutex, 1, NULL);
	ReleaseSemaphore(stopMutex, 1, NULL);
	// empty sound buffer here
	/**/
	}catch(exception exc){/*RestartEngine();*/}
	
}

// resume the threads if they are not already terminated
void TTSEngine::ResumeEngine()
{
	try{
	if(threads[0] != NULL)
		ResumeThread(threads[0]);
	if(threads[1] != NULL)
		ResumeThread(threads[1]);
	}catch(exception exc){/*RestartEngine();*/}
}

void WINAPI Produce(LPVOID lpParam)
{
	TTSEngine *engine = (TTSEngine*)lpParam;
	try{

		WaitForSingleObject(engine->mutex, INFINITE);

		engine->parser->LoadString((engine->globalTextPtr));
		engine->parser->Parse();
		cout<<"OK - producer"<<endl;
		engine->queue->isBusy = false;

		ReleaseSemaphore(engine->mutex, 1, NULL);

	}catch(exception exc){
		/*engine->RestartEngine();*/
	}
}

void WINAPI Consume(LPVOID lpParam)
{
	TTSEngine *engine = (TTSEngine*)lpParam;
	try{

	Word *wrd;


	while((wrd = engine->queue->Dequeue()) != NULL || engine->queue->isBusy)
	{
		WaitForSingleObject(engine->stopMutex, INFINITE);
	
		if(engine->threads[0] == NULL || engine->threads[1] == NULL)
		{
			ReleaseSemaphore(engine->stopMutex, 1, NULL);
			return;
		}
		if(wrd != NULL)
		{
			engine->synthesizer->SayWord(wrd);
			ReleaseSemaphore(engine->stopMutex, 1, NULL);
			cout<<"OK - consumer"<<endl;
		}
		else if(!engine->queue->isBusy) 
		{
			ReleaseSemaphore(engine->stopMutex, 1, NULL);
			break;
		}
		else
		{
			ReleaseSemaphore(engine->stopMutex, 1, NULL);
		}
	}
	
	while(engine->synthesizer->IsBusy());
	}catch(exception exc){
		cout<<"consumer is terminated abnormally!"<<endl;
		//MessageBox(NULL,"consumer down","!",MB_OK);		
		/*engine->RestartEngine();*/
	}
}

void TTSEngine::PauseEngine()
{
	try{
    SuspendThread(threads[0]);
	//CloseHandle(threads[0]);
	SuspendThread(threads[1]);		
	//CloseHandle(threads[1]);
	}catch(exception exc){}
}

void TTSEngine::RestartEngine()
{
	try{
	if(threads[0] != NULL)
	{
		/**/
		// If the function succeeds, the return value is nonzero
		if(TerminateThread(threads[0], 0) == 0)
		{
			//MessageBox(NULL,"Producer thread could not be terminated...","Error",0);
			//exit(0);
		}
		/**
		else
			MessageBox(NULL,"Producer thread terminated...","Info",0);
		/**/
	
		/**/
		// an error occured if CloseHandle returns 0
		if(CloseHandle(threads[0]) == 0)
		{
			//MessageBox(NULL,"Error closing producer handle...","Error",0);
			//exit(0);
		}
		/**
		else
			MessageBox(NULL,"Producer handle closed...","Info",0);
		/**/
	
	}
	if(threads[1] != NULL)
	{
		// If the function succeeds, the return value is nonzero
		/**/
		if(TerminateThread(threads[1], 0) == 0)
		{
			//MessageBox(NULL,"Consumer thread could not be terminated...","Error",0);
			//exit(0);
		}
		/**/
		// an error occured if CloseHandle returns 0
		if(CloseHandle(threads[1]) == 0)
		{
			//MessageBox(NULL,"Error closing consumer handle...","Error",0);
			//exit(0);
		}
		/**
		else
			MessageBox(NULL,"Consumer handle closed...","Info",0);
		/**/
	}
	
	// make the threads null for later control
	if(threads[0] != NULL)
		threads[0] = NULL;
	if(threads[1] != NULL)
		threads[1] = NULL;
			
	queue->EmptyQueue();
	synthesizer->StopSynthesizer();
	ReleaseSemaphore(mutex, 1, NULL);
	ReleaseSemaphore(stopMutex, 1, NULL);
	}catch(exception exc){}
}


void TTSEngine::SetPuncRead(bool flag)
{
	parser->SetPuncRead(flag);
}