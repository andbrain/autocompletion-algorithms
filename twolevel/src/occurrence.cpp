#include "occurrence.hpp"

using namespace trie;

Occurrence_t::Occurrence_t(unsigned int ref,  unsigned int position) : m_positionOnItem(position), m_itemRef(ref){

}

const bool OccurrenceComparator_t::operator()(Occurrence_t* o1, Occurrence_t* o2){
	return o1->getRef() < o2->getRef();
}

const bool OccurrenceComparator2_t::operator()(Occurrence_t* o1, Occurrence_t* o2){
	return o1->getPosition() > o2->getPosition();
}

unsigned int Occurrence_t::getPosition(){
  return this->m_positionOnItem;
}

void Occurrence_t::setPosition(unsigned int position){
  this->m_positionOnItem = position;
}

unsigned int Occurrence_t::getRef(){
  return this->m_itemRef;
}
