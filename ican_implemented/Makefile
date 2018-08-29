### C++ Arguments
CC=g++
CF=-std=c++14 -Wall -O2

### Program Arguments
NAME=cheeki

### Compiler Arguments
SRC_P=src

all: merge clean

merge:	twoltrie.o trie.o trieNode.o occurrence.o itemTable.o
	$(CC) $(CF) -o $(NAME) trie.o trieNode.o main.o occurrence.o itemTable.o

twoltrie.o: trie.o $(SRC_P)/main.cpp
	$(CC) $(CF) -c $(SRC_P)/main.cpp

trie.o: trieNode.o itemTable.o $(SRC_P)/trie.cpp $(SRC_P)/trie.hpp
	$(CC) $(CF) -c $(SRC_P)/trie.cpp

trieNode.o: occurrence.o $(SRC_P)/trieNode.cpp $(SRC_P)/trieNode.hpp
	$(CC) $(CF) -c $(SRC_P)/trieNode.cpp

itemTable.o: $(SRC_P)/itemTable.cpp $(SRC_P)/itemTable.hpp
	$(CC) $(CF) -c $(SRC_P)/itemTable.cpp

occurrence.o: $(SRC_P)/occurrence.cpp $(SRC_P)/occurrence.hpp
	$(CC) $(CF) -c $(SRC_P)/occurrence.cpp

clean:
	rm -rf *.o
