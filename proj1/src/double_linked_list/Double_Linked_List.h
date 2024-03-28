#ifndef Double_Linked_List
#define Double_Linked_List

#include <iostream>

//DLL = Double Linked List

//Node class
template<class T>
class Node {
public:
    T number;
    Node<T>* previous;
    Node<T>* next;

    //constructor
    Node(void) : previous(nullptr), next(nullptr) {}
    //destructor
    ~Node() {}

};

// The "main" class
template<class T>
class DoubleLinkedList {
private:
    int size;

public:
    Node<T>* head;
    Node<T>* tail;


    //constructor
    DoubleLinkedList(void) : head(nullptr), tail(nullptr), size(0) {}
    //destructor
    ~DoubleLinkedList() {}

    //add to the front
    void push_head(const T& value);
    //add to the end
    void push_tail(const T& value);
    //add to any place
    void push(int index, const T& value);

    //remove from the front
    void pop_head();
    //remove from the end
    void pop_tail();
    //remove from any place
    void pop(int index);

    //show the list
    void show() const;
    //show the list backward
    void show_backward() const;
    //size of the list
    int counter() const;
    //search for the number
    int search(int index);
};

template<class T>
void DoubleLinkedList<T>::pop_head() {
    if (size == 0) {
        return;
    }

    Node<T>* node = head; //set pointer as first element in DLL
    head = head->next; //set head as one after first head in DLL

    delete node;

    if (head != nullptr) { //if no head, set head as nullptr
        head->previous = nullptr;
    }
    size--;
}

template<class T>
void DoubleLinkedList<T>::pop_tail() {
    //checking size
    if (size == 0) {
        return;
    }
    else if (size == 1) {
        pop_head();
        return;
    }

    Node<T>* NewNode = tail; //set pointer as last element in DLL
    tail = tail->previous; //set tail as one before last tail in DLL
    tail->next = nullptr;
    delete NewNode;
    size--;
}

template<class T>
void DoubleLinkedList<T>::pop(int index) {
    // Check if the list is empty
    if (size == 0) {
        return;
    }

    // Check if the index is out of range
    if (index < 0 || index >= size) {
        std::cout << "Index out of range \n";
        return;
    }

    Node<T>* current;

    if (index < size / 2) { // If the index is closer to the beginning of the DLL
        current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
    }
    else { // If the index is closer to the end of the DLL
        current = tail;
        for (int i = size - 1; i > index; --i) {
            current = current->previous;
        }
    }

    // Perform the appropriate operation based on the index
    if (index == 0) { // If the index is at the beginning of the DLL
        pop_head();
    }
    else if (index == size - 1) { // If the index is at the end of the DLL
        pop_tail();
    }
    else {
        current->previous->next = current->next;
        current->next->previous = current->previous;
        delete current;
        size--;
    }
}


template<class T>
void DoubleLinkedList<T>::push_head(const T& value) {

    Node<T>* newNode = new Node<T>();
    newNode->number = value;
    //If DLL is empty
    if (!head) {
        head = newNode;
        tail = newNode;
    }
    // Setting pointer
    else {
        newNode->next = head;
        head->previous = newNode;
        head = newNode;
    }
    size++;
}

template<class T>
void DoubleLinkedList<T>::push_tail(const T& value) {

    Node<T>* newNode = new Node<T>();
    newNode->number = value;
    //If DLL is empty
    if (!tail) {
        head = newNode;
        tail = newNode;
    }
    // Setting pointer
    else {
        newNode->previous = tail;
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}


template<class T>
void DoubleLinkedList<T>::push(int index, const T& value) {
    if (index > size || index < 0) {
        std::cout << "Index out of range \n";
        return;
    }
    // checking index and size
    if (index == 0 || size == 0) {
        push_head(value);
        return;
    }

    if (index == size) {
        push_tail(value);
        return;
    }

   // Checking where is better to start from, head or tail
    Node<T>* current;
    if (index < size / 2) { // If index is closer to the beginning of the DLL
        current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
    }
    else { // If index is closer to the end of the DLL
        current = tail;
        for (int i = size - 1; i > index; --i) {
            current = current->previous;
        }
    }

    Node<T>* newNode = new Node<T>();
    newNode->number = value;

    // Put new node to the DLL
    newNode->previous = current->previous;
    newNode->next = current;
    current->previous->next = newNode;
    current->previous = newNode;

    size++;
}

//showing the DLL
template<class T>
void DoubleLinkedList<T>::show() const {
    Node<T>* current = head;
    while (current) {
        std::cout << current->number << " ";
        current = current->next;
    }
    std::cout << "\n";
}

//showing the DLL backward
template<class T>
void DoubleLinkedList<T>::show_backward() const {
    Node<T>* current = tail;
    while (current) {
        std::cout << current->number << " ";
        current = current->previous;
    }
    std::cout << "\n";
}

template<class T>
int DoubleLinkedList<T>::counter() const {
    return size;
}

template<class T>
int DoubleLinkedList<T>::search(int index) {
    // Check if the index is within the list range
    if (index < 0 || index >= size) {
        std::cout << "Index out of range \n";
        return 0;
    }

    Node<T>* current;

    // Choose starting point
    if (index < size / 2) { // If the index is closer to the beginning of the DLL
        current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
    }
    else { // If the index is closer to the end of the DLL
        current = tail;
        for (int i = size - 1; i > index; --i) {
            current = current->previous;
        }
    }

    // Return the value
    return current->number;
}


#endif