#pragma once
#include <list>

namespace trie {

  struct Occurrence_t {
    unsigned int itemRef;
    mutable char positionOnItem;
    mutable int weight;
    Occurrence_t(unsigned int, char);
    Occurrence_t(unsigned int,char, unsigned int);
    bool operator<(const Occurrence_t&)const ;
  };

  struct OccurrencePriorityComparator_t{
    const bool operator()(const Occurrence_t&,const Occurrence_t&) const;
  };
}
