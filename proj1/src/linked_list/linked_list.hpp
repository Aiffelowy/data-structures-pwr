#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "node.hpp"

#include <functional>

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
  int len();


};

//default constructor, set everything to zero, "nought" as british people call it
template<typename T>
LinkedList<T>::LinkedList(void) : head(nullptr), length(0) {}

//copy constructor, so our kernel doesnt call us stupid
template<typename T>
LinkedList<T>::LinkedList(const LinkedList& other) {
  //copy length
  this->length = other.length;
  if(this->length == 0) { return; }
  //lets keep track of the node pointer of the other list
  Node<T>* other_node = other.head;
  //make a copy of a node with the same content as in the other list at our head
  this->head = new Node(other_node->content);
  //and lets keep track of nodes in this list
  Node<T>* this_node = head;
  //iterate over all nodes
  while(other_node->next != nullptr) {
    //set this to the next node
    other_node = other_node->next;
    //then set our next to the new node with stuff from the other node
    this_node->next = new Node(other_node->content);
    //and new lets track the new node
    this_node = this_node->next;
  }
  //and the final node gets no new friend :((
  this_node->next = nullptr;
}

//lets commit mass murde- i mean genocid- ...eee... free memory
template<typename T>
void LinkedList<T>::destroy_nodes() {
  //keep track of this and next node
  Node<T>* current = head;
  //we need this so we can access the next node after we delete current
  Node<T>* next = nullptr;
  while(current != nullptr) {
    next = current->next;
    delete current;
    current = next;
  }

}

//copy assignment to spare our CPU *some* cringe
template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
  //if we are us, then dont do anything
  if(this == other) { return *this; }
  //if we hold data, we need to kill it.. i mean free it
  if(this->head != nullptr) { destroy_nodes(); }
  //copy length
  this->length = other.length;
  //keep track of other nodes
  Node<T>* other_node = other.head;
  //set our head to something new --- maybe i should pick up snowboard instead of skiis? nah lets steal ideas from the other list
  head = new Node(other_node->content);
  
  //keep track of our nodes
  Node<T>* this_node = head;
  while(other_node != nullptr) {
    other_node = other_node->next;
    //steal other nodes content and make it our next node
    this_node->next = new Node(other_node->content);
    //and track this new node, the old one got boring
    this_node = this_node->next;
  }
  //of course the last node gets no friend, what a looser
  this_node->next = nullptr;
  return *this;
}

//comparison operator
template<typename T>
bool LinkedList<T>::operator==(const LinkedList<T>& other) const {
  //if we dont hold the same amount of data, then its just not the same
  if(this->length != other.length) { return false; }
  //keep track of the nodes
  Node<T>* this_node = head;
  Node<T>* other_node = other.head;

  while(this_node != nullptr) {
    //if something is different then return false
    if(this_node->content != other_node->content) { return false; }
    this_node = this_node->next;
    other_node = other_node->next;
  }
  //we passed all other tests! literally twins omg!
  return true;
}

//KILL KILL KILL
template<typename T>
LinkedList<T>::~LinkedList() {
  //MORE KILL
  destroy_nodes();
}

//get the node at index
template<typename T>
Node<T>* LinkedList<T>::get_node(int position) {
  //if out of bounds, punish user for being bad.
  if(position < 0 || position >= length) { return nullptr; }
  //just let the nodes do the work for you. Its not slavery its a volunteer work
  return head->get_next_node(position);
}


template<typename T>
T& LinkedList<T>::operator[](int i) {
  //get the node at index
  return get_node(i)->content;
}

//push item at the end
template<typename T>
void LinkedList<T>::push(T item) {
  length++;
  //if we dont have a head, we need to grow one (maybe thinking will be easier)
  if(head == nullptr) { head = new Node(item); return; }
  //create new node with a given item
  Node<T>* node = new Node(item);
  node->next = nullptr;
  //set the last nodes next into our new node
  get_node(length-2)->next = node;
}

//insert node at the position
template<typename T>
void LinkedList<T>::insert(T item, int position) {
  //if out of bounds: bad
  if(position < 0 || position > length) { return; }
  //new node with item
  Node<T>* new_node = new Node(item);
  //if position is head, then we need to update head
  if(position == 0) { new_node->next = head; head = new_node; return; }
  //get the node before our position
  Node<T>* temp = get_node(position-1);
  //change the new nodes next
  new_node->next = temp->next;
  //update the next of the node before
  temp->next = new_node;
  length++;
}

//remove item at position
template<typename T>
void LinkedList<T>::remove(int position) {
  if(position < 0 || position > length) { return; }
  //if position 0, we need to cut our head off
  if(position == 0) { Node<T>* temp = head->next; delete head; head = temp; return; }
  //get the node before
  Node<T>* temp = get_node(position-1);
  //store the node we want to delete, its not needed but ... idk
  Node<T>* to_delete = temp->next;
  //set the node before to skip this node
  temp->next = temp->next->next;
  //KILL
  delete to_delete;
  length--;
}

//remove the last node and get its content
template<typename T>
T LinkedList<T>::pop(void) {
  //get the node before last
  Node<T>* temp = get_node(length-2);
  //get the last nodes content
  T content = temp->next->content;
  //MURDER
  delete temp->next;
  //so we killed the temps node friend :((
  temp->next = nullptr;
  length--;
  //return the insides of now dead node
  return content;
}

//modify nodes with custom logic
template<typename T>
void LinkedList<T>::for_each(std::function<void(T&)> lambda) {
  //just a simple loop that applies the lambda to every item
  Node<T>* current = head;
  while(current != nullptr) {
    //lambda stuff
    lambda(current->content);
    current = current->next;
  }
}

//find item according to a custom logic
//should this be T&? maybe, probaby
template<typename T>
T LinkedList<T>::find(std::function<bool(const T&)> lambda) const {
  //just a simple loop that applies the lambda to every item
  Node<T>* current = head;
  while(current != nullptr) {
    //if this custom logic finds something, then return it
    if(lambda(current->content)) { return current->content; }
    current = current->next;
  }
  return T();
}

//return length
template<typename T>
int LinkedList<T>::len() { return this->length; }

#endif
