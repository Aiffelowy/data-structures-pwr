#ifndef PQUEUE_HEAP_H
#define PQUEUE_HEAP_H

#include "heap.hpp"

template<typename T>
struct PriorityQueueHeap {
private:
 struct Node {
    T element;
    int priority;

    Node() {}
    Node(T element, int priotiry) : element(element), priority(priotiry) {}
    bool operator>(const Node& other) { return priority > other.priority; }
    bool operator<(const Node& other) { return priority < other.priority; }
  };

  Heap<Node> heap;

public:
  PriorityQueueHeap();
  PriorityQueueHeap(int);
  ~PriorityQueueHeap();

  void insert(T, int);
  T extract_max();

  T& peek();

  void modify_key(int, int);
  void remove(int);

  int return_size() const;
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
  int old_priority = heap[index].priority;
  heap[index].priority = new_priority;
  if(new_priority > old_priority) {
    heap.shift_up(index);
  } else {
    heap.shift_down(index);
  }
}


template<typename T>
void PriorityQueueHeap<T>::remove(int index) {
  heap[index].priority = heap[0].priority + 1;
  heap.shift_up(index);
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
