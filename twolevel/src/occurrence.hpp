#pragma once
#include <list>

namespace trie {


  class Occurrence_t {
    unsigned int m_positionOnItem;
    unsigned int m_itemRef;
  public:
    Occurrence_t(unsigned int, unsigned int);
    unsigned int getPosition();
    unsigned int getRef();

    void setPosition(unsigned int);
  };

  struct OccurrenceComparator_t{
		const bool operator()(Occurrence_t*, Occurrence_t*);
	};

	struct OccurrenceComparator2_t{
		const bool operator()(Occurrence_t*, Occurrence_t*);
	};
}
