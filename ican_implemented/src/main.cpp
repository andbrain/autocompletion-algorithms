#include "occurrence.hpp"
#include "trie.hpp"
#include <chrono>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
    int tau;

    if(argc < 3){
        cout << "ERROR: missing arguments" << endl << " Command example: ./cheeki path-database.txt path-queries.txt 1(limit edit distance - *optional)" << endl;

        return 0;
    }
    else if(argc == 3){
        tau = 1;
    }
    else{
	    tau = atoi(argv[3]);
    }

    std::ifstream fileInputStream(argv[1]);
    std::ifstream fileQueryStream(argv[2]);

    trie::Trie_t tree;

    long long global = 0.0;
    int nq = 0;

    std::string currentLine;
    std::string word;
    string query;
    char opt;
    int limit;

    DIR* dirp;
    struct dirent* directory;

    dirp = opendir("./stopwords");
    if (dirp) {
        while ((directory = readdir(dirp)) != NULL) {
            if (directory->d_type == DT_REG) {
                cout << "The stopword file \'" << directory->d_name << "\' has been detected\n";
                tree.addStopWords(std::string("./stopwords/") + (directory->d_name));
            }
        }

        closedir(dirp);
    }

    // tree.putFile(argv[1]);

    while (std::getline(fileInputStream, currentLine)) {
        std::stringstream wordStream(currentLine);
        char position = 1;
        unsigned int ref = tree.insertItemOnTable(currentLine);
        while (std::getline(wordStream, word, ' ')) {
            // word.pop_back();

            if (!tree.isStopWord(word)) {
                tree.putWord(word, position, ref);
            }

            position++;
        }
    }

    tree.buildActiveNodeSet();


    tree.setFuzzyLimitThreshold(tau);

    while (std::getline(fileQueryStream, currentLine)) {

        if (currentLine.size() > 0) {

            // currentLine.pop_back();
            // cout << currentLine << endl;

            std::chrono::nanoseconds start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
            auto fuzzyWords = tree.searchSimilarKeyword2(currentLine);
            std::chrono::nanoseconds end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
            
            
            nq++;
            
            std::cout << "[LOG] Query "<< nq << ": " << (end - start).count() << " ns" << std::endl;

            global += (end - start).count();

            // unsigned int count = 1;
            // for(trie::Occurrence_t ocurrence : fuzzyWords){
            // while (!fuzzyWords.empty() && count < 10) {
            //     auto ocurrence = fuzzyWords.top();
            //     fuzzyWords.pop();
            //     cout << tree.getItemOnTable(ocurrence.itemRef) << endl;
            //     count++;
            // }


            // cout << "]\n}";

            // cout << '\n';
            // }
        }

    }

    double mglob, dnq, nstoms;
    mglob = global;
    dnq = nq;
    nstoms = 1000000;

    std::cout << endl << "[LOG] GLOBAL " << global << " ns" << std::endl;
    std::cout << "[LOG] MEDIA GLOBAL " << mglob/(dnq*nstoms) << " ms" << std::endl;


    // // cout << "FuzzyWord Search v0.2 \t\t Teewa 2017\n\n";
    // // cout << "P - print the trie (caution).\n";
    // // cout << "R <NUMBER> - set the results number limit.\n";
    // // cout << "L <NUMBER> - set the fuzzy characters number limit\n";
    // // cout << "F <STRING> - fuzzy string query\n";
    // // cout << "Q <STRING> - normal string query\n";
    // // cout << "E - exit\n";

    // // cout << "\n>";
    // while (cin >> opt) {
    //     switch (opt) {
    //     case 'P':
    //     case 'p': {
    //         cout << "\n ### START OF TRIE ##\n";
    //         tree.printTrie();
    //         cout << "\n ### END OF TRIE ##\n";
    //     } break;

    //     case 'R':
    //     case 'r': {
    //         cin >> limit;
    //         tree.setSearchLimitThreshold(limit);
    //         cout << "\n New search limit threshold set to \"" << limit << "\"\n";
    //     } break;

    //     case 'l':
    //     case 'L': {
    //         cin >> limit;
    //         tree.setFuzzyLimitThreshold(limit);
    //         cout << "\n New fuzzy limit threshold set to \"" << limit << "\"\n";
    //     } break;

    //     // case 'Q':
    //     // case 'q': {
    //     //     cin >> query;
    //     //     std::vector<std::string> sugestions = tree.searchExactKeyword(query);
    //     //     if (!sugestions.empty()) {
    //     //         cout << "\nSugestions for \"" << query << "\"\n";
    //     //         for (std::string currentSugestion : sugestions) {
    //     //             cout << " - " << currentSugestion << '\n';
    //     //         }
    //     //     } else {
    //     //         cout << "\nNo results found for query \"" << query << "\"\n";
    //     //     }
    //     // } break;

    //     case 'f':
    //     case 'F':

    //     {
    //         string realQuery;

    //         while (cin >> query && query != "$") {

    //             if (!tree.isStopWord(query)) {
    //                 realQuery += query + ' ';
    //             }
    //         }

    //         if (realQuery.size() > 0) {

    //             realQuery.pop_back();
    //             // cout << '\n';

    //             std::chrono::nanoseconds start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
    //             auto fuzzyWords = tree.searchSimilarKeyword(realQuery);
    //             std::chrono::nanoseconds end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
                
                
    //             nq++;
                
    //             std::cout << "[LOG] Query "<< nq << ": " << (end - start).count() << " ns" << std::endl;

    //             global += (end - start).count();

    //             // unsigned int count = 1;
    //             // for(trie::Occurrence_t ocurrence : fuzzyWords){
    //             // while (!fuzzyWords.empty() && count < 10) {
    //             //     auto ocurrence = fuzzyWords.top();
    //             //     fuzzyWords.pop();
    //             //     cout << tree.getItemOnTable(ocurrence.itemRef) << endl;
    //             //     count++;
    //             // }


    //             // cout << "]\n}";

    //             // cout << '\n';
    //         } else {
    //             cout << "Cannot find this query\n";
    //         }
    //         break;
    //     }

    //     case 'E':
    //     case 'e':
    //         std::cout << "[LOG] GLOBAL " << global << " ns" << std::endl;
    //         return 0;
    //         break;
    //     }
    //     // cout << "\n>";
    // }

    return 0;
}
