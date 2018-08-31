#pragma once
#include "itemTable.hpp"
#include "trieNode.hpp"
#include <set>
#include <vector>

namespace trie {

  struct ActiveNode_t {
    TrieNode_t* node;
    mutable int editDistance;
    ActiveNode_t(TrieNode_t*, int);
    bool operator<(const ActiveNode_t&) const;
  };

  struct ActiveNodeComparator_t {
    bool operator()(const ActiveNode_t&, const ActiveNode_t&) const;
  };

  class Trie_t {

    // std::vector<unsigned int> m_characterMap; // used to map all the characters to it's defined codes
    TrieNode_t* m_lambdaNode; // used to indicate the first node
    std::vector<char> m_reverseCharacterMap; // used to map all the defined codes to it's characters (4fun)
    std::set<ActiveNode_t> m_activeNodeSet; // uset to save the main activeNode set
    std::set<std::string> m_stopWords;
    int m_searchLimitThreshold; // threshold used for delimit the answers amount (default : 5)
    int m_fuzzyLimitThreshold; // threshold used for delimit the edit distance from node (default : 1)
    // void encodeCharacters(const std::string&); // used to start the encoding/decoding vector of  the characters contained on the file
    std::set<ActiveNode_t> buildNewSet(std::set<ActiveNode_t>&, char);
    unsigned int editDistance(const std::string&, const std::string&);
    ItemTable_t m_itemTable;

  public:

    Trie_t(); // default constructor, receiving the filename which wordmap will be readen
    void printTrie(); // print the tree { see the tree on http://mshang.ca/syntree/ =) }
    void addStopWords(const std::string&);
    bool isStopWord(std::string);
    void putWord(const std::string&, unsigned int); // include a string in the trie structure
    void setSearchLimitThreshold(int); // set the m_searchLimitThreshold
    void setFuzzyLimitThreshold(int); // set the m_fuzzyLimitThreshold
    void buildActiveNodeSet(); // used to build the activeNode set at once
    // std::vector<std::string> searchExactKeyword(const std::string&);
    std::vector<Occurrence_t> searchSimilarKeyword(const std::string&);
    std::vector<Occurrence_t> searchSecondLevel(std::vector<Occurrence_t>&, const std::string&);
    std::string getItemOnTable(unsigned int);
    unsigned int insertItemOnTable(const std::string&);
  };
}
