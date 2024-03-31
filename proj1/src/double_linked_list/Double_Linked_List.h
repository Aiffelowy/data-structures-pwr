#ifndef Double_Linked_List
#define Double_Linked_List

#include <functional>
#include <iostream>

//DLL = Double Linked List

//NodeD class
template<class T>
class NodeD {
public:
    T number;
    NodeD<T>* previous;
    NodeD<T>* next;

    //constructor
    NodeD(void) : previous(nullptr), next(nullptr) {}
    //destructor
    ~NodeD() {};

};

// The "main" class
template<class T>
class DoubleLinkedList {
private:
    int size;

public:
    NodeD<T>* head;
    NodeD<T>* tail;


    //constructor
    DoubleLinkedList(void) : size(0), head(nullptr), tail(nullptr) {}
    //copy constructor
    DoubleLinkedList(const DoubleLinkedList&);
    //copy assignment
    DoubleLinkedList& operator=(const DoubleLinkedList&);
    //destructor
    ~DoubleLinkedList();

    //add to the front
    void push_head(const T& value);
    //add to the end
    void push(const T& value);
    //add to any place
    void insert(int index, const T& value);

    //remove from the front
    void pop_head();
    //remove from the end
    void pop();
    //remove from any place
    void remove(int index);

    //show the list
    void show() const;
    //show the list backward
    void show_backward() const;
    //size of the list
    int counter() const;
    //search for the number
    int search(int index);
    //return length of the list
    int len() const;

    T find(std::function<bool(const T&)>) const;
};

template<class T>
DoubleLinkedList<T>::~DoubleLinkedList() {
  NodeD<T>* current = head;
  NodeD<T>* next = nullptr;
  while(current != nullptr) {
    next = current->next;
    delete current;
    current = next;
  }
}

template<class T>
DoubleLinkedList<T>::DoubleLinkedList(const DoubleLinkedList& other) {
  size = other.size;
  if(this->size == 0) { return; }

  NodeD<T>* other_node = other.head;
  head = new NodeD<T>();
  head->number = other_node->number;
  
  NodeD<T>* this_node = head;
  while(other_node->next != nullptr) {
    other_node = other_node->next;
    this_node->next = new NodeD<T>();
    this_node->next->number = other_node->number;
    this_node->next->previous = this_node;
    this_node = this_node->next;
  }
  this_node->next = nullptr;
  tail = this_node;
}

template<class T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(const DoubleLinkedList& other) {
  if(this == other) { return *this; }
  if(this->head != nullptr) {
    NodeD<T>* current = head;
    NodeD<T>* next = nullptr;
    while(current != nullptr) {
      next = current->next;
      delete current;
      current = next;
    }
  }
  this->size = other.size;

  NodeD<T>* other_node = other.head;
  head = new NodeD<T>();
  head->number = other_node->number;
  
  NodeD<T>* this_node = head;
  while(other_node != nullptr) {
    other_node = other_node->next;
    this_node->next = new NodeD<T>();
    this_node->next->number = other_node->number;
    this_node->next->previous = this_node;
    this_node = this_node->next;
  }
  this_node->next = nullptr;
  tail = this_node;
  return *this;
}

template<class T>
void DoubleLinkedList<T>::pop_head() {
    if (size == 0) {
        return;
    }

    NodeD<T>* node = head; //set pointer as first element in DLL
    head = head->next; //set head as one after first head in DLL

    delete node;

    if (head != nullptr) { //if no head, set head as nullptr
        head->previous = nullptr;
    }
    size--;
}

template<class T>
void DoubleLinkedList<T>::pop() {
    //checking size
    if (size == 0) {
        return;
    }
    else if (size == 1) {
        pop_head();
        return;
    }

    NodeD<T>* NewNodeD = tail; //set pointer as last element in DLL
    tail = tail->previous; //set tail as one before last tail in DLL
    tail->next = nullptr;
    delete NewNodeD;
    size--;
}

template<class T>
void DoubleLinkedList<T>::remove(int index) {
    // Check if the list is empty
    if (size == 0) {
        return;
    }

    // Check if the index is out of range
    if (index < 0 || index >= size) {
        std::cout << "Index out of range \n";
        return;
    }

    NodeD<T>* current;

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
        pop();
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

    NodeD<T>* newNodeD = new NodeD<T>();
    newNodeD->number = value;
    //If DLL is empty
    if (!head) {
        head = newNodeD;
        tail = newNodeD;
    }
    // Setting pointer
    else {
        newNodeD->next = head;
        head->previous = newNodeD;
        head = newNodeD;
    }
    size++;
}

template<class T>
void DoubleLinkedList<T>::push(const T& value) {

    NodeD<T>* newNodeD = new NodeD<T>();
    newNodeD->number = value;
    //If DLL is empty
    if (!tail) {
        head = newNodeD;
        tail = newNodeD;
    }
    // Setting pointer
    else {
        newNodeD->previous = tail;
        tail->next = newNodeD;
        tail = newNodeD;
    }
    size++;
}


template<class T>
void DoubleLinkedList<T>::insert(int index, const T& value) {
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
        push(value);
        return;
    }

   // Checking where is better to start from, head or tail
    NodeD<T>* current;
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

    NodeD<T>* newNodeD = new NodeD<T>();
    newNodeD->number = value;

    // Put new node to the DLL
    newNodeD->previous = current->previous;
    newNodeD->next = current;
    current->previous->next = newNodeD;
    current->previous = newNodeD;

    size++;
}

//showing the DLL
template<class T>
void DoubleLinkedList<T>::show() const {
    NodeD<T>* current = head;
    while (current) {
        std::cout << current->number << " ";
        current = current->next;
    }
    std::cout << "\n";
}

//showing the DLL backward
template<class T>
void DoubleLinkedList<T>::show_backward() const {
    NodeD<T>* current = tail;
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

    NodeD<T>* current;

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

template<typename T>
int DoubleLinkedList<T>::len() const { return size; }


template<typename T>
T DoubleLinkedList<T>::find(std::function<bool(const T&)> lambda) const {
  NodeD<T>* current = head;
  while(current != nullptr) {
    if(lambda(current->number)) { return current->number; }
    current = current->next;
  }
  return T();
}

#endif
