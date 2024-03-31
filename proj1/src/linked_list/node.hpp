#ifndef NODE_H
#define NODE_H

template <typename T>
struct Node {
  T content;
  Node* next;
  Node(T);
  Node* get_next_node(int);
};

template<typename T>
Node<T>::Node(T item) : content(item), next(nullptr) {}

template<typename T>
Node<T>* Node<T>::get_next_node(int position) {
  if(position == 0) { return this; }
  return next->get_next_node(position-1);
}



#endif
