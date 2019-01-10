#pragma once
#include <algorithm>
#include <map>
namespace trie {

class ItemTable_t {
    std::map<unsigned int, std::string> m_itemMap;
public:
    // unsigned int emplaceElement(unsigned int, const std::string&);
    unsigned int emplaceElement(const std::string&);
    std::string getElement(unsigned int);
    ItemTable_t();
};
}
