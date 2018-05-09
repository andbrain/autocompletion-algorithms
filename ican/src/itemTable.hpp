#pragma once
#include <algorithm>
#include <map>
namespace trie {

class ItemTable_t {
    std::map<unsigned int, std::string> m_itemMap;
public:
    size_t tableSize();
    unsigned int emplaceElement(const std::string&);
    std::string getElement(unsigned int);
    ItemTable_t();
};
}
