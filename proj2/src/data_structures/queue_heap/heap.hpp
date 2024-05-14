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

  void insert_to_buffer(T);
  void remove_from_buffer();

  void resize(int);
  void expand_buffer();
  void expand_buffer(int);

  constexpr int parent(int i);
  constexpr int left_child(int i);
  constexpr int right_child(int i);

public:
  Heap();

  Heap(int);

  Heap(const Heap&);

  Heap& operator=(const Heap&);

  T& operator[](int index) {
    return buffer[index];
  }

  ~Heap();
  
  void shift_up(int i);
  
  void shift_down(int i);

  void push(T);

  T pop();

  T& peek();

  int len() const;

  bool empty() const;
};


template<typename T>
Heap<T>::Heap() {}


template<typename T>
Heap<T>::Heap(int prealloc_size) : size(prealloc_size), buffer(new T[prealloc_size]) {}


template<typename T>
Heap<T>::Heap(const Heap& other) : size(other.size), length(other.length), buffer(new T[other.size]) {
  if(other.size == 0) {
    buffer = nullptr;
    return;
  }
  std::copy(&other.buffer[0], &other.buffer[other.length], &this->buffer[0]);
}


template<typename T>
Heap<T>& Heap<T>::operator=(const Heap& other) {
  if(this == &other)
    return *this;

  if(this->buffer != nullptr)
    delete [] buffer;

  this->size = other.size;
  this->length = other.length;
  
  if(this->size == 0) {
    this->buffer = nullptr;
    return *this;
  }

  this->buffer = new T[other.size];
  std::copy(&other.buffer[0], other.buffer[other.length], &this->buffer[0]);
  return *this;
}


template<typename T>
void Heap<T>::resize(int new_size) {
  size = new_size;
  T* new_buffer = new T[size];

  std::copy(&buffer[0], &buffer[length], &new_buffer[0]);
  
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
  delete [] buffer;
}


template<typename T>
void Heap<T>::insert_to_buffer(T node) {
  if(buffer == nullptr) {
    buffer = new T[1];
    size = 1;
  }

  if(length == size) {
    expand_buffer();
  }
  buffer[length] = node;
  length++;
}


template<typename T>
void Heap<T>::remove_from_buffer() {
  length--;
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
  insert_to_buffer(item);
  shift_up(length-1);
}


template<typename T>
T Heap<T>::pop() {
  T item = buffer[0];

  buffer[0] = buffer[length-1];
  remove_from_buffer();

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
