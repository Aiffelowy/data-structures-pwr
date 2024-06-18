#ifndef LINEAR_H
#define LINEAR_H

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
public:
	KeyValue *table;
	int size;
	int count;

	//the most basic hashFunction 
	int hashFunction(int key) {
		return key % size;
	}

	//rehash method
	void rehash() {
		int oldSize = size;
		size *= 2;
		KeyValue* oldTable = table;
		table = new KeyValue[size];

		count = 0;
		for (int i = 0; i < size; i++) {
			table[i] = KeyValue();
		}

		for (int i = 0; i < oldSize; i++) {
			if (oldTable[i].key != -1 && !oldTable[i].isDeleted) {
				insert(oldTable[i].key, oldTable[i].value);
			}
		}

		delete[] oldTable;
	}

	//constructor
	HashTable(int iSize) {
		size = iSize;
		table = new KeyValue[size];
		count = 0;
	}

	//destructor
	~HashTable() {
		delete[] table;
	}

	//inrest method
	void insert(int key, int value) {
		if (count >= size / 2) {
			rehash();
		}

		int hash = hashFunction(key);
		while (table[hash].key != -1 && !table[hash].isDeleted) {
			if (table[hash].key == key) {
				table[hash].value = value;
				return;
			}
			hash = (hash + 1) % size;
		}

		table[hash].key = key;
		table[hash].value = value;
		table[hash].isDeleted = false;
		count++;
	}

	//remove method
	void remove(int key) {}

	//display method
	void display() {}
};

#endif
