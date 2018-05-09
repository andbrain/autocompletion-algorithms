#include "itemTable.hpp"

using namespace trie;

unsigned int ItemTable_t::emplaceElement(const std::string& description_item){
  return this->m_itemMap.emplace(this->m_itemMap.size() ,description_item).first->first;
}

std::string ItemTable_t::getElement(unsigned int id){
  return this->m_itemMap.find(id)->second;
}

size_t ItemTable_t::tableSize(){
  return this->m_itemMap.size();
}

ItemTable_t::ItemTable_t(){

}
