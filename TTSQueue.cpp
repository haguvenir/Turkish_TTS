#include "TextStruct.h"


TTSQueue::TTSQueue()
{
	isBusy	= false;
	header	= NULL;
	last	= NULL;
	size	= 0;
	mutex = CreateSemaphore(NULL, 1, 1, "mutex");
	full = CreateSemaphore(NULL, 0, QUEUE_SIZE, "full");
	empty = CreateSemaphore(NULL, QUEUE_SIZE, QUEUE_SIZE, "empty");

}

TTSQueue::~TTSQueue()
{
	CloseHandle(mutex);
	CloseHandle(empty);
	CloseHandle(full);

	EmptyQueue();
}

void TTSQueue::EmptyQueue()
{
	WaitForSingleObject(mutex, INFINITE);
	//this->header = NULL;
	Word *todel = header;
	while(header != NULL)
	{
		header = header->next;
		delete todel;
		todel = header;
	}
	header = NULL;
	last = NULL;
	size = 0;
	ReleaseSemaphore(mutex, 1, NULL);
}

void TTSQueue::Enqueue(Word * word)
{
	WaitForSingleObject(empty, INFINITE);
	WaitForSingleObject(mutex, INFINITE);

	size++;
	if(header == NULL)
	{
		header	= word;
		last	= word;
	}
	else
	{
		last->next	= word;
		last		= word;
	}

	ReleaseSemaphore(mutex, 1, NULL);
	ReleaseSemaphore(full, 1, NULL);
}

Word * TTSQueue::Dequeue()
{

	if(header == NULL) 
		return NULL;

	WaitForSingleObject(full, INFINITE);
	WaitForSingleObject(mutex, INFINITE);

	Word * rt = NULL;
	size--;
	rt = header;
	header = header->next;
	
	ReleaseSemaphore(mutex, 1, NULL);
	ReleaseSemaphore(empty, 1, NULL);

	return rt;
}

void TTSQueue::Display()
{
	Word * word = header;
	while(word != NULL)
	{
		word->Display();
		word = word->next;
	}
}

void TTSQueue::DisplayS()
{
	Word * word = header;
	while(word != NULL)
	{
		word->DisplayS();
		word = word->next;
	}
}

void TTSQueue::DisplayP()
{
	Word * word = header;
	while(word != NULL)
	{
		word->DisplayP();
		word = word->next;
	}
}


