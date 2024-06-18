#ifndef Linear.h
#define Linear.h

#include <iostream>


//Keyvalue class, can not find better name for it 
class KeyValue {
public:
	int key;
	int value;
	bool isDeleted;

	//constructor set start values as -1 to represent them as unused
	KeyValue() : key(-1), value(-1), isDeleted(false) {}
};

//hash table class, basic name 
class HashTable {
private:
	int *table;
	int size;
	int count;

	//the most basic hashFunction 
	int hashFunction(int key) {
		return key % size;
	}

	//rehash method
	void rehash() {}

public:
	//constructor
	HashTable(int iSize) {}

	//destructor
	~HashTable() {}

	//inrest method
	void insert(int key, int value) {}

	//remove method
	void remove(int key) {} 

	//display method
	void display() {}
};

#endif
