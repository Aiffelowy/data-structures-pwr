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

// hash Table class
template <class T>
class HashTable {
private:
    int size;
    Node<T>** table;  

    // hash function
    int hashFunction(int key) {
        return key % size;
    }

public:
    // constructor
    HashTable(int size) : size(size) {
        table = new Node<T>*[size]();  
    }

    // destructor
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

    // insert method
    void insert(int key, T value);
    // remove method
    void remove(int key);
};

// insert method
template <class T>
void HashTable<T>::insert(int key, T value) {
    int index = hashFunction(key);
    Node<T>* newNode = new Node<T>(key, value);

    newNode->next = table[index];
    table[index] = newNode;
    size++;
}

// remove method
template <class T>
void HashTable<T>::remove(int key) {
    int index = hashFunction(key);
    Node<T>* current = table[index];
    Node<T>* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                table[index] = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
    size--;
}


#endif 

