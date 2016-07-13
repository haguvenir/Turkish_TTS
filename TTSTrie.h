#ifndef _TTS_TRIE_H
#define _TTS_TRIE_H

#include "TTSIncludes.h"

///////////////////////////////////////////
//////////Defines For TTSTrie//////////////
///////////////////////////////////////////
#define TTS_TRIE_NODE_ELEMENT_COUNT 35 //in TTSTrie class, element count in each node
#define TTS_TRIE_DATA_FILE "veriler\\index.dat"


/**
 * Type Definition for a TTSTrie element. 
 * Each TTSTrie element has three members
 *  ch: represented character
 *  fileOffset: File offset of the sound data for the phone/diphone/triphone path 
 *              that starts from a root element and ends at the current node element.
 *  size: Size of the sound data for for the phone/diphone/triphone path 
 *        that starts from a root element and ends at the current node element.
 */
typedef struct _TTSTrieElement{
  int fileOffset;
  int size;
}TTSTrieElement;

/**
 * Type definition for a TTSTrie node. 
 * Each node keeps elements. A path from a root element to any node element 
 * represents a diphone or triphone.
 */
typedef struct _TTSTrieNode {
  TTSTrieElement elements[TTS_TRIE_NODE_ELEMENT_COUNT];
}TTSTrieNode;

/**
 * TTSTrie class is a representation of a trie data structure that is used for TTS engine. 
 * TTSTrie is indexed by numbers which represent the characters in Turkish. 
 * The depth of the Trie is 3 and fixed. 3 represents number of 
 * letters in a phone/diphone/triphone path.
 */
class TTSTrie {
  
private:

  TTSTrieNode rootNode; 
  TTSTrieNode level1Nodes[TTS_TRIE_NODE_ELEMENT_COUNT];
  TTSTrieNode level2Nodes[TTS_TRIE_NODE_ELEMENT_COUNT][TTS_TRIE_NODE_ELEMENT_COUNT];
  TTSTrieNode level3Nodes[TTS_TRIE_NODE_ELEMENT_COUNT][TTS_TRIE_NODE_ELEMENT_COUNT][TTS_TRIE_NODE_ELEMENT_COUNT];

  void InitTrie();
  void InitTrieFromFile();

public:

  TTSTrie();
  ~TTSTrie();
  int Char2Pos(char c);
  TTSTrieElement * GetTrieElement(string item);
  void SetTrieElement(string item, TTSTrieElement TElement);    
  bool exists(string item);
};
#endif