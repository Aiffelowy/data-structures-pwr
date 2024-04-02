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


//why this thing doesnt receive new capacity via the parameter??? idk too lazy to change
template<typename T>
void ArrayList<T>::resize_buffer() {
  //allocate new memory for the buffer
  T* new_buffer = new T[this->capacity];
  //copy items from old to the new buffer ( basically just a better version of the for(int i = 0; i < length; i++) { new[i] = old[i]; } )
  std::copy(this->cbegin(), this->cend(), &new_buffer[0]);
  //delete the old buffer
  delete[] this->buffer;
  //set the buffer pointer to the new buffer
  this->buffer = new_buffer;
}

//default constructor, just set everything to 0
template<typename T>
ArrayList<T>::ArrayList(void) : buffer(nullptr), length(0), capacity(0) {}

//constructor with preallocated size
template<typename T>
ArrayList<T>::ArrayList(int size) : buffer(new T[size]), length(size), capacity(size) {}

//this thing that allows a very nice syntax array = {1, 2, 3, 4}; using a parameter pack expansion and moving all the values from the pack to the allocated buffer
//the requires is a template constraint, all values in the parameter pack need to be able to change into T
template<typename T>
template<typename... Ts> requires((... && std::is_convertible_v<Ts, T>))
ArrayList<T>::ArrayList(Ts&& ... values) : buffer(new T[sizeof...(Ts)]{ std::move(values) ... }), length(sizeof...(Ts)), capacity(sizeof...(Ts)) {}

//destructor, just delete the buffer
template<typename T>
ArrayList<T>::~ArrayList(void) { delete [] this->buffer; }

//copy constructor, so we dont nuke our RAM
template<typename T>
ArrayList<T>::ArrayList(const ArrayList& other) {
  //copy the other arrays length
  this->length = other.length;
  //and copy capacity
  this->capacity = other.capacity;
  //if capacity is 0 then we dont need to create a new buffer
  if(this->capacity == 0) { this->buffer = nullptr; return; }
  //allocate memory for the new buffer
  this->buffer = new T[this->capacity];
  //and copy the items
  std::copy(other.cbegin(), other.cend(), this->cbegin());
}

//copy assignment - to prevent nuclear tests on our RAM
template<typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList& other) {
  //if the other object is just this object, we dont have to do anything
  if(this == &other) { return *this; }
  //copy other arrays length
  this->length = other.length;
  //and capacity
  this->capacity = other.capacity;
  //if we hold some data, we need to free it before replacing
  if(this->buffer != nullptr) {
    delete [] this->buffer;
  }
  if(this->capacity == 0) { this->buffer = nullptr; return *this; }
  this->buffer = new T[this->capacity];
  std::copy(other.cbegin(), other.cend(), this->begin());
  return *this;
}

//so we can use array[i], really easy implementation
template<typename T>
T& ArrayList<T>::operator[](int i) {
  return this->buffer[i];
}

//just a comparison
template<typename T>
bool ArrayList<T>::operator==(const ArrayList<T>& other) const {
  //if the arrays arent holding the same amount of data, then of course they dont hold the same data
  if(this->length != other.length) { return false; }
  //just check if everything is the same
  for(int i = 0; i < this->length; i++) {
    //and if not, well its not the same
    if(this->buffer[i] != other.buffer[i]) { return false; }
  }
  //if we didnt quit from the loop, then everything is the same
  return true;
}

//push data at the end of the buffer
template<typename T>
void ArrayList<T>::push(T item) {
  //if we dont have enough space for the new data, we need to extend our buffer
  if (this->length+1 > this->capacity) { this->extend(); }
  this->length++;
  buffer[length-1] = item;
}

//insert item at the provided index
template<typename T>
void ArrayList<T>::insert(T item, int position) {
  //if position if out of bounds of our current stored data, then exit
  if (position < 0 || position >= this->length) { return; }
  //extend buffer is we can hold this new data
  if (this->length+1 > this->capacity) { this->extend(); }
  this->length++;
  //move all the stuff after the position so we can insert our new data
  for (int i = this->length; i >= position; i--) { this->buffer[i + 1] = this->buffer[i]; }
  //set new data
  this->buffer[position] = item;
}

//remove item at index
template<typename T>
void ArrayList<T>::remove(int position) {
  if (position < 0 || position >= this->length) { return; }
    
  this->length--;
  
  //move all the stuff back, our item will just get overwritten 
  for (int i = position; i < this->length; i++) {
    this->buffer[i] = this->buffer[i + 1];
  }
  //if stuff we holding occupies half of available space, then we shrink the buffer to reclaim memory
  if(this->length <= (this->capacity/2)) { resize(this->capacity/2); }
}

//remove from the end
template<typename T>
T ArrayList<T>::pop(void) {
  //store the thing in the temp value in case it gets deleted by shrinking the buffer
  T temp = this->buffer[length-1];
  this->length--;
  if(this->length <= (this->capacity/2)) { resize(this->capacity/2); }
  return temp;
}

//double capacity and call resize_buffer to well resize the buffer
template<typename T>
void ArrayList<T>::extend() {
    this->capacity *= 2;
    resize_buffer();
}

//add whatever more space user needs and resize
template<typename T>
void ArrayList<T>::extend(int by) {
    this->capacity += by;
    resize_buffer();

}

//resize to the new size
template<typename T>
void ArrayList<T>::resize(int new_size) {
  this->capacity = new_size;
  //if the stuff we are holding is bigger then new size, this stuff will be lost so we adjust the length
  if (this->length > this->capacity) {
    this->length = this->capacity;
  }
  resize_buffer();
}


//return the pointer to the first thing
template<typename T>
T* ArrayList<T>::begin(void) { return &buffer[0]; }
//the same as above but const
template<typename T>
T* ArrayList<T>::begin(void) const { return &buffer[0]; }
//return the pointer to one after the last, because c++ ig
template<typename T>
T* ArrayList<T>::end(void) { return &buffer[length]; }
//the same as above but const
template<typename T>
T* ArrayList<T>::end(void) const { return &buffer[length]; }
//whats the difference between this and the begin() const one? different name ig, c++ spagghet
template<typename T>
T* ArrayList<T>::cbegin(void) const { return &buffer[0]; }
//whats the difference between this and the begin() const one? different name ig, c++ spagghet
template<typename T>
T* ArrayList<T>::cend(void) const { return &buffer[length]; }
//return capacity
template<typename T>
int ArrayList<T>::size() const { return capacity; }
//return length
template<typename T>
int ArrayList<T>::len() const { return length; }

//take a lambda with a custom search logic, and find whatever
template<typename T>
T ArrayList<T>::find(std::function<bool(const T&)> lambda) const {
  //loop over the buffer and return the first thing that passed the custom search logic
  for(const T& item : *this) {
    if(lambda(item)) { return item; }
  }
  //if we didnt find anything return a default constructed value which is bad. Sue me. Too lazy to implement my own option and result. Maybe next project idk
  return T();
}

#endif
