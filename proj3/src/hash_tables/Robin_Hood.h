#ifndef ROBINHOOD_H
#define ROBINGOOD_H

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
	void insert(int key, int value) {}

	//remove method
	void remove(int key) {}

	//display method
	void display() {}

	//rehash method
	void rehash(int start_index) {}
};

#endif 

