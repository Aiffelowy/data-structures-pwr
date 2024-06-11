#ifndef ROBINHOOD_H
#define ROBINGOOD_H

#include <iostream>
#include <utility>

//entry class
class Entry {
public:
	int key;
	int value;
	bool isOccupied;

	//constructor
	Entry() : key(0), value(0), isOccupied(false) {}

	//destructor
	~Entry() {}
};

class RobinHood {
private:
	Entry* table;
	int size; 

	//the reason why we do not use DJB2 HashFunction is to use power of Robin Hood hashing
	int hashFunction(int key) {
		return key % size;
	}
public:
	//constructor
	RobinHood(int table_size) : size(table_size) {
		table = new Entry[size];
	}

	//destructor
	~RobinHood() {
		delete[] table;
	}

	//insert method
	void insert(int key, int value) {
		//calculate the index
		int index = hashFunction(key);
		int dist = 0;
		while (table[index].isOccupied) {
			//calculate the distance from original index if occupied
			int current_dist = (index + size - hashFunction(table[index].key)) % size;
			//if distance current key is shorter than new key, sawp place
			if (current_dist < dist) {
				std::swap(table[index].key, key);
				std::swap(table[index].value, value);
				dist = current_dist;
			}
			//moving until find empty spot
			index = (index + 1) % size;
			dist++;
		}
		table[index].key = key;
		table[index].value = value;
		table[index].isOccupied = true;
	}

	//remove method
	void remove(int key) {
		int index = hashFunction(key);
		int dist = 0;
		while (table[index].isOccupied) {
			if (table[index].key == key) {
				table[index].isOccupied = false;
				table[index].key = 0;
				table[index].value = 0;
				return;
			}
			index = (index + 1) % size;
			dist++;
		}
	}

	//display method
	void display() {
		for (int i = 0; i < size; ++i) {
			if (table[i].isOccupied) {
				std::cout << i << ": " << table[i].key << " => " << table[i].value << std::endl;
			}
			else {
				std::cout << i << ": " << "EMPTY" << std::endl;
			}
		}
	}

};


#endif 

