#ifndef PQUEUE_HEAP_H
#define PQUEUE_HEAP_H

#include "heap.hpp"

template<typename T>
struct PriorityQueueHeap {
private:
  //wow, a nested struct, these exist
 struct Node {
    T element;
    int priority;
    //default constructor, so we can prealloc size within the heap
    Node() {}
    //normal constructor
    Node(T element, int priotiry) : element(element), priority(priotiry) {}
    //we overload comparison operators so the heap can operate on our nodes seemlessly 
    bool operator>(const Node& other) { return priority > other.priority; }
    bool operator<(const Node& other) { return priority < other.priority; }
  };
  //heapy weapy UwU
  Heap<Node> heap;

public:
  //default constructor
  PriorityQueueHeap();
  //constructor that preallocates some space
  PriorityQueueHeap(int);
  //destructor
  ~PriorityQueueHeap();
  
  //insert item with priority
  void insert(T, int);
  //return and remove the last item
  T extract_max();

  //returns a reference to the last item
  T& peek();

  //modify priority of the item at index
  void modify_key(int, int);
  //remove item at index
  void remove(int);

  //returns number of items in the priority queue
  int return_size() const;
  //checks if the queue has items
  bool empty() const;
};


template<typename T>
PriorityQueueHeap<T>::PriorityQueueHeap() {}


template<typename T>
PriorityQueueHeap<T>::PriorityQueueHeap(int prealloc_size) : heap(Heap<Node>(prealloc_size)) {}


template<typename T>
PriorityQueueHeap<T>::~PriorityQueueHeap() {}


template<typename T>
void PriorityQueueHeap<T>::insert(T item, int priority) {
  heap.push(Node(item, priority));
}


template<typename T>
T PriorityQueueHeap<T>::extract_max() {
  return heap.pop().element;
}

template<typename T>
T& PriorityQueueHeap<T>::peek() {
  return heap.peek().element;
}


template<typename T>
void PriorityQueueHeap<T>::modify_key(int index, int new_priority) {
  //we save the old priority for later
  int old_priority = heap[index].priority;
  //we set the new priority
  heap[index].priority = new_priority;
  //if the new priority is larger then we shift the item up
  if(new_priority > old_priority) {
    heap.shift_up(index);
    //and if the new priority is lower the we shift the looser item down
  } else {
    heap.shift_down(index);
  }
}


template<typename T>
void PriorityQueueHeap<T>::remove(int index) {
  //set priority of our node to the highest
  heap[index].priority = heap[0].priority + 1;
  //place it up top
  heap.shift_up(index);
  //and then pop the first item, which is our node
  heap.pop();
}



template<typename T>
int PriorityQueueHeap<T>::return_size() const {
  return heap.len();
}


template<typename T>
bool PriorityQueueHeap<T>::empty() const {
  if(heap.len() == 0)
    return true;
  return false;
}

#endif
