#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <algorithm>
#include <functional>


template <typename T>
struct ArrayList {
private:
  T* buffer;
  int length;
  int capacity;

  void resize_buffer();

public:
  //default constructor
  ArrayList(void);
  //default constructor with optional preallocated size 
  ArrayList(int);
  //destructor
  ~ArrayList();
  //copy constructor
  ArrayList(const ArrayList&);
  //copy assignment
  ArrayList& operator=(const ArrayList&);
  //array like constructor -- im sooo sorry
  template<typename ... Ts>  requires((... && std::is_convertible_v<Ts, T>))
  ArrayList(Ts && ... values);
  //array accessor
  T& operator[](int);
  //comparison operator
  bool operator==(const ArrayList&) const;

  //iterator begin
  T* begin(void);
  T* begin(void) const;
  //iterator end
  T* end(void);
  T* end(void) const;
  //const iterator begin
  T* cbegin(void) const;
  //const iterator end
  T* cend(void) const;

  //find
  T find(std::function<bool(const T&)>) const;

  //push the new item at the end of the array
  void push(T);
  //instert new item at the provided position in the array
  void insert(T, int);
  //removes item in the position
  void remove(int);
  //pop the last item in the array
  T pop(void);
  //expand the array by X cells. If no argument provided it doubles the array size
  void extend();
  
  void extend(int);
  //resizes the array to provided size - items that dont fit the new size are lost
  void resize(int);
  //returns the size of the array
  int size(void) const;
  //return the length of the array
  int len(void) const;
};

template<typename T>
void ArrayList<T>::resize_buffer() {
  T* new_buffer = new T[this->capacity];
  std::copy(this->cbegin(), this->cend(), &new_buffer[0]);
  delete[] this->buffer;
  this->buffer = new_buffer;
}

template<typename T>
ArrayList<T>::ArrayList(void) : buffer(nullptr), length(0), capacity(0) {}

template<typename T>
ArrayList<T>::ArrayList(int size) : buffer(new T[size]), length(size), capacity(size) {}

template<typename T>
template<typename... Ts> requires((... && std::is_convertible_v<Ts, T>))
ArrayList<T>::ArrayList(Ts&& ... values) : buffer(new T[sizeof...(Ts)]{ std::move(values) ... }), length(sizeof...(Ts)), capacity(sizeof...(Ts)) {}

template<typename T>
ArrayList<T>::~ArrayList(void) { delete [] buffer; }

template<typename T>
ArrayList<T>::ArrayList(const ArrayList& other) {
  this->length = other.length;
  this->capacity = other.capacity;
  if(this->capacity == 0) { this->buffer = nullptr; return; }

  this->buffer = new T[this->capacity];
  std::copy(other.cbegin(), other.cend(), this->cbegin());
}

template<typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList& other) {
  if(this == &other) { return *this; }

  this->length = other.length;
  this->capacity = other.capacity;
  if(this->buffer != nullptr) {
    delete [] this->buffer;
  }
  if(this->capacity == 0) { this->buffer = nullptr; return *this; }
  this->buffer = new T[this->capacity];
  std::copy(other.cbegin(), other.cend(), this->begin());
  return *this;
}

template<typename T>
T& ArrayList<T>::operator[](int i) {
  return this->buffer[i];
}


template<typename T>
bool ArrayList<T>::operator==(const ArrayList<T>& other) const {
  if(this->length != other.length) { return false; }
  for(int i = 0; i < this->length; i++) {
    if(this->buffer[i] != other.buffer[i]) { return false; }
  }
  return true;
}

template<typename T>
void ArrayList<T>::push(T item) {
    if (this->length+1 > this->capacity) { this->extend(); }
    this->length++;
    buffer[length-1] = item;
}

template<typename T>
void ArrayList<T>::insert(T item, int position) {
    if (position >= this->length) { return; }
    
    if (this->length+1 > this->capacity) { this->extend(); }
    this->length++;
    
    for (int i = this->length; i >= position; i--) { this->buffer[i + 1] = this->buffer[i]; }
    this->buffer[position] = item;
}

template<typename T>
void ArrayList<T>::remove(int position) {
    if (position >= this->length) { return; }
    
    this->length--;
    for (int i = position; i < this->length; i++) {
        this->buffer[i] = this->buffer[i + 1];
    }
    if(this->length <= (this->capacity/2)) { this->capacity /= 2; resize_buffer(); }
}

template<typename T>
T ArrayList<T>::pop(void) {
  T temp = this->buffer[length-1];
  this->length--;
  if(this->length <= (this->capacity/2)) { this->capacity /= 2; resize_buffer(); }
  return temp;
}

template<typename T>
void ArrayList<T>::extend() {
    this->capacity *= 2;
    resize_buffer();
}

template<typename T>
void ArrayList<T>::extend(int by) {
    this->capacity += by;
    resize_buffer();

}

template<typename T>
void ArrayList<T>::resize(int new_size) {
    this->capacity = new_size;
    if (this->length > this->capacity) {
        this->length = this->capacity;
    }
    resize_buffer();
}

template<typename T>
T* ArrayList<T>::begin(void) { return &buffer[0]; }

template<typename T>
T* ArrayList<T>::begin(void) const { return &buffer[0]; }

template<typename T>
T* ArrayList<T>::end(void) { return &buffer[length]; }

template<typename T>
T* ArrayList<T>::end(void) const { return &buffer[length]; }

template<typename T>
T* ArrayList<T>::cbegin(void) const { return &buffer[0]; }

template<typename T>
T* ArrayList<T>::cend(void) const { return &buffer[length]; }

template<typename T>
int ArrayList<T>::size() const { return capacity; }

template<typename T>
int ArrayList<T>::len() const { return length; }

template<typename T>
T ArrayList<T>::find(std::function<bool(const T&)> lambda) const {
  for(const T& item : *this) {
    if(lambda(item)) { return item; }
  }
  return T();
}

#endif
