#include "trie.hpp"
#include "occurrence.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace trie;

// ActiveNode_t public
ActiveNode_t::ActiveNode_t(TrieNode_t* nd, int ed)
: node(nd)
, editDistance(ed)
, positionDistance(0)
{
}

ActiveNode_t::ActiveNode_t(TrieNode_t* nd, int ed, int pos)
: node(nd)
, editDistance(ed)
, positionDistance(pos)
{
}

bool ActiveNode_t::operator<(const ActiveNode_t& anode) const
{
  return this->node < anode.node;
}

bool ActiveNodeComparator_t::operator()(const ActiveNode_t& n1, const ActiveNode_t& n2) const
{
  return n1.editDistance > n2.editDistance;
}

// Trie_t private methods
void Trie_t::buildActiveNodeSet()
{
  std::queue<std::pair<TrieNode_t*, int>> seekQueue;
  std::unordered_map<TrieNode_t*, TrieNode_t*> father;
  std::set<TrieNode_t*> visited;

  seekQueue.emplace(this->m_lambdaNode, 0); // add the lambdaNode to the seek
  m_activeNodeSet.emplace(this->m_lambdaNode, 0); // add the lambdaNode to the activeSet
  father.emplace(this->m_lambdaNode, nullptr); // set lambdaNode father as nullptr

  /* initialization */

  while (!seekQueue.empty()) {
    auto currentNode = seekQueue.front(); // get currentNode
    seekQueue.pop();

    for (TrieNode_t* child : currentNode.first->getChildren()) {
      // verify if the child hasn't been visited recently
      if (visited.find(child) == visited.end()) {
        int currentLevel = currentNode.second + 1;

        visited.emplace(child); // add child to the visited set
        father.emplace(child, currentNode.first); // set current vertex as father

        if (currentLevel < m_fuzzyLimitThreshold) { // add the child on the seek only if it has a threshold < fuzzy
          seekQueue.emplace(child, currentLevel);
        }

        if (child->isEndOfWord()) { // verify if this child is end of word
          TrieNode_t* currentChild = child;
          m_activeNodeSet.emplace(currentChild, currentLevel); // add the child to the activeSet

          auto currentChild_it = father.find(child);

          // add all the father's father's father's ... father's of this child to the activeSet
          while (currentChild_it->second != nullptr) {

            if (m_activeNodeSet.emplace(currentChild_it->second, --currentLevel).second) {
              currentChild_it = father.find(currentChild_it->second);
            } else {
              break;
            }
          }
        }
      }
    }
  }
}

// void Trie_t::encodeCharacters(const std::string& filename)
// {

//   int lineCode = 1;
//   std::string currentLine;
//   std::ifstream fileInputStream(filename);

//   while (std::getline(fileInputStream, currentLine)) {
//     m_reverseCharacterMap[lineCode] = currentLine.back();
//     for (char currentChar : currentLine) {
//       m_characterMap[currentChar] = lineCode;
//     }
//     lineCode++;
//   }

//   m_reverseCharacterMap[0] = '#'; // lambda character
// }

void Trie_t::addStopWords(const std::string& filename)
{
  std::string currentLine;
  std::ifstream fileInputStream(filename);

  // std::cout << filename << std::endl;

  while (std::getline(fileInputStream, currentLine)) {
    std::transform(currentLine.begin(), currentLine.end(), currentLine.begin(), ::tolower);
    // std::cout << currentLine << std::endl;
    m_stopWords.insert(currentLine);
  }
}

bool Trie_t::isStopWord(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return m_stopWords.find(str) != m_stopWords.end();
}

// Trie_t public methods

Trie_t::Trie_t()
// : m_characterMap(255, '$')
: m_reverseCharacterMap(255, '$')
, m_searchLimitThreshold(5)
, m_fuzzyLimitThreshold(1)
{
  this->m_lambdaNode = new TrieNode_t(0);
  // this->encodeCharacters(filename);
}

void Trie_t::printTrie()
{
  TrieNode_t* currentNode;
  std::set<TrieNode_t*> visited;

  std::stack<TrieNode_t*> nodeStack;
  nodeStack.push(this->m_lambdaNode);

  while (!nodeStack.empty()) {
    currentNode = nodeStack.top();

    if (visited.find(currentNode) == visited.end()) {
      visited.insert(currentNode);
      std::cout << "[" << this->m_reverseCharacterMap[currentNode->getContent()] << (currentNode->isEndOfWord() ? "'" : " ");
      std::vector<TrieNode_t*> children = currentNode->getChildren();

      while (!children.empty()) {
        nodeStack.push(children.back());
        children.pop_back();
      }

    } else {
      nodeStack.pop();
      std::cout << "]";
    }
  }
}

std::string Trie_t::getItemOnTable(unsigned int id)
{
  return this->m_itemTable.getElement(id);
}

unsigned int Trie_t::insertItemOnTable(const std::string& str)
{
  return this->m_itemTable.emplaceElement(str);
}

// void Trie_t::putFile(const std::string& filename)
// {

//   struct PreprocessWord_Bundle_t {
//     TrieNode_t* node;
//     std::list<std::pair<char, unsigned int>> posRefList;
//     PreprocessWord_Bundle_t(TrieNode_t* _node, char _pos, unsigned int _ref)
//     : node(_node)
//     {
//       posRefList.emplace_back(_pos, _ref);
//     }
//   };

//   std::string currentLine;
//   std::string word;
//   std::ifstream fileInputStream(filename);
//   std::map<std::string, PreprocessWord_Bundle_t> stringBundle;

//   while (std::getline(fileInputStream, currentLine)) {

//     while (std::getline(fileInputStream, currentLine)) {
//       std::stringstream wordStream(currentLine);
//       char position = 1;
//       unsigned int ref = this->insertItemOnTable(currentLine);
//       while (std::getline(wordStream, word, ' ')) {

//         if (!this->isStopWord(word)) {

//           std::map<std::string, PreprocessWord_Bundle_t>::iterator it;

//           if ((it = stringBundle.find(word)) != stringBundle.end()) {
//             it->second.posRefList.emplace_back(position, ref);
//           } else {
//             // from https://stackoverflow.com/questions/14075128/mapemplace-with-a-custom-value-type
//             stringBundle.emplace(std::piecewise_construct, std::make_tuple(word), std::make_tuple(putNReturnWord(word), position, ref));
//           }
//         }
//         position++;
//       }
//     }
//   }

//   for(std::map<std::string, PreprocessWord_Bundle_t>::iterator it = stringBundle.begin() ; it != stringBundle.end() ; it++){
//     // std::cout << "Keyword \"" << it->first << "\"\n";
//     for(auto prList : it->second.posRefList){
//       // std::cout << "\tPosition : " << (int)ref.first << " / Ref : " << ref.second <<  " / Phrase : \"" << m_itemTable.getElement(ref.second) <<"\"\n";
//       it->second.node->addOccurrence(prList.first, prList.second);
//     }
//   }

// }

TrieNode_t* Trie_t::putNReturnWord(const std::string& str)
{
  TrieNode_t *currentRoot, *lastRoot;
  currentRoot = this->m_lambdaNode;
  for (char c : str) {

    // unsigned int code = this->m_characterMap[c];


    // if(code >= m_characterMap.size()){
    //   std::cout << "Warning : the word \"" << str << "\" contains unrecognized characters\n";
    // }

    lastRoot = currentRoot;
    currentRoot = currentRoot->getChild(c);
    if (!currentRoot) {
      currentRoot = lastRoot->insertNReturnChild(c);
    }
  }

  if (currentRoot != this->m_lambdaNode) {

    if (!currentRoot->isEndOfWord()) {
      currentRoot->buildOccurrences();
      currentRoot->setEndOfWord(true);
    }

    return currentRoot;
  }

  return nullptr;
}

void Trie_t::putWord(const std::string& str, char position, unsigned int reference)
{
  TrieNode_t *currentRoot, *lastRoot;
  currentRoot = this->m_lambdaNode;
  for (char c : str) {

    // unsigned int code = this->m_characterMap[c];

    // if(code >= m_characterMap.size()){
    //   std::cout << "Warning : the word \"" << str << "\" contains unrecognized characters\n";
    // }

    lastRoot = currentRoot;
    currentRoot = currentRoot->getChild(c);
    if (!currentRoot) {
      currentRoot = lastRoot->insertNReturnChild(c);
    }
  }

  if (currentRoot != this->m_lambdaNode) {

    if (!currentRoot->isEndOfWord()) {
      currentRoot->buildOccurrences();
      currentRoot->setEndOfWord(true);
    }
    currentRoot->addOccurrence(position, reference);
  }
}

std::set<ActiveNode_t> Trie_t::buildNewSet(std::set<ActiveNode_t>& set, char curChar)
{
  std::set<ActiveNode_t> activeNodeSet;

  /*
  ** Considering node N
  ** We can transform the content of this node for the currentWord by first adding a character. (Prebuilt Addiction)
  */
  // std::cout << "\nBuilding node for " << m_reverseCharacterMap[curChar] << '\n';
  for (auto curActiveNode = set.begin(); curActiveNode != set.end(); curActiveNode++) {
    if (curActiveNode->editDistance < m_fuzzyLimitThreshold) {
      // std::cout << " * Updating distance for node " << m_reverseCharacterMap[curActiveNode->node->getContent()] << " from " << curActiveNode->editDistance << " to " << curActiveNode->editDistance + 1 << '\n';
      activeNodeSet.emplace(curActiveNode->node, curActiveNode->editDistance + 1);
    }
  }

  /*
  ** Considering no+de P (child of N)
  ** -  C1. Different character from `curChar` : (Posbuilt Substitution Addiction)
  **    We can transform this character of P for the current character by substitution, relative to ED(N), then, we have to add P to
  **    the set if and only if ED(N)+1 [previously calculated and updated] < threshold
  **
  ** -  C2. Equals character from `curChar` : (Posbuilt Match Addiction)
  **    If this node character match with `curChar`, it means that we got to add it with ED(N) (indeed, when we got a match, we have ED=0) [1]
  **    Once doing it, we have to add the descendant nodes of P (called here `PChild`) with distance PChild = ED(P) + DeepthFrom(P, PChild) < threshold
  **
  ** Addendum [1] - I must update the distance only if the new distance is lower than the last distance (it happens when a previously father node have been calculated)
  */

  for (auto curActiveNode = set.begin(); curActiveNode != set.end(); curActiveNode++) {

    // std::cout << "\nScanning children of node " << m_reverseCharacterMap[curActiveNode->node->getContent()] << '\n';

    for (auto childOfcurActiveNode : curActiveNode->node->getChildren()) {
      // std::cout << " * Node " << m_reverseCharacterMap[childOfcurActiveNode->getContent()] << " found ";
      if (childOfcurActiveNode->getContent() != curChar) { // case 1
        // std::cout << " and haven't matched with the character,";
        if (curActiveNode->editDistance < m_fuzzyLimitThreshold) { // verify if ED(N)+1 < P
          // std::cout << " but it's edit distance can be increased and it would be added to the set";
          int newEditDistance = curActiveNode->editDistance + 1; // the new edit distance for the child

          // retrieves to add the child, return FALSE if it was not possible, cuz the child was already added before (so, i've to verify which edit distance is lower : the current or the previous?)
          auto childIteratorOnSet = activeNodeSet.emplace(childOfcurActiveNode, newEditDistance);

          // if the child was not added (it was cuz' there was a activeNode marked on this node before) and the last ED is bigger than the new ED
          if (!childIteratorOnSet.second) {
            if (childIteratorOnSet.first->editDistance > newEditDistance) {
              childIteratorOnSet.first->editDistance = newEditDistance; // guaranteed by addendum 1
            }
          }
        }
        // std::cout << '\n';
      } else { // case 2
        // std::cout << " and it matches with the character, so will be added to the set\n";
        // retrieves to add the child, return FALSE if it was not possible, cuz the child was already added before (so, i've to verify which edit distance is lower : the current or the previous?)
        auto childIteratorOnSet = activeNodeSet.emplace(childOfcurActiveNode, curActiveNode->editDistance);

        int currentChildDistance = childIteratorOnSet.first->editDistance;

        if (!childIteratorOnSet.second) { /* processor thanks! */
          if (childIteratorOnSet.first->editDistance > curActiveNode->editDistance) {
            currentChildDistance = childIteratorOnSet.first->editDistance = curActiveNode->editDistance;
          }
        }

        /*
        **  I've to fetch the children and the entire set of children to the active node if
        **  and only if the dist( father, getChildRecursive(father)) ) < threshold;
        */
        // std::cout << "\t\tIt's children will be verifieds to be added to the set : \n";

        std::queue<TrieNode_t*> toRecover; // a queue to save which node is on the way
        toRecover.push(childIteratorOnSet.first->node); // adding the current matched node to the queue

        // while the distance is lesser than the limit and we got some node to recover...
        while (currentChildDistance < m_fuzzyLimitThreshold && !toRecover.empty()) {
          // recover the current node from the queue
          TrieNode_t*& currentNode = toRecover.front();
          // std::cout << "\t\tCurrent node : " << m_reverseCharacterMap[currentNode->getContent()] << '\n';
          // and update the distance to one more
          ++currentChildDistance;

          // for each child of the current node
          for (auto child : currentNode->getChildren()) {

            // we add this child to the active node set, once we can face it as a addiction operation inside the boundary imposed by the search
            auto currentChildIterator = activeNodeSet.emplace(child, currentChildDistance);
            // std::cout << "\t\t * Children " << m_reverseCharacterMap[currentChildIterator.first->node->getContent()] << " added to the set ";
            // if there was this child within the activeNode, we have to keep the minor operation distance
            if (!currentChildIterator.second) {
              // std::cout << "by update/permanence ";
              if (currentChildIterator.first->editDistance > currentChildDistance) {
                currentChildIterator.first->editDistance = currentChildDistance;
              }
            }
            // std::cout<<" with edit distance " << currentChildIterator.first->editDistance << '\n';

            // and put it if and only if the currentDistance is lesser than the thresould (the memory and processment thank!)
            if (currentChildDistance < m_fuzzyLimitThreshold) {
              toRecover.push(child);
            }
          }
          toRecover.pop();
        }
      }
    }
  }
  // std::cout << "\n\n";
  return activeNodeSet;
}

std::priority_queue<Occurrence_t, std::vector<Occurrence_t> , OccurrencePriorityComparator_t> Trie_t::searchSimilarKeyword(const std::string& keyword)
{
  std::set<Occurrence_t> lastAnswerSet;
  std::list< std::priority_queue<ActiveNode_t,std::vector<ActiveNode_t>,ActiveNodeComparator_t> > activeLists;
  std::string curKeyword;

  std::stringstream wordStream(keyword);

  while (std::getline(wordStream, curKeyword, ' ')) {
    std::set<ActiveNode_t> lastActiveNodes = this->m_activeNodeSet;
    for (char c : curKeyword) {
      // lastActiveNodes = buildNewSet(lastActiveNodes, m_characterMap[c]);
      lastActiveNodes = buildNewSet(lastActiveNodes, c);
    }


    auto currentList = activeLists.emplace(activeLists.end() ,std::priority_queue<ActiveNode_t,std::vector<ActiveNode_t>,ActiveNodeComparator_t>());
    for(auto node : lastActiveNodes){
      currentList->push(node);
    }
  }

  char iteration = 1;
  for (auto currentList : activeLists) {
    // std::cout << "\n\n";
    std::set<Occurrence_t> answerSet;
    std::unordered_set<TrieNode_t*> visitedNode;

    while(!currentList.empty()){

      auto aNode = currentList.top();
      currentList.pop();

      if (visitedNode.find(aNode.node) == visitedNode.end()) {
        std::queue<TrieNode_t*> pQueue;
        pQueue.push(aNode.node);

        while (!pQueue.empty()) {
          auto currentSeeker = pQueue.front();
          visitedNode.insert(currentSeeker);
          pQueue.pop();

          if (currentSeeker->isEndOfWord()) {
            for (auto action : (*currentSeeker->getOccurences())) {
              // std::cout << action.itemRef << " - " << (int)action.positionOnItem << " : " << getItemOnTable(action.itemRef) << '\n';
              action.weight = aNode.editDistance;
              action.positionOnItem=abs(action.positionOnItem - iteration);
              answerSet.insert(action);
            }
          }

          for (TrieNode_t* curChild : currentSeeker->getChildren()) {
            if (visitedNode.find(curChild) == visitedNode.end()) {
              pQueue.emplace(curChild);
            }
          }
        }
      }
    }//

    if (iteration > 1) {

      for(auto ans = lastAnswerSet.begin(); ans != lastAnswerSet.end();){
        auto iterator = answerSet.emplace(ans->itemRef, 0);
        // std::cout << "TRYING at " << getItemOnTable(ans->itemRef) << " - " << (int)ans->positionOnItem << " - " << (int) iterator.first->positionOnItem <<  '\n';
        if (iterator.second) {
          // std::cout << "ERASED at " << (int)ans->positionOnItem << " - " << (int) iterator.first->positionOnItem <<  '\n';
          ans = lastAnswerSet.erase(ans);
        }else{
          // std::cout << "MATCH at " << (int)ans->positionOnItem << " - " << (int) iterator.first->positionOnItem <<  '\n';
          ans->weight += iterator.first->weight;
          ans->positionOnItem += iterator.first->positionOnItem;
          ++ans;
        }

      }
    } else {
      lastAnswerSet = answerSet;
    }

    iteration++;
  }

  std::priority_queue<Occurrence_t, std::vector<Occurrence_t> , OccurrencePriorityComparator_t> ocurrencesQueue;

  for(auto ocur :lastAnswerSet){
    ocurrencesQueue.push(ocur);
  }

  return ocurrencesQueue;
}

// std::vector<std::string> Trie_t::searchExactKeyword(const std::string& keyword)
// {
//   std::vector<std::string> answerList;
//   TrieNode_t* currentNode = this->m_lambdaNode;
//   std::queue<TrieNode_t*> pQueue;
//   std::unordered_map<TrieNode_t*, std::vector<unsigned int>> keywords; // map that keeps the keywords for each node
//   keywords.reserve(this->m_searchLimitThreshold << 1);
//   std::vector<unsigned int> wordToTheNode; // local variable that absorvs the current string of the found node as vector of its char codes
//   std::vector<unsigned int> keywordMapped; // local variable to absorv the current string as vector of its char codes

//   for (char c : keyword) {
//     keywordMapped.push_back(this->m_characterMap[c]);
//   }

//   for (unsigned int currentCode : keywordMapped) {
//     currentNode = currentNode->getChild(currentCode);
//     wordToTheNode.push_back(currentCode);
//     if (currentNode == nullptr) {
//       break;
//     }
//   }

//   if (currentNode) {
//     keywords.emplace(currentNode, wordToTheNode);
//     pQueue.push(currentNode);
//   }

//   int threshold = this->m_searchLimitThreshold;

//   while (!pQueue.empty() && threshold) {
//     auto currentSeeker = pQueue.front();
//     pQueue.pop();

//     if (currentSeeker->isEndOfWord()) {
//       std::string currentWord;
//       for (unsigned int ch : keywords[currentSeeker]) {
//         currentWord.push_back(this->m_reverseCharacterMap[ch]);
//       }

//       answerList.push_back(currentWord); //transform string
//       threshold--;
//     }

//     for (TrieNode_t* curChild : currentSeeker->getChildren()) {
//       keywords.emplace(curChild, keywords[currentSeeker]);
//       keywords[curChild].push_back(curChild->getContent());
//       pQueue.emplace(curChild);
//     }
//   }

//   return answerList;
// }

void Trie_t::setSearchLimitThreshold(int limit)
{
  this->m_searchLimitThreshold = limit;
}

void Trie_t::setFuzzyLimitThreshold(int limit)
{
  this->m_fuzzyLimitThreshold = limit;
}
