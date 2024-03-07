#ifndef MY_DOUBLE_LINKED_LIST
#define MY_DOUBLE_LINKED_LIST

template<typename T>
struct DoubleNode {
  T content;
  DoubleNode* next;
  DoubleNode* previous;
};


template<typename T>
struct DoubleLinkedList {
private:
  DoubleNode<T>* head;
  DoubleNode<T>* tail;
  
public:
  //constructor
  DoubleLinkedList(void);
  //destructor
  ~DoubleLinkedList(void);
  //copy constructor
  DoubleLinkedList(const DoubleLinkedList&);
  //copy assignment
  DoubleLinkedList& operator=(const DoubleLinkedList&);
  //array accessor
  T& operator[](int);

  //iterator
  template<typename F>
  void for_each(F&& lambda);

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
DoubleLinkedList<T>::DoubleLinkedList(void) {}

template<typename T>
DoubleLinkedList<T>::DoubleLinkedList(const DoubleLinkedList&) {}

template<typename T>
DoubleLinkedList<T>::~DoubleLinkedList(void) {}

template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(const DoubleLinkedList&) {}

template<typename T>
T& DoubleLinkedList<T>::operator[](int i) {}

template<typename T>
void DoubleLinkedList<T>::push(T item) {}

template<typename T>
void DoubleLinkedList<T>::insert(T item, int position) {}

template<typename T>
void DoubleLinkedList<T>::remove(int position) {}

template<typename T>
T DoubleLinkedList<T>::pop(void) {}


#endif
