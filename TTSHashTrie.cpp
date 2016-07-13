
#include "TextStruct.h"

TTSHashTrie::TTSHashTrie()
{
	root = Create(LEAF_NODE);
}

TTSHashTrie::~TTSHashTrie()
{
	DeleteAll();
}



void TTSHashTrie::Delete(TTSHashTrieNode *node)
{
	if (node->type == LEAF_NODE)
	{
		delete [] node->data;
		delete [] node->info;
		delete node;
	} else
	{
		Delete(node->left);
		Delete(node->right);
		delete [] node->data;
		delete [] node->info;
		delete node;
	}
}

void TTSHashTrie::DeleteAll()
{
	if (root != NULL)
		Delete(root);
}

void TTSHashTrie::Insert(string str, string data)
{
	Insert(root,str,data);
}

void TTSHashTrie::Display()
{
	cout<<"\nHash-TTSHashTrie\n--------"<<endl;
	Display(root);
}

string TTSHashTrie::Exists(string str)
{
	return Exists(str,root);
}



/*
 * Recursive private methods that the users cannot directly call
 */
string TTSHashTrie::Exists(string str, TTSHashTrieNode *node)
{
	if(node->type == LEAF_NODE)
	{
		// AT leaf
		for(int i=0; i<node->elements; i++)
			if(str == node->info[i])
				return node->data[i];
		return "";
	}
	if (strcmp(&str.substr(0,node->index)[0],&node->compare[0]) < 1)
	{	// GO left
		if(node->type == INTERNAL_NODE) 
			return Exists(str,node->left);
	}
	else
	{	// GO right
		if(node->type == INTERNAL_NODE)
			return Exists(str,node->right);
	}
	return "";
}

void TTSHashTrie::Display(TTSHashTrieNode *node)
{
	if(node == NULL)
	{
		if(DEBUG_0) cout<<"null"<<endl; 
		return; 
	}
	if(node->type ==  INTERNAL_NODE)
	{
		if(DEBUG_0) cout<<"<<"<<endl;
		Display(node->left);
		if(DEBUG_0) cout<<"root: "<<node->index<<endl;
		if(DEBUG_0) cout<<">>"<<endl;
		Display(node->right);
	}
	else
	{
		for(int i=0; i<node->elements; i++)
			cout<<node->info[i]<<endl;
	}
}

void TTSHashTrie::Insert(TTSHashTrieNode *node, string str, string data)
{
	
	// case of root
	if(node->type == LEAF_NODE)
	{
		int i=0;
		for(i=node->elements; i>=1; i--)
		{
			if(Compare(str,node->info[i-1]) != 1)
			{
				node->info[i] = node->info[i-1];
				node->data[i] = node->data[i-1];
			}
			else break;
		}
		node->info[i] = str;
		node->data[i] = data;
		node->elements++;
		Check(node);
		return;
	}
	
	if (strcmp(&str.substr(0,node->index)[0],&node->compare[0]) < 1)
	{	// left
		if(node->type == INTERNAL_NODE) 
			Insert(node->left,str,data);
	}
	else
	{	// right
		if(node->type == INTERNAL_NODE)
			Insert(node->right,str,data);
	}
}

void TTSHashTrie::Check(TTSHashTrieNode * node)
{
	if(node->elements <= HASH_TRIE_BUFFER) return;
	//if(DEBUG_0) cout<<"**** Overflow"<<endl;
	
	int lower = HASH_TRIE_BUFFER/2;
	node->index ++;
	node->compare = node->info[lower].substr(0,node->index);
	node->left  = Create(LEAF_NODE);
	
	for(int ii=0; ii<HASH_TRIE_BUFFER+1; ii++)
		if( Compare(node->info[ii].substr(0,node->index), node->compare) == 1)
		{ // insert to right
			if(node->right == NULL)
				node->right = Create(LEAF_NODE);
			node->right->info[node->right->elements] = node->info[ii];
			node->right->data[node->right->elements] = node->data[ii];
			node->right->elements++;
		}
		else
		{ // insert to left
			node->left->info[node->left->elements] = node->info[ii];
			node->left->data[node->left->elements] = node->data[ii];
			node->left->elements++;
		}
	if(node->left->elements > HASH_TRIE_BUFFER)
	{
//		if(DEBUG_0) cout<<"**** OLMADI bir DAHA"<<endl;
		Check(node);
		return;
	}
	node->elements	= 0;
	node->type		= INTERNAL_NODE;
}

TTSHashTrieNode * TTSHashTrie::Create(TRIE_NODE_TYPE type)
{
	TTSHashTrieNode * node = new TTSHashTrieNode();
	node->data		= new string[HASH_TRIE_BUFFER+1];
	node->info		= new string[HASH_TRIE_BUFFER+1];
	node->elements	= 0;
	node->left		= NULL;
	node->right		= NULL;
	node->compare	= '\0';
	node->index		= 0;
	node->type		= type;
	return node;
}

int Compare(string str, string str2)
{
	int i=0;
	int len1 = (int)str.length();
	int len2 = (int)str2.length();

	while(str[i] != '\0' && str2[i] != '\0')
	{
		if(str[i] < str2[i])
			return -1;
		if(str[i] > str2[i])
			return 1;
		i++;
	}
	if(len1 == len2) return 0;
	if(len1 < len2) return -1;
	else return 1;
}