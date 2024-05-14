#ifndef HEAP_H
#define HEAP_H

#include <algorithm>
#include <utility>

template<typename T>
struct Heap {
private: 
 
  int size = 0;
  int length = 0;
  T* buffer = nullptr;

  //function that inserts an item at the end of internal buffer
  void insert_to_buffer(T);
  //fuction to remove the last item in internal buffer
  void remove_from_buffer();

  //resized the internal buffer to the desired size
  void resize(int);
  //doubles the size of the internal buffer
  void expand_buffer();
  //expands the internal buffer by x spaces
  void expand_buffer(int);

  //returns the index of the parent for the given index of an item
  constexpr int parent(int i);
  //returns the index of the left child of an item
  constexpr int left_child(int i);
  //returns the index of the right child of an item
  constexpr int right_child(int i);

public:
  //default constructor
  Heap();
  //constructor that preallocates some space
  Heap(int);
  //copy constructor
  Heap(const Heap&);
  //assignment operator
  Heap& operator=(const Heap&);
  //subscript operator
  T& operator[](int index) {
    return buffer[index];
  }
  //destructor
  ~Heap();
  //shift the node up the tree
  void shift_up(int i);
  //shift the node down the tree
  void shift_down(int i);
  //push the new item on the heap
  void push(T);
  //remove and return the first item from the heap
  T pop();
  //return the reference to the first item on the heap
  T& peek();
  //returns the number of items on the heap
  int len() const;
  //checks if there are items on the heap
  bool empty() const;
};


template<typename T>
Heap<T>::Heap() {}


template<typename T>
Heap<T>::Heap(int prealloc_size) : size(prealloc_size), buffer(new T[prealloc_size]) {}


template<typename T>
Heap<T>::Heap(const Heap& other) : size(other.size), length(other.length) {
  //if we are copying empty heap, theres no need to allocate memory just yet
  if(other.size == 0) {
    this->buffer = nullptr;
    return;
  }
  //allocate space for our items
  this->buffer = new T[other.size];
  //copy things from the other buffer to our buffer
  std::copy(&other.buffer[0], &other.buffer[other.length], &this->buffer[0]);
}


template<typename T>
Heap<T>& Heap<T>::operator=(const Heap& other) {
  //no need to copy ourselves
  if(this == &other)
    return *this;
  //destroy the current buffer so we dont leak any memory
  if(this->buffer != nullptr)
    delete [] buffer;
  
  this->size = other.size;
  this->length = other.length;
  //if the other buffer is empty theres no need to alloc memory
  if(this->size == 0) {
    this->buffer = nullptr;
    return *this;
  }
  //alloc some space
  this->buffer = new T[other.size];
  //copy buffer
  std::copy(&other.buffer[0], other.buffer[other.length], &this->buffer[0]);
  return *this;
}


template<typename T>
void Heap<T>::resize(int new_size) {
  size = new_size;
  //alloc space for the new buffer
  T* new_buffer = new T[size];
  //copy items from the other buffer to this buffer
  std::copy(&buffer[0], &buffer[length], &new_buffer[0]);
  //dealloc the old buffer so we dont leak memory
  delete [] buffer;
  buffer = new_buffer;
}


template<typename T>
void Heap<T>::expand_buffer(int by) {
  resize(size+by);
}


template<typename T>
void Heap<T>::expand_buffer() {
  resize(size*2);
}


template<typename T>
Heap<T>::~Heap() {
  //free memory
  delete [] buffer;
}


template<typename T>
void Heap<T>::insert_to_buffer(T node) {
  //if our buffer is empty we need to alloc some memory first
  if(buffer == nullptr) {
    buffer = new T[1];
    size = 1;
  }
  //were about to overflow, we need to increase the buffer size
  if(length == size) {
    expand_buffer();
  }
  //assign item at the end of the buffer
  buffer[length] = node;
  length++;
}


template<typename T>
void Heap<T>::remove_from_buffer() {
  //decreasing the length is enough, that memory will be overwritten by the next inserted node
  length--;
  //we arent using that much space, lets free it
  if(length < size/2) {
    resize(size/2);
    return;
  }
}


template<typename T>
constexpr int Heap<T>::parent(int i) {
  return (i - 1) / 2;
}


template<typename T>
constexpr int Heap<T>::left_child(int i) {
  return ((2 * i) + 1);
}


template<typename T>
constexpr int Heap<T>::right_child(int i) {
  return ((2 * i) + 2);
}


template<typename T>
void Heap<T>::shift_up(int i) {
  while(i > 0 && buffer[parent(i)] < buffer[i]) {
    std::swap(buffer[parent(i)], buffer[i]);
    i = parent(i);
  }
}


template<typename T>
void Heap<T>::shift_down(int i) {
  int max_i = i;
  
  int left_kid = left_child(i);
  if(left_kid < length && buffer[left_kid] > buffer[max_i])
    max_i = left_kid;

  int right_kid = right_child(i);
  if(right_kid < length && buffer[right_kid] > buffer[max_i])
    max_i = right_kid;

  if(i != max_i) {
    std::swap(buffer[i], buffer[max_i]);
    shift_down(max_i);
  }
}



template<typename T>
void Heap<T>::push(T item) {
  //insert the item at the end
  insert_to_buffer(item);
  //and shift it to its correct place
  shift_up(length-1);
}


template<typename T>
T Heap<T>::pop() {
  //make a copy of our first object
  T item = buffer[0];
  //overwrite the first object with the last one, we dont need it anymore
  buffer[0] = buffer[length-1];
  //remove the old copy of the last object
  remove_from_buffer();
  //and shift that object to its correct place
  shift_down(0);
  return item;
}

template<typename T>
T& Heap<T>::peek() {
  return buffer[0];
}

template<typename T>
int Heap<T>::len() const {
  return length;
}


template<typename T>
bool Heap<T>::empty() const {
  if(length == 0)
    return true;
  return false;
}

#endif
