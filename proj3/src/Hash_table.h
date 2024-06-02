#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>

// Node class
template <class T>
class Node {
public:
    int key;
    T value;
    Node* next;

    // Constructor
    Node(int key, T value) : key(key), value(value), next(nullptr) {}

    // Destructor
    ~Node() {}
};

// Hash Table class
template <class T>
class HashTable {
private:
    int size;
    Node<T>** table;  

    // Hash function
    int hashFunction(int key) {
        return key % size;
    }

public:
    // Constructor
    HashTable(int size) : size(size) {
        table = new Node<T>*[size]();  
    }

    // Destructor
    ~HashTable() {
        for (int i = 0; i < size; ++i) {
            Node<T>* current = table[i];
            while (current != nullptr) {
                Node<T>* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    void insert(int key, T value);
    void remove(int key);
};

// Insert method
template <class T>
void HashTable<T>::insert(int key, T value) {
    int index = hashFunction(key);
    Node<T>* newNode = new Node<T>(key, value);

    newNode->next = table[index];
    table[index] = newNode;
}


#endif 

