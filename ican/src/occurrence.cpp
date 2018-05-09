#include "occurrence.hpp"

using namespace trie;

Occurrence_t::Occurrence_t(unsigned int ref,  char position) : itemRef(ref), positionOnItem(position) ,weight(0){

}

bool Occurrence_t::operator<(const Occurrence_t& par1) const{
  return this->itemRef < par1.itemRef;
}


const bool OccurrencePriorityComparator_t::operator()(const Occurrence_t& o1,const Occurrence_t& o2) const{
  return (o1.weight << o1.positionOnItem) > (o2.weight << o2.positionOnItem) ;
}
