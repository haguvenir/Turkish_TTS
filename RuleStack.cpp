#include "TextStruct.h"

RuleStack::RuleStack(int size, int ruleNo)
{
	size = size > 0 ? size : 10;
	top = -1;
	ruleno = ruleNo;
	stackPtr = new int[size];
}
RuleStack::~RuleStack()
{
	delete [] stackPtr;
}

void RuleStack::MakeEmpty()
{
	top = -1;
}

void RuleStack::Push(int data)
{
	if(isFull())
		Resize(size+10);
	stackPtr[++top] = data;
}

int RuleStack::Pop()
{
	if(!isEmpty())
		return stackPtr[top--];
	return -1;
}

const int RuleStack::TopE()
{
	if(!isEmpty())
		return stackPtr[top];
	return -1;
}
void RuleStack::Resize(int nsize)
{
	if(nsize < size) return;

	int * newarray = new int[nsize];
	for(int i=0; i<=top; i++)
		newarray[i] = stackPtr[i];
	delete [] stackPtr;
	stackPtr = newarray;
}
void RuleStack::Display()
{
	for(int i=top; i>=0; i--)
		cout<<stackPtr[i]<<endl;
}