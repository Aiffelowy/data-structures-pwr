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
  DoubleLinkedList();
  ~DoubleLinkedList();
  T pop();
  void push(T item);
  void insert(T item, int place);
  void remove(int place);
};


#endif
