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
    int capacity;
    Node<T>** table; 

    // hash function
    int hashFunction(int key) {
        return key % size;
    }
    //helper to copy our list (for rule of three)
    Node<T>* copyList(Node<T>* head) {
        if (!head) return nullptr;
        Node<T>* newHead = new Node<T>(head->key, head->value);
        Node<T>* current = newHead;
        Node<T>* source = head->next;
        while (source) {
            current->next = new Node<T>(source->key, source->value);
            current = current->next;
            source = source->next;
        }
        return newHead;
    }


public:
    // constructor
    HashTable(int capacity) : size(0), capacity(capacity) {
        table = new Node<T>*[capacity]();  
    }

    //copy constructor
    HashTable(const HashTable& other) : size(other.size), capacity(other.capacity) {
        table = new Node<T>*[capacity]();
        for (int i = 0; i < capacity; ++i) {
            table[i] = copyList(other.table[i]);
        }
    }
    
    //copy operator
    HashTable& operator=(const HashTable& other) {
        if (this == &other) {
            return *this;
        }

        for (int i = 0; i < capacity; ++i) {
            Node<T>* current = table[i];
            while (current != nullptr) {
                Node<T>* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;

        // Copy new resources
        size = other.size;
        capacity = other.capacity;
        table = new Node<T>*[capacity]();
        for (int i = 0; i < capacity; ++i) {
            table[i] = copyList(other.table[i]);
        }

        return *this;
    }

    // Destructor
    ~HashTable() {
        for (int i = 0; i < capacity; ++i) {
            Node<T>* current = table[i];
            while (current != nullptr) {
                Node<T>* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    // Insert method
    void insert(int key, T value);

    // Remove method
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

