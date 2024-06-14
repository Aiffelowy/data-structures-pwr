#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>

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
    int numElements;
    Node<T>** table;

    // Hash function
    int hashFunction(int key);

    void rehash();

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

//basic constructor
template <class T>
HashTable<T>::HashTable(int size) : size(size) {
    table = new Node<T>*[size]();
}
//destructor, we need free memory
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
//DJB2 hush function
template <class T>
int HashTable<T>::hashFunction(int key) {
    unsigned long hash = 5381;
    std::string key_str = std::to_string(key);
    for (char c : key_str) {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % size;
}

// Rehash function
template <class T>
void HashTable<T>::rehash() {
    int oldSize = size;
    size = size * 2;
    Node<T>** oldTable = table;
    table = new Node<T>*[size]();
    numElements = 0;

    for (int i = 0; i < oldSize; ++i) {
        Node<T>* current = oldTable[i];
        while (current) {
            insert(current->key, current->value);
            Node<T>* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] oldTable;
}

//insert method
template <class T>
void HashTable<T>::insert(int key, T value) {
    if (static_cast<float>(numElements) / size >= 0.5) {
        rehash();
    }
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
    numElements++;
}

//remove method
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

//display method, its important to see if it works
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
//i assumed that code isnt much complicated and do not need many comments

#endif
