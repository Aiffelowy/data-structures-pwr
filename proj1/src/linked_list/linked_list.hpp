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
  LinkedList();
  ~LinkedList();
  void push(T item);
  void insert(T item, int place);
  void remove(int place);
  T pop();
  T* operator[](int i);
};



#endif
