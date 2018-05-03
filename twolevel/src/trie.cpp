#include "trie.hpp"
#include "occurrence.hpp"
#include <climits>
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
  // std::set<TrieNode_t*> visited;

  seekQueue.emplace(this->m_lambdaNode, 0); // add the lambdaNode to the seek
  m_activeNodeSet.emplace(this->m_lambdaNode, 0); // add the lambdaNode to the activeSet
  father.emplace(this->m_lambdaNode, nullptr); // set lambdaNode father as nullptr

  /* initialization */

  while (!seekQueue.empty()) {
    auto currentNode = seekQueue.front(); // get currentNode
    seekQueue.pop();

    for (TrieNode_t* child : currentNode.first->getChildren()) {
      // verify if the child hasn't been visited recently
      // if (visited.find(child) == visited.end()) {
      int currentLevel = currentNode.second + 1;

      // visited.emplace(child); // add child to the visited set
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
      // }
    }
  }
  // std::cout << "\nActive Node Set : \n";
  // for (auto nod : m_activeNodeSet) {
  //   std::cout << " - Node : " << m_reverseCharacterMap[nod.node->getContent()] << '-' << nod.editDistance << '\n';
  // }
  // std::cout << '\n';
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

// Trie_t public methods

Trie_t::Trie_t()
// : m_characterMap(255, '$')
: m_reverseCharacterMap(255, '$')
, m_searchLimitThreshold(5)
, m_fuzzyLimitThreshold(1)
{
  this->m_lambdaNode = new TrieNode_t();
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

std::string Trie_t::getItemOnTable(unsigned int id){
  return this->m_itemTable.getElement(id);
}

unsigned int Trie_t::insertItemOnTable(const std::string& str)
{
  return this->m_itemTable.emplaceElement(str);
}

void Trie_t::putWord(const std::string& str, unsigned int position, unsigned int reference)
{
  TrieNode_t *currentRoot, *lastRoot;
  currentRoot = this->m_lambdaNode;
  for (char c : str) {
    // char code = this->m_characterMap[c];
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
    // std::cout << "\nInserting word " << str << " ";
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
    } else {
      // std::cout << " * Node " << m_reverseCharacterMap[curActiveNode->node->getContent()] << " will not be added to the set, cuz it's edit distance " << curActiveNode->editDistance << '\n';
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

std::vector<Occurrence_t> Trie_t::searchSimilarKeyword(const std::string& keyword)
{
  std::vector<Occurrence_t> answerList;
  std::set<ActiveNode_t> activeNodesReceived = m_activeNodeSet;
  std::unordered_set<TrieNode_t*> visitedNode;
  int threshold = this->m_searchLimitThreshold;

  for (char c : keyword) {
    // unsigned int curCharCode = m_characterMap[c];
    activeNodesReceived = buildNewSet(activeNodesReceived, c);
  }

  // std::set<ActiveNode_t, ActiveNodeComparator_t> activeSet;
  // activeSet.insert(activeNodesReceived.begin(), activeNodesReceived.end());
  std::priority_queue<ActiveNode_t, std::vector<ActiveNode_t>, ActiveNodeComparator_t> activeSet;

  for (auto node : activeNodesReceived) {
    activeSet.push(node);
  }
  //
  //
  // while(!activeSet.empty()){
  //   auto activeNode = activeSet.top();
  //   std::cout << "Node : " << m_reverseCharacterMap[activeNode.node->getContent()] << '-' << activeNode.editDistance << '\n';
  //   activeSet.pop();
  // }

  // for (auto aNode : activeNodes) {
  while (!activeSet.empty()) {

    auto aNode = activeSet.top();
    activeSet.pop();

    if (threshold > 0) {
      if (visitedNode.find(aNode.node) == visitedNode.end()) {
        if (aNode.node->isEndOfWord()) {
          threshold--;
          // answerList.push_back((*aNode.node->getString()));

          for (auto action : (*aNode.node->getOccurences())) {
            answerList.push_back(action);
            // std::cout << m_reverseCharacterMap[ aNode.node->getContent()] <<" - " << (int)action.getPosition() << '\n';
          }

          visitedNode.insert(aNode.node);
        } else {

          std::queue<TrieNode_t*> pQueue;
          pQueue.push(aNode.node);

          while (!pQueue.empty() && threshold) {
            auto currentSeeker = pQueue.front();
            visitedNode.insert(currentSeeker);
            pQueue.pop();

            if (currentSeeker->isEndOfWord()) {
              // answerList.push_back(*currentSeeker->getString()); //transform string
              for (auto action : (*currentSeeker->getOccurences())) {
                answerList.push_back(action);
                // std::cout << m_reverseCharacterMap[ currentSeeker->getContent()] <<" - " << (int)action.getPosition() << '\n';
              }
              threshold--;
            }

            for (TrieNode_t* curChild : currentSeeker->getChildren()) {
              if (visitedNode.find(curChild) == visitedNode.end()) {
                pQueue.emplace(curChild);
              }
            }
          }
        }
      }
    }
  }

  return answerList;
}

// std::vector<std::string> Trie_t::searchExactKeyword(const std::string& keyword)
// {
//   std::vector<std::string> answerList;
//   TrieNode_t* currentNode = this->m_lambdaNode;
//   std::queue<TrieNode_t*> pQueue;
//   std::unordered_map<TrieNode_t*, std::vector<unsigned int>> keywords; // map that keeps the keywords for each node
//   keywords.reserve(this->m_searchLimitThreshold << 1);
//   // std::vector<char> wordToTheNode; // local variable that absorvs the current string of the found node as vector of its char codes
//   // std::vector<unsigned int> keywordMapped; // local variable to absorv the current string as vector of its char codes

//   // for (char c : keyword) {
//   //   keywordMapped.push_back(c);
//   // }

//   for (char c : keyword) {
//     currentNode = currentNode->getChild(c);
//     // wordToTheNode.push_back(c);
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


unsigned int Trie_t::editDistance(const std::string& s1, const std::string &s2){
  const std::size_t len1 = s1.size(), len2 = s2.size();
  std::vector<unsigned int> col(len2+1), prevCol(len2+1);

  for (unsigned int i = 0; i < prevCol.size(); i++){
    prevCol[i] = i;
  }

  for (unsigned int i = 0; i < len1; i++) {
    col[0] = i+1;

    for (unsigned int j = 0; j < len2; j++)
    col[j+1] = std::min(prevCol[1 + j] + 1, std::min(col[j] + 1, prevCol[j] + (s1[i]==s2[j] ? 0 : 1) ));

    col.swap(prevCol);
  }  return prevCol[len2];
}


std::vector<Occurrence_t> Trie_t::searchSecondLevel(std::vector<Occurrence_t>& fuzzyWords, const std::string& realQuery){

  std::set<trie::Occurrence_t*, trie::OccurrenceComparator_t> found;

  for(Occurrence_t &ocurrence : fuzzyWords){

    std::string item = getItemOnTable(ocurrence.getRef());

    int distanceTotal = INT_MAX;

    unsigned int numWords;

    std::string currentQueryWord;
    std::stringstream queryStream(realQuery); //realQuery stream
    std::getline(queryStream, currentQueryWord, ' '); // get each word from the query (ignoring the first one)
    while(std::getline(queryStream, currentQueryWord, ' ')){

      numWords = 1;
      std::string currentItemWord;
      std::stringstream itemStream(item); // item of the current occurence
      while(std::getline(itemStream, currentItemWord, ' ')){ // currentItemWord

        transform(currentQueryWord.begin(), currentQueryWord.end(), currentQueryWord.begin(), ::tolower);
        transform(currentItemWord.begin(), currentItemWord.end(), currentItemWord.begin(), ::tolower);

        if(numWords != ocurrence.getPosition()){ // && currentItemWord.size() > 2

          int distance = INT_MAX;

          // if(queryStream.rdbuf()->std::streambuf::in_avail() != 0){
          //   distance = editDistance(currentQueryWord, currentItemWord);
          // }else{

            int abs = currentItemWord.size() - currentQueryWord.size();

            for(int i = 0; i<abs; i++){
              currentItemWord.pop_back();
            }

            distance = editDistance(currentQueryWord, currentItemWord);
          // }


          if(distance <= m_fuzzyLimitThreshold){
            // found.emplace(&ocurrence);
            auto it = found.emplace(&ocurrence);

            if(!it.second){
              if((*it.first)->getPosition() > ocurrence.getPosition()){
                (*it.first)->setPosition(ocurrence.getPosition());
              }
            }
          }

          if(distance < distanceTotal){
            distanceTotal = distance;
          }
        }

        numWords++;

      }
    }
    ocurrence.setPosition(distanceTotal);
  }

  std::vector<trie::Occurrence_t*> founded;

  while(!found.empty()){
    founded.push_back(*found.begin());
    found.erase(found.begin());
  }

  std::make_heap(founded.begin(), founded.end(), OccurrenceComparator2_t());

  std::vector<Occurrence_t> answerList;

  int c = 0;
  while(!founded.empty() && c < this->m_searchLimitThreshold){
    // std::cout << getItemOnTable(founded.front()->getRef()) << '\n';

    answerList.push_back(*founded.front());

    std::pop_heap(founded.begin(), founded.end(), OccurrenceComparator2_t());
    founded.pop_back();

    c++;

  }

  return answerList;

}

void Trie_t::setSearchLimitThreshold(int limit)
{
  this->m_searchLimitThreshold = limit;
}

void Trie_t::setFuzzyLimitThreshold(int limit)
{
  this->m_fuzzyLimitThreshold = limit;
}
