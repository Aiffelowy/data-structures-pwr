#ifndef MY_ARRAY_LIST
#define MY_ARRAY_LIST


#include <utility>
template <typename T>
struct ArrayList {
private:
  T* array;
  int length;
  int size;

public:
  //default constructor
  ArrayList(void);
  //default constructor with optional preallocated size 
  ArrayList(int);
  //array like constructor -- im sooo sorry
  template<typename ... Ts>
  ArrayList(Ts && ... values) : array(new T[sizeof...(Ts)]{ std::move(values) ... }), length(sizeof...(Ts)), size(sizeof...(Ts)) {}
  //destructor
  ~ArrayList();
  //copy constructor
  ArrayList(const ArrayList&);
  //copy assignment
  ArrayList& operator=(const ArrayList&);
  //array accessor
  T& operator[](int);
 
  //iterator begin
  T* begin(void);
  //iterator end
  T* end(void);

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
  

  //tests
  int test_s() { return size; }
  int test_l() { return length; }
};

template<typename T>
ArrayList<T>::ArrayList(void) : array(nullptr), length(0), size(0) {}

template<typename T>
ArrayList<T>::ArrayList(int size) : array(new T[size]), length(0), size(size) {}

template<typename T>
ArrayList<T>::~ArrayList(void) { delete [] array; }

template<typename T>
ArrayList<T>::ArrayList(const ArrayList& other) {}

template<typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList& other) {}

template<typename T>
T& ArrayList<T>::operator[](int i) {}

template<typename T>
void ArrayList<T>::push(T item) {}

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
T* ArrayList<T>::begin(void) { return &array[0]; }

template<typename T>
T* ArrayList<T>::end(void) { return &array[length]; }


#endif
