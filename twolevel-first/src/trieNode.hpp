#pragma once
#include "occurrence.hpp"
#include <list>
#include <map>
#include <vector>

namespace trie {
  class TrieNode_t {
    std::map<char, TrieNode_t*> m_childrenMap; // used to save all the children (access / insertion O(log n))
    char m_content; // used to store the current character code in it's structure
    bool m_endOfWord; // used to classify this node as a "WK-end-of-word" scenario
    std::list<Occurrence_t>* m_occurrences;

  public:
    TrieNode_t(); // default constructor
    TrieNode_t(char); // it puts the code
    char getContent(); // used to return the content
    std::vector<TrieNode_t*> getChildren(); // it gets a vector containing all the childrem of this node
    TrieNode_t* insertNReturnChild(char); // it inserts a children with this code and returns it
    TrieNode_t* getChild(char); // retrieves and returns the relative child
    bool isEndOfWord(); // check if it's the end of a word
    void setEndOfWord(bool); // set whether it's a end of word or not
    void buildOccurrences();
    void addOccurrence(unsigned int);
    const std::list<Occurrence_t>* getOccurences();
  };
}
