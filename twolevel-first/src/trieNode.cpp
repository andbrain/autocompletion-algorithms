#include "trieNode.hpp"
#include <iostream>

using namespace trie;

// TrieNode_t public methods
TrieNode_t::TrieNode_t(char val)
{
  this->m_content = val;
  this->m_endOfWord = false;
}

TrieNode_t::TrieNode_t()
{
  TrieNode_t(0);
}

std::vector<TrieNode_t*> TrieNode_t::getChildren()
{
  std::vector<TrieNode_t*> nodes;
  for (auto cur : this->m_childrenMap) {
    nodes.push_back(cur.second);
  }
  return nodes;
}

TrieNode_t* TrieNode_t::insertNReturnChild(char value)
{
  TrieNode_t* node = new TrieNode_t(value);
  this->m_childrenMap.insert({ value, node });
  return node;
}

TrieNode_t* TrieNode_t::getChild(char value)
{
  auto child = this->m_childrenMap.find(value);
  if (child != this->m_childrenMap.end()) {
    return child->second;
  }
  return nullptr;
}

char TrieNode_t::getContent()
{
  return this->m_content;
}

void TrieNode_t::setEndOfWord(bool isEnd)
{
  this->m_endOfWord = isEnd;
}

bool TrieNode_t::isEndOfWord()
{
  return this->m_endOfWord;
}

void TrieNode_t::buildOccurrences(){
  this->m_occurrences = new std::list<Occurrence_t>();
}

void TrieNode_t::addOccurrence(unsigned int reg)
{

  this->m_occurrences->emplace(this->m_occurrences->begin(), reg);
  // std::cout << "at position " << (int)a->getPosition() << "( stb : " << (int)position<<")" << "refering to register " << (int)a->getRef() <<"( stb : " << reg<<")"<< '\n';
}

const::std::list<Occurrence_t>*  TrieNode_t::getOccurences()
{
  return this->m_occurrences;
}
