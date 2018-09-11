/*

   Copyright (C) 2011 by The Department of Computer Science and
   Technology, Tsinghua University

   Redistribution of this file is permitted under the terms of
   the BSD license.

Author   : Dong Deng
Created  : 2014-09-05 11:42:44 
Modified : 2014-11-02 23:03:54
Contact  : dd11@mails.tsinghua.edu.cn

*/

#include "Trie.h"
#include "ActiveNode.h"
#include <map>
#include <sys/time.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <stdlib.h>


vector<string> recs;
vector<string> queries;
unordered_map<unsigned int, string> m_table;

void readData(string& filename, vector<string>& recs) {
	string str;
	ifstream input(filename, ios::in);
	while (getline(input, str)) {
		for (auto i = 0; i < str.length(); i++)
		  str[i] = tolower(str[i]);
		recs.push_back(str);
	}
}

bool comp_lower(const pair<int, int> & p, const int v) {
	return p.first < v;
}

const vector<string> explode(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

unsigned int editDistance(const std::string& s1, const std::string &s2){
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

// std::vector<Occurrence_t> Trie_t::searchSecondLevel(std::vector<Occurrence_t>& fuzzyWords, const std::string& realQuery){

// 	std::set<trie::Occurrence_t*, trie::OccurrenceComparator_t> found;

// 	for(Occurrence_t &ocurrence : fuzzyWords){

// 		std::string item = getItemOnTable(ocurrence.getRef());

// 		int distanceTotal = INT_MAX;

// 		unsigned int numWords;

// 		std::string currentQueryWord;
// 		std::stringstream queryStream(realQuery); //realQuery stream
// 		std::getline(queryStream, currentQueryWord, ' '); // get each word from the query (ignoring the first one)
// 		while(std::getline(queryStream, currentQueryWord, ' ')){

// 			numWords = 1;
// 			std::string currentItemWord;
// 			std::stringstream itemStream(item); // item of the current occurence
// 			while(std::getline(itemStream, currentItemWord, ' ')){ // currentItemWord

// 				transform(currentQueryWord.begin(), currentQueryWord.end(), currentQueryWord.begin(), ::tolower);
// 				transform(currentItemWord.begin(), currentItemWord.end(), currentItemWord.begin(), ::tolower);

// 				// if(numWords != ocurrence.getPosition()){ // && currentItemWord.size() > 2

// 				int distance = INT_MAX;

// 				// if(queryStream.rdbuf()->std::streambuf::in_avail() != 0){
// 				//   distance = editDistance(currentQueryWord, currentItemWord);
// 				// }else{

// 				int abs = currentItemWord.size() - currentQueryWord.size();

// 				for(int i = 0; i<abs; i++){
// 					currentItemWord.pop_back();
// 				}

// 				distance = editDistance(currentQueryWord, currentItemWord);
// 				// }


// 				if(distance <= m_fuzzyLimitThreshold){
// 					// found.emplace(&ocurrence);
// 					auto it = found.emplace(&ocurrence);

// 					// if(!it.second){
// 					//   if((*it.first)->getPosition() > ocurrence.getPosition()){
// 					//     (*it.first)->setPosition(ocurrence.getPosition());
// 					//   }
// 					// }
// 				}

// 				// if(distance < distanceTotal){
// 				//   distanceTotal = distance;
// 				// }


// 				numWords++;

// 			}
// 		}
//     // ocurrence.setPosition(distanceTotal);
// 	}

// 	std::vector<trie::Occurrence_t*> founded;

// 	while(!found.empty()){
// 		founded.push_back(*found.begin());
// 		found.erase(found.begin());
// 	}

// 	// std::make_heap(founded.begin(), founded.end(), OccurrenceComparator2_t());

// 	std::vector<Occurrence_t> answerList;

// 	int c = 0;
// 	while(!founded.empty() && c < this->m_searchLimitThreshold){
// 		// std::cout << getItemOnTable(founded.front()->getRef()) << '\n';

// 		answerList.push_back(*founded.front());

// 		// std::pop_heap(founded.begin(), founded.end(), OccurrenceComparator2_t());
// 		founded.pop_back();

// 		c++;

// 	}

// 	return answerList;

// }

int main(int argc, char ** argv) {
	string filename = string(argv[1]);
	string queryfile = string(argv[2]);
	const int tau = atoi(argv[3]);
	const int tok = atoi(argv[4])

	// int maxPrefix = 1000;
	// double search_time[maxPrefix];
	// double fetch_time[maxPrefix];
	// int match_num[maxPrefix];
	// int result_num[maxPrefix];
	// int query_num[maxPrefix];

	long long global = 0.0;
    int nq = 0;

	readData(filename, recs);
	readData(queryfile, queries);

	Trie<char>* trie = new Trie<char>();
	for (auto i = 0; i < recs.size(); i++){
		
		
		std::string item = recs[i];
		std::string firstWord = item.substr(0,tok);
		std::string strToTable = item.erase(0,tok);


		m_table.emplace(i, strToTable); 
		trie->append(firstWord.c_str(), i);
	}
	trie->buildIdx();
	// finished indexing

	for (auto i = 0; i < queries.size(); i++) {

		std::chrono::nanoseconds start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());

		vector<int> results;

		std::string queryStr = queries[i];
		std::string firstWordQuery = queryStr.substr(0,tok);
		std::string secondQuery = queryStr.erase(0,tok);

		if(firstWordQuery.size() > 0){

			PrefixActiveNodeSet<char>* pset = new PrefixActiveNodeSet<char>(trie, tau);
			for (auto j = 1; j <= firstWordQuery.length(); j++) {
				// timeval start, middle, term;
				// gettimeofday(&start, NULL);
				char ch = firstWordQuery[j - 1];     // current key stroke
				PrefixActiveNodeSet<char>* temp = pset->computeActiveNodeSetIncrementally(ch);
				delete pset;
				pset = temp;

				// gettimeofday(&middle, NULL);

				// cout << pset->getNumberOfActiveNodes() << " active nodes" << endl;
				// cout << "begin fetch results" << endl;
				map<TrieNode<char>*, unsigned> minActiveNodes;

				pset->computeMinimizedTrieNodesInRange(0, tau, minActiveNodes);
				// cout << minActiveNodes.size() << " active nodes";
				// cout << "fetch results" << endl;
				//    unordered_set<int> resrec;

				if(j == firstWordQuery.length()){
					
					int prev_last = -1;
					auto tit = trie->ids.begin();
					for (auto mit = pset->trieNodeDistanceMap.begin(); mit != pset->trieNodeDistanceMap.end(); mit++) {
						if (mit->first->last <= prev_last) continue;
						prev_last = mit->first->last;
						tit = lower_bound(tit, trie->ids.end(), make_pair(mit->first->id, -1));
						while (tit != trie->ids.end() && tit->first <= mit->first->last) {
							results.push_back(tit->second);
							++tit;
						}
					}
			
				}
			}
		}



		//second level
		vector<int> newResults;
		if(secondQuery.size() > 0){


			for(int found : results){

				std::string item = m_table[found];

				// int distanceTotal = INT_MAX;


				int distance = 999999;

				int abs = item.size() - secondQuery.size();

				for(int k = 0; k<abs; k++){
					item.pop_back();
				}

				distance = editDistance(secondQuery, item);
			
				if(distance <= tau){
					newResults.push_back(found);
				}


			}

		}

/*		if(secondQuery.size() > 0){
			cout << "second: ";
			for(auto r : newResults){
				cout << r << "  ";
			}
		}
		else{
			cout << "first: ";
			for(auto r : results){
				cout << r << "  ";
			}
		}
*/
		std::chrono::nanoseconds end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());

		nq++;
		std::cout << "[LOG] Query "<< nq << ": " << (end - start).count() << " ns" << std::endl;

		global += (end - start).count();
	}

	double mglob, dnq, nstoms;
    mglob = global;
    dnq = nq;
    nstoms = 1000000;

    std::cout << endl << "[LOG] GLOBAL " << global << " ns" << std::endl;
    std::cout << "[LOG] MEDIA GLOBAL " << mglob/(dnq*nstoms) << " ms" << std::endl;

	// int idx = 0;
	// while (true) {
	// 	if (query_num[idx] == 0) break;
	// 	int num = query_num[idx];
	// 	double total_time = (search_time[idx] + fetch_time[idx]) / num;
	// 	printf("%d %d %3f %3f %3f %d %d %d %d\n", idx, num, total_time, search_time[idx] / num, 
	// 				fetch_time[idx] / num, match_num[idx] / num, result_num[idx] / num, match_num[idx], result_num[idx]);
	// 	++idx;
	// }

	return 0;
}
