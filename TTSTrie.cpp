//OK--//unsigned char turkce char icin
/**
* Implementation of TTSTrie class
*/

#include "TTSTrie.h"

/**
* TTSTrie:	Constructor
* Action: Initializes TTSTrie object with default values, sets	characters
*		   for each	element	in the trie	nodes, sets	offset and size	to -1.
* Parameters: -
* Returns:	-
* Comments: -
*/
TTSTrie::TTSTrie() {
	InitTrieFromFile();
} 

TTSTrie::~TTSTrie()	{
} 



int	TTSTrie::Char2Pos(char c){
	//turkish	characters are represented as negative numbers
	//so check if	letter is Turkish
	if((int)(c) <	0) {
		switch(c) {
		  case 'ç':	return 26;
		  case 'ð':	return 27;
		  case 'ý':	return 28;
		  case 'ö':	return 29;
		  case 'þ':	return 30;
		  case 'ü':	return 31;
		  case 'â':	return 32;
		  case 'î':	return 33;
		  case 'û':	return 34;
		}	 
	}	else {	  //if english letter then put as it is
		return (int(c) - int('a'));
	}
}

/**
* InitTrieFromFile:
* Action: Initializes TTSTrie object with values gathered from	TTS_TRIE_DATA_FILE,	
*		   sets	characters,	offsets	and	sizes.
* Parameters: -
* Returns:	-
* Comments: For now data file should have all data	in lowercase letters.
*/


void TTSTrie::InitTrie() {
	int	i, j, k, l;
	for(i=0;i<TTS_TRIE_NODE_ELEMENT_COUNT; i++) {
		rootNode.elements[i].fileOffset	= -1;
		rootNode.elements[i].size =	-1;
		for(j =	0; j< TTS_TRIE_NODE_ELEMENT_COUNT; j++)	{
			level1Nodes[i].elements[j].fileOffset	= -1;
			level1Nodes[i].elements[j].size =	-1;
			for(k =	0; k< TTS_TRIE_NODE_ELEMENT_COUNT; k++)	{
				level2Nodes[i][j].elements[k].fileOffset = -1;
				level2Nodes[i][j].elements[k].size = -1;	  
				for(l =	0; l< TTS_TRIE_NODE_ELEMENT_COUNT; l++)	{
					level3Nodes[i][j][k].elements[l].fileOffset	= -1;
					level3Nodes[i][j][k].elements[l].size =	-1;		 
				}
			}
		}
	}
}

void TTSTrie::InitTrieFromFile() {

	TCHAR	*fileName =	_T(TTS_TRIE_DATA_FILE);
	TTSTrieElement temp;	//temporary	TTSTrieElement
	FILE * fp	= NULL;
	TCHAR	input[256];

	int letterCount,//letter count in	a phone	path 
		result, 
		offset, 
		size;

	//opening	file
	if( (fp =	_tfopen(fileName, "r"))	==NULL ) 
	{
		TTSError::ShowError(TTSError::TTS_FILE_NOT_FOUND_ERR);
		return;
	}

	//getting	information	from file and initializing trie	elements in	each node
	do {
		result =  _ftscanf(fp, "%d", &letterCount);	//number of	letters	in the phone path
		if(result == EOF) break;
		_ftscanf(fp, "%s", input) ;	 //get phone/diphone/triphone
		_ftscanf(fp, "%d", &offset); //get offset
		_ftscanf(fp, "%d", &size);	 //get size

		//set offset and size value	for	element
		temp.fileOffset	= offset;
		temp.size =	size;

		//set element
		SetTrieElement(input,temp);

	}while(1);
	//finished, close	the	open doors pls.
	fclose(fp);

}//end InitFromFile()

/**
* GetTrieElement:
* Action: Retrieves a trie	element	for	given index	values.
* Parameters: 
rootIndex: Index number of the element at	root level 
level1Index: Index number	of the element at inner	level
level2Index: Index number	of the element at leaf level
***If the element is in	an inner level node, then level2Index is -1.
***If the element is in	root node, then	level2Index	and	level1Index	is -1.
* Returns:	Pointer	to the requested element.
* Comments: -
*/
TTSTrieElement * TTSTrie::GetTrieElement(string	item) {

	int	length = (int)item.length();

	if (length == 1) {
		return &(rootNode.elements[Char2Pos(item[0])]);
	} else if (length == 2)	{
		return &(level1Nodes[Char2Pos(item[0])].elements[Char2Pos(item[1])]);
	} else if (length == 3)	{
		return &(level2Nodes[Char2Pos(item[0])][Char2Pos(item[1])].elements[Char2Pos(item[2])]);
	} else if (length == 4)	{
		return &(level3Nodes[Char2Pos(item[0])][Char2Pos(item[1])][Char2Pos(item[2])].elements[Char2Pos(item[3])]);
	}else 
		return NULL;

}//end GetTrieElement()


/**
* SetTrieElement:
* Action: Sets	an element of given	indexes	according to a new given element
* Parameters: 
rootIndex: Index number of the element at	root level 
level1Index: Index number	of the element at inner	level
level2Index: Index number	of the element at leaf level
TElement:	New	element	for	given indexes.
***If the element is in	an inner level node, then level2Index is -1.
***If the element is in	root node, then	level2Index	and	level1Index	is -1.
* Returns:	-
* Comments: -
*/
void TTSTrie::SetTrieElement(string	item, TTSTrieElement TElement) {

	int	length = (int)item.length();

	if (length == 1) {
		rootNode.elements[Char2Pos(item[0])].fileOffset	= TElement.fileOffset;
		rootNode.elements[Char2Pos(item[0])].size =	TElement.size;
	} else if (length == 2)	{
		level1Nodes[Char2Pos(item[0])].elements[Char2Pos(item[1])].fileOffset =	TElement.fileOffset;
		level1Nodes[Char2Pos(item[0])].elements[Char2Pos(item[1])].size	= TElement.size;
	} else if (length == 3)	{
		level2Nodes[Char2Pos(item[0])][Char2Pos(item[1])].elements[Char2Pos(item[2])].fileOffset = TElement.fileOffset;
		level2Nodes[Char2Pos(item[0])][Char2Pos(item[1])].elements[Char2Pos(item[2])].size = TElement.size;
	} else if (length == 4)	{
		level3Nodes[Char2Pos(item[0])][Char2Pos(item[1])][Char2Pos(item[2])].elements[Char2Pos(item[3])].fileOffset	= TElement.fileOffset;
		level3Nodes[Char2Pos(item[0])][Char2Pos(item[1])][Char2Pos(item[2])].elements[Char2Pos(item[3])].size =	TElement.size;
	}

}//end SetTrieElement()

/**
* TTS Trie	node search	method,
*	searches the nodes of the Trie and returns true/false
*	according to the existance of the input
*/
bool TTSTrie::exists(string	item)
{
	int	length = (int)item.length();

	if (length == 1) {
		return rootNode.elements[Char2Pos(item[0])].size > 0;
	} else if (length == 2)	{
		return level1Nodes[Char2Pos(item[0])].elements[Char2Pos(item[1])].size > 0;
	} else if (length == 3)	{
		return level2Nodes[Char2Pos(item[0])][Char2Pos(item[1])].elements[Char2Pos(item[2])].size >	0;
	} else if (length == 4)	{
		return level3Nodes[Char2Pos(item[0])][Char2Pos(item[1])][Char2Pos(item[2])].elements[Char2Pos(item[3])].size > 0;
	}else 
		return false;
}