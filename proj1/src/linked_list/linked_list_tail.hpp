#ifndef LINKED_LIST_T_H
#define LINKED_LIST_T_H

#include "node.hpp"

#include <functional>
#include <iostream>

//the comments here will be almost the same from the normal linked list, execpt we set the tail where needed

template <typename T>
struct LinkedListT {
private:
  Node<T>* head;
  Node<T>* tail;
  int length;

  Node<T>* get_node(int);
  void destroy_nodes();

public:
  //constructor
  LinkedListT(void);
  //destructor
  ~LinkedListT(void);
  //copy constructor
  LinkedListT(const LinkedListT&);
  //copy assignment
  LinkedListT& operator=(const LinkedListT&);
  //array accessor
  T& operator[](int);
  //comparison operator
  bool operator==(const LinkedListT&) const;

  //iterator
  void for_each(std::function<void(T&)>);

  //find function
  T find(std::function<bool(const T&)>) const;
  
  //push new item at the end of the list
  void push(T);
  //insert new item at the provided position in the list
  void insert(T, int);
  //remove item at the position in the list
  void remove(int);
  //pop the last item on the list
  T pop(void);
  //return the length of the list
  int len(void);

  Node<T>* rtail();


  void print_list();
};

template<typename T>
LinkedListT<T>::LinkedListT(void) : head(nullptr), tail(nullptr), length(0) {}

template<typename T>
LinkedListT<T>::LinkedListT(const LinkedListT& other) {
  length = other.length;
  if(this->length == 0) { return; }
  
  Node<T>* other_node = other.head;
  head = new Node(other_node->content);
  
  Node<T>* this_node = head;
  while(other_node->next != nullptr) {
    other_node = other_node->next;
    this_node->next = new Node(other_node->content);
    this_node = this_node->next;
  }
  this_node->next = nullptr;
  tail = this_node;
}

template<typename T>
void LinkedListT<T>::destroy_nodes() {
  Node<T>* current = head;
  Node<T>* next = nullptr;
  while(current != nullptr) {
    next = current->next;
    delete current;
    current = next;
  }

}

template<typename T>
LinkedListT<T>& LinkedListT<T>::operator=(const LinkedListT& other) {
  if(this == other) { return *this; }
  if(this->head != nullptr) {destroy_nodes();}
  this->length = other.length;

  Node<T>* other_node = other.head;
  head = new Node(other_node->content);
  
  Node<T>* this_node = head;
  while(other_node != nullptr) {
    other_node = other_node->next;
    this_node->next = new Node(other_node->content);
    this_node = this_node->next;
  }
  this_node->next = nullptr;
  this->tail = this_node;
  return *this;
}

template<typename T>
bool LinkedListT<T>::operator==(const LinkedListT<T>& other) const {
  if(this->length != other.length) { return false; }
  Node<T>* this_node = head;
  Node<T>* other_node = other.head;
  while(this_node != nullptr) {
    if(this_node->content != other_node->content) { return false; }
    this_node = this_node->next;
    other_node = other_node->next;
  }
  return true;
}

template<typename T>
LinkedListT<T>::~LinkedListT() {
  destroy_nodes();
}

template<typename T>
Node<T>* LinkedListT<T>::get_node(int position) {
  if(position == length-1) { return tail; }
  if(position >= length) { return nullptr; }
  return head->get_next_node(position);
}

template<typename T>
T& LinkedListT<T>::operator[](int i) {
  //if(length == 0 || i > length) { return; }
  return get_node(i)->content;
}

template<typename T>
void LinkedListT<T>::push(T item) {
  length++;
  if(head == nullptr) { head = tail = new Node(item); return; }
  tail->next = new Node(item);
  tail = tail->next;
}

template<typename T>
void LinkedListT<T>::insert(T item, int position) {
  if(position > length) { return; }
  Node<T>* new_node = new Node(item);
  if(position == 0) { new_node->next = head; head = new_node; return; }
  if(position == length-1) { tail->next = new_node; tail = new_node; return; }
  //for optimisation
  Node<T>* temp = get_node(position-1);
  new_node->next = temp->next;
  temp->next = new_node;
  length++;
}

template<typename T>
void LinkedListT<T>::remove(int position) {
  if(position > length) { return; }
  if(position == 0) { Node<T>* temp = head->next; delete head; head = temp; return; }
  Node<T>* temp = get_node(position-1);
  Node<T>* to_delete = temp->next;
  if(to_delete == tail) { tail = temp; }
  temp->next = to_delete->next;
  delete to_delete;
  length--;
}

template<typename T>
T LinkedListT<T>::pop(void) {
  Node<T>* temp = get_node(length-2);
  T content = temp->next->content;
  delete temp->next;
  temp->next = nullptr;
  tail = temp;
  length--;
  return content;
}

template<typename T>
void LinkedListT<T>::for_each(std::function<void(T&)> lambda) {
  Node<T>* current = head;
  while(current != nullptr) {
    //lambda stuff
    lambda(current->content);
    current = current->next;
  }
}

template<typename T>
T LinkedListT<T>::find(std::function<bool(const T&)> lambda) const {
  Node<T>* current = head;
  while(current != nullptr) {
    if(lambda(current->content)) { return current->content; }
    current = current->next;
  }
  return T();
}

template<typename T>
void LinkedListT<T>::print_list() {
  Node<T>* current = head;
  std::cout << "length: " << this->length << "\n";
  while(current != nullptr) {
    std::cout << current->content << " ";
    current = current->next;
  }
  std::cout << "\n";
}

template<typename T>
int LinkedListT<T>::len() { return this->length; }

template<typename T>
Node<T>* LinkedListT<T>::rtail() { return tail; }

#endif
