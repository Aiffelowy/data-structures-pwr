#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>

namespace Hash2 {

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
    int hashFunction(int key);

public:
    // Constructor
    HashTable(int size);

    // Destructor
    ~HashTable();

    // Insert method
    void insert(int key, T value);

    // Remove method
    void remove(int key);

    // Display method
    void display();
};

template <class T>
HashTable<T>::HashTable(int size) : size(size) {
    table = new Node<T>*[size]();
}

template <class T>
HashTable<T>::~HashTable() {
    for (int i = 0; i < size; ++i) {
        Node<T>* current = table[i];
        while (current) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] table;
}

// DJB2 hashFunction
template <class T>
int HashTable<T>::hashFunction(int key) {
    int hash = 5381;
    int c;
    std::string key_str = std::to_string(key);
    for (char c : key_str) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % size;
}

template <class T>
void HashTable<T>::insert(int key, T value) {
    int index = hashFunction(key);
    Node<T>* newNode = new Node<T>(key, value);
    if (!table[index]) {
        table[index] = newNode;
    }
    else {
        Node<T>* current = table[index];
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

template <class T>
void HashTable<T>::remove(int key) {
    int index = hashFunction(key);
    Node<T>* current = table[index];
    Node<T>* prev = nullptr;
    while (current) {
        if (current->key == key) {
            if (prev)
                prev->next = current->next;
            else
                table[index] = current->next;
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

template <class T>
void HashTable<T>::display() {
    for (int i = 0; i < size; ++i) {
        std::cout << "Bucket " << i << ": ";
        Node<T>* current = table[i];
        while (current) {
            std::cout << "(" << current->key << ", " << current->value << ") ";
            current = current->next;
        }
        std::cout << std::endl;
    }
}

}

#endif
