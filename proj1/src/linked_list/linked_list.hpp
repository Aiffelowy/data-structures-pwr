#ifndef MY_LINKED_LIST
#define MY_LINKED_LIST

template <typename T>
struct Node {
  T content;
  Node* next;
};

template <typename T>
struct LinkedList {
private:
  Node<T>* head;
  int length;

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

  //iterator
  template<typename F>
  void for_each(F&& lambda) {}

  //push new item at the end of the list
  void push(T);
  //insert new item at the provided position in the list
  void insert(T, int);
  //remove item at the position in the list
  void remove(int);
  //pop the last item on the list
  T pop(void);
};

template<typename T>
LinkedList<T>::LinkedList(void) {}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList&) {}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList&) {}

template<typename T>
T& LinkedList<T>::operator[](int i) {}

template<typename T>
void LinkedList<T>::push(T item) {}

template<typename T>
void LinkedList<T>::insert(T item, int position) {}

template<typename T>
void LinkedList<T>::remove(int position) {}

template<typename T>
T LinkedList<T>::pop(void) {}


#endif
