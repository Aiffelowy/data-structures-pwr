#ifndef hash_table.h
#define hash_table.h

#include <iostream>

//node class
template <class T>
class Node {
public:
	int key;
	T value;
	Node* next;

	//constructor
	Node(int key, T vaule) : key(key), value(value), next(nullptr);
	
	//destructor
	~Node(void);
};


//Hash Table class
template <class T>
class HashTable {
private:
	int size;
	Node** table;

	int hushFunction(int key) {
		return key % size;
	}

public:
	//construdtor
	HashTable(int size) : size(size) {
		table = new Node * [size]; // pointers array
	}

	//destructor
	~HashTable() {
		for (int i = 0; i < size; ++i) {
			Node* current = table[i];
			while (current != nullptr) {
				Node* temp = current;
				current = current->next;
				delete temp;
			}
		}
		delete[] table;
	}

	void insert(int key, T value);
	void remove(int key);
};

#endif // !hash_table.h

