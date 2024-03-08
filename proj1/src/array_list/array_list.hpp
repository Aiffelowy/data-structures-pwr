#ifndef MY_ARRAY_LIST
#define MY_ARRAY_LIST


#include <algorithm>

template <typename T>
struct ArrayList {
private:
  T* buffer;
  int length;
  int capacity;

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
 
  //iterator begin
  T* begin(void);
  //iterator end
  T* end(void);
  //const iterator begin
  T* cbegin(void) const;
  //const iterator end
  T* cend(void) const;

  //push the new item at the end of the array
  void push(T);
  //instert new item at the provided position in the array
  void insert(T, int);
  //removes item in the position
  void remove(int);
  //pop the last item in the array
  T pop(void);
  //expand the array by X cells. If no argument provided it doubles the array size
  void extend(int = 0);
  //resizes the array to provided size - items that dont fit the new size are lost
  void resize(int);
  //returns the size of the array
  int size(void) const;
  //return the length of the array
  int len(void) const;
};

template<typename T>
ArrayList<T>::ArrayList(void) : buffer(nullptr), length(0), capacity(0) {}

template<typename T>
ArrayList<T>::ArrayList(int size) : buffer(new T[size]), length(0), capacity(size) {}

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
  std::copy(other.cbegin(), other.cend(), this->begin());
}

template<typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList& other) {
  if(this == &other) { return *this; }

  this->length = other.length;
  this->size = other.capacity;
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
void ArrayList<T>::push(T item) {
}

template<typename T>
void ArrayList<T>::insert(T item, int position) {}

template<typename T>
void ArrayList<T>::remove(int position) {}

template<typename T>
T ArrayList<T>::pop(void) {}

template<typename T>
void ArrayList<T>::extend(int by) {}

template<typename T>
void ArrayList<T>::resize(int new_size) {}

template<typename T>
T* ArrayList<T>::begin(void) { return &buffer[0]; }

template<typename T>
T* ArrayList<T>::end(void) { return &buffer[length]; }

template<typename T>
T* ArrayList<T>::cbegin(void) const { return &buffer[0]; }

template<typename T>
T* ArrayList<T>::cend(void) const { return &buffer[length]; }

template<typename T>
int ArrayList<T>::size() const { return capacity; }

template<typename T>
int ArrayList<T>::len() const { return length; }

#endif
