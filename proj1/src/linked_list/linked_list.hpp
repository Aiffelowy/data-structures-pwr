#ifndef MY_LINKED_LIST
#define MY_LINKED_LIST

#include <functional>
#include <iostream>

template <typename T>
struct Node {
  T content;
  Node* next;
  
  Node(T);
  Node* get_next_node(int);
};

template<typename T>
Node<T>::Node(T item) : content(item), next(nullptr) {}

template<typename T>
Node<T>* Node<T>::get_next_node(int position) {
  if(position == 0) { return this; }
  return next->get_next_node(position-1);
}

template <typename T>
struct LinkedList {
private:
  Node<T>* head;
  int length;

  Node<T>* get_node(int);
  void destroy_nodes();

public:
  //constructor
  LinkedList(void);
  //destructor
  ~LinkedList(void);
  //copy constructor
  LinkedList(const LinkedList&);
  //copy assignment
  LinkedList& operator=(const LinkedList&);
  //array accessor
  T& operator[](int);
  //comparison operator
  bool operator==(const LinkedList&) const;

  //iterator
  void for_each(std::function<void(T&)>);

  //find function
  T find(std::function<bool(T&)>);
  
  //push new item at the end of the list
  void push(T);
  //insert new item at the provided position in the list
  void insert(T, int);
  //remove item at the position in the list
  void remove(int);
  //pop the last item on the list
  T pop(void);


  void print_list();
};

template<typename T>
LinkedList<T>::LinkedList(void) : head(nullptr), length(0) {}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList& other) {
  length = other.length;
  if(this->length == 0) { return; }
  
  Node<T>* other_node = other.head;
  head = new Node(other_node->content);
  
  Node<T>* this_node = head;
  for(int i = 1; i < length; i++) {
    other_node = other_node->next;
    this_node->next = new Node(other_node->content);
    this_node = this_node->next;
  }
}

template<typename T>
void LinkedList<T>::destroy_nodes() {
  Node<T>* current = head;
  Node<T>* next = nullptr;
  while(current != nullptr) {
    next = current->next;
    delete current;
    current = next;
  }

}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
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
  return *this;
}

template<typename T>
bool LinkedList<T>::operator==(const LinkedList<T>& other) const {
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
LinkedList<T>::~LinkedList() {
  destroy_nodes();
}

template<typename T>
Node<T>* LinkedList<T>::get_node(int position) {
  if(position > length) { return nullptr; }
  return head->get_next_node(position);
}

template<typename T>
T& LinkedList<T>::operator[](int i) {
  //if(length == 0 || i > length) { return; }
  return get_node(i)->content;
}

template<typename T>
void LinkedList<T>::push(T item) {
  length++;
  if(head == nullptr) { head = new Node(item); return; }
  get_node(length-2)->next = new Node(item);
}

template<typename T>
void LinkedList<T>::insert(T item, int position) {
  if(position > length) { return; }
  Node<T>* new_node = new Node(item);
  if(position == 0) { new_node->next = head; head = new_node; return; }
  //for optimisation
  Node<T>* temp = get_node(position-1);
  new_node->next = temp->next;
  temp->next = new_node;
  length++;
}

template<typename T>
void LinkedList<T>::remove(int position) {
  if(position > length) { return; }
  if(position == 0) { Node<T>* temp = head->next; delete head; head = temp; return; }
  Node<T>* temp = get_node(position-1);
  Node<T>* to_delete = temp->next;
  temp->next = temp->next->next;
  delete to_delete;
  length--;
}

template<typename T>
T LinkedList<T>::pop(void) {
  Node<T>* temp = get_node(length-2);
  T content = temp->next->content;
  delete temp->next;
  temp->next = nullptr;
  length--;
  return content;
}

template<typename T>
void LinkedList<T>::for_each(std::function<void(T&)> lambda) {
  Node<T>* current = head;
  while(current != nullptr) {
    //lambda stuff
    lambda(current->content);
    current = current->next;
  }
}

template<typename T>
T LinkedList<T>::find(std::function<bool(T&)> lambda) {
  Node<T>* current = head;
  while(current != nullptr) {
    if(lambda(current->content)) { return current->content; }
    current = current->next;
  }
  return T();
}

template<typename T>
void LinkedList<T>::print_list() {
  Node<T>* current = head;
  std::cout << "length: " << this->length << "\n";
  while(current != nullptr) {
    std::cout << current->content << " ";
    current = current->next;
  }
  std::cout << "\n";
}

#endif
