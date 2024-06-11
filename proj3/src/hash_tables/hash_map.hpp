#include <algorithm>
#ifndef HASH_TABLE_HPP

#include <cstddef>


namespace Hash1 {

template<typename K>
struct hash {
  std::size_t operator()(const K& k) = 0;
};

template<typename F, typename S>
struct pair {
  F first;
  S second;

  pair(F f, S s): first(f), second(s) {}
};

template<typename K, typename T, typename MapType, typename HashGen = hash<K>>
struct HashMap {
private:
  struct Node {
    private:
      Node* clone() const;

    public:
      pair<K, T> item;
      Node* next = nullptr;

      ~Node();

      Node(K k, T t, Node* n): item(k, t), next(n) {}
      Node(pair<K, T> p, Node* n): item(p), next(n) {}

      Node(const Node&);
      Node(Node&&);

      Node& operator=(const Node&);
      Node& operator=(Node&&);
  };

  Node** buffer;
  std::size_t buffer_size;
  std::size_t length;

  void free_buffer();
  static Node** clone_buffer(Node**, std::size_t, std::size_t);

public:
  ~HashMap();

  HashMap();
  HashMap(const HashMap&);
  HashMap(HashMap&&);

  HashMap& operator=(const HashMap&);
  HashMap& operator=(HashMap&&);

  T& operator[](const K&);

  void insert(K, T);
  void remove(K);

  std::size_t len() const;
  std::size_t size() const;

  pair<K, T>* begin();
  pair<K, T>* begin() const;

  pair<K, T>* end();
  pair<K, T>* end() const;

};


template<typename K, typename T, typename MapType, typename HashGen>
HashMap<K, T, MapType, HashGen>::Node::~Node() {
  delete next; //this, now this is some smart behavior right here: delete calls the destructor of the object to delete so it will recursively call the destructor on the whole list; we dont even need to check if the pointer is null because the delete operator does this check anyway
}

template<typename K, typename T, typename MapType, typename HashGen>
HashMap<K, T, MapType, HashGen>::Node::Node(const Node& other):
  item(other.item), next(other.clone()) {}

template<typename K, typename T, typename MapType, typename HashGen>
HashMap<K, T, MapType, HashGen>::Node::Node(Node&& other):
  item(other.item), next(other.next) {
    other.next = nullptr;
  }

template<typename K, typename T, typename MapType, typename HashGen>
typename HashMap<K, T, MapType, HashGen>::Node& HashMap<K, T, MapType, HashGen>::Node::operator=(const Node& other) {
  item = other.item;
  delete next;
  next = other.clone();
}

template<typename K, typename T, typename MapType, typename HashGen>
typename HashMap<K, T, MapType, HashGen>::Node& HashMap<K, T, MapType, HashGen>::Node::operator=(Node&& other) {
  item = other.item;
  delete next;
  next = other.next;
  other.next = nullptr;
}

template<typename K, typename T, typename MapType, typename HashGen>
typename HashMap<K, T, MapType, HashGen>::Node* HashMap<K, T, MapType, HashGen>::Node::clone() const {
  if(next)
    return new Node(*next);
  return nullptr;
}

template<typename K, typename T, typename MapType, typename HashGen>
HashMap<K, T, MapType, HashGen>::~HashMap() {
  free_buffer();
}

template<typename K, typename T, typename MapType, typename HashGen>
HashMap<K, T, MapType, HashGen>::HashMap(): buffer(nullptr), buffer_size(0), length(0) {}

template<typename K, typename T, typename MapType, typename HashGen>
HashMap<K, T, MapType, HashGen>::HashMap(const HashMap& other): buffer_size(other.buffer_size), length(other.length) {
  buffer = clone_buffer(other.buffer, length, buffer_size);
}

template<typename K, typename T, typename MapType, typename HashGen>
HashMap<K, T, MapType, HashGen>::HashMap(HashMap&& other): buffer(other.buffer), buffer_size(other.buffer_size), length(other.length) {
  other.buffer = nullptr;
}

template<typename K, typename T, typename MapType, typename HashGen>
HashMap<K, T, MapType, HashGen>& HashMap<K, T, MapType, HashGen>::operator=(const HashMap& other) {
  buffer_size = other.buffer_size;
  length = other.length;
  free_buffer();
  buffer = clone_buffer(other.buffer, length, buffer_size);
} 

template<typename K, typename T, typename MapType, typename HashGen>
HashMap<K, T, MapType, HashGen>& HashMap<K, T, MapType, HashGen>::operator=(HashMap&& other) {
  buffer_size = other.buffer_size;
  length = other.length;
  free_buffer();
  buffer = other.buffer;
  other.buffer = nullptr;
}

template<typename K, typename T, typename MapType, typename HashGen>
void HashMap<K, T, MapType, HashGen>::free_buffer() {
  if(!buffer)
    return;

  for(Node* n = &buffer[0]; n != buffer[length]; n++) {
    delete n;
  }
  delete [] buffer;
}

template<typename K, typename T, typename MapType, typename HashGen>
typename HashMap<K, T, MapType, HashGen>::Node** HashMap<K, T, MapType, HashGen>::clone_buffer(Node** other, std::size_t len, std::size_t size) {
  Node** buffer = new Node*[size];
  for(int i = 0; i < len; i++) {
    buffer[i] = new Node(*other[i]);
  }
  return buffer;
}

template<typename K, typename T, typename MapType, typename HashGen>
std::size_t HashMap<K, T, MapType, HashGen>::len() const { return length; }

template<typename K, typename T, typename MapType, typename HashGen>
std::size_t HashMap<K, T, MapType, HashGen>::size() const { return buffer_size; }


template<typename K, typename T, typename MapType, typename HashGen>
pair<K ,T>* HashMap<K, T, MapType, HashGen>::begin() { return buffer[0]; }


template<typename K, typename T, typename MapType, typename HashGen>
pair<K ,T>* HashMap<K, T, MapType, HashGen>::begin() const { return buffer[0]; }


template<typename K, typename T, typename MapType, typename HashGen>
pair<K ,T>* HashMap<K, T, MapType, HashGen>::end() { return buffer[length]; }


template<typename K, typename T, typename MapType, typename HashGen>
pair<K ,T>* HashMap<K, T, MapType, HashGen>::end() const { return buffer[length]; }

}

#endif // !HASH_TABLE_HPP
