#ifndef hash_table.h
#define hash_table.h

#include <iostream>

template <class T>
class Node {
public:
	int key;
	int value;
	Node* next;

	Node();
	~Node();
};

template <class T>
class HashTable {
private:
	int size;
	Node** table;

	int hushFunction(int key) {
		return key % size;
	}

public:

	HashTable();
	~HashTable();

	void insert(int key, int value);
	void remove(int key);
};

#endif // !hash_table.h

