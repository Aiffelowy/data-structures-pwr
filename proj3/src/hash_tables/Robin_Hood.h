#ifndef ROBINHOOD_H
#define ROBINHOOD_H

#include <iostream>
#include <utility>

// Entry class
class Entry {
public:
    int key;
    int value;
    bool isOccupied;

    // Constructor
    Entry() : key(0), value(0), isOccupied(false) {}

    // Destructor
    ~Entry() {}
};

class RobinHood {
private:
    Entry* table;
    int size;
    int numElements;

    // Hash function
    int hashFunction(int key) {
        return key % size;
    }

    // Rehash function
    void rehash() {
        int oldSize = size;
        size *= 2;
        Entry* oldTable = table;
        table = new Entry[size];
        numElements = 0;

        for (int i = 0; i < oldSize; ++i) {
            if (oldTable[i].isOccupied) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }

        delete[] oldTable;
    }

public:
    // Constructor
    RobinHood(int table_size) : size(table_size), numElements(0) {
        table = new Entry[size];
    }

    // Destructor
    ~RobinHood() {
        delete[] table;
    }

    // Insert method
    void insert(int key, int value) {
        if (numElements >= size / 2) {
            rehash();
        }

        int index = hashFunction(key);
        int dist = 0;

        while (table[index].isOccupied) {
            int current_dist = (index + size - hashFunction(table[index].key)) % size;
            if (current_dist < dist) {
                std::swap(table[index].key, key);
                std::swap(table[index].value, value);
                dist = current_dist;
            }
            index = (index + 1) % size;
            dist++;
        }

        table[index].key = key;
        table[index].value = value;
        table[index].isOccupied = true;
        numElements++;
    }

    // Remove method
    void remove(int key) {
        int index = hashFunction(key);
        int dist = 0;

        while (table[index].isOccupied) {
            if (table[index].key == key) {
                table[index].isOccupied = false;
                table[index].key = 0;
                table[index].value = 0;
                numElements--;
                return;
            }
            index = (index + 1) % size;
            dist++;
        }
    }

    // Display method
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
