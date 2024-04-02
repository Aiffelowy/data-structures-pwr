#ifndef NODE_H
#define NODE_H

template <typename T>
struct Node {
  T content;
  Node* next;
  Node(T);
  Node* get_next_node(int);
};

//constructor, put the item in the node
template<typename T>
Node<T>::Node(T item) : content(item), next(nullptr) {}

//recursive way of getting nodes. recursion is basically just a cooler for loop
template<typename T>
Node<T>* Node<T>::get_next_node(int position) {
  //if position is 0, then we want this node.
  if(position == 0) { return this; }
  //if not, then maybe the next node will be more lucky
  return next->get_next_node(position-1);
}



#endif
