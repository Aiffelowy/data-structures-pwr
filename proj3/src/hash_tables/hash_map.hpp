#include <algorithm>
#include <concepts>
#ifndef HASH_TABLE_HPP

#include <cstddef>


namespace Hash1 {

template<typename K>
struct HashGenerator {
  static std::size_t hash(const K& k) = 0;
};


template<typename F, typename S>
struct pair {
  F first;
  S second;

  pair(F f, S s): first(f), second(s) {}
};

template<typename Key, typename Value, typename MapType>
struct HashMap {
private:
  struct Node {
    private:
      Node* clone() const;

    public:
      pair<Key, Value> item;
      Node* next = nullptr;

      ~Node();

      Node(Key k, Value t, Node* n): item(k, t), next(n) {}
      Node(pair<Key, Value> p, Node* n): item(p), next(n) {}

      Node(const Node&);
      Node(Node&&);

      Node& operator=(const Node&);
      Node& operator=(Node&&);
  };

  struct Buffer { 
    Node** buffer;
    std::size_t buffer_size;
    std::size_t length;

    Buffer();
    ~Buffer();

    Buffer(const Buffer&);
    Buffer(Buffer&&);

    Buffer& operator=(const Buffer&);
    Buffer& operator=(Buffer&&);

    Node* operator[](const std::size_t&);

    void free();
    Node** clone_buffer() const;

    Node** begin();
    Node** begin() const;
    Node** end();
    Node** end() const;
  } buffer;

public:
  HashMap();

  Value& operator[](const Key&);

  void insert(const Key&, Value);
  void remove(const Key&);

  std::size_t len() const;
  std::size_t size() const;

  pair<Key, Value>* begin();
  pair<Key, Value>* begin() const;

  pair<Key, Value>* end();
  pair<Key, Value>* end() const;

};






template<typename Key, typename Value, typename MapType>
HashMap<Key, Value, MapType>::Node::~Node() {
  delete next; //this, now this is some smart behavior right here: delete calls the destructor of the object to delete so it will recursively call the destructor on the whole list; we dont even need to check if the pointer is null because the delete operator does this check anyway
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Node::Node(const Node& other):
  item(other.item), next(other.clone()) {}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Node::Node(Node&& other):
  item(other.item), next(other.next) {
    other.next = nullptr;
  }

template<typename K, typename T, typename MapType>
typename HashMap<K, T, MapType>::Node& HashMap<K, T, MapType>::Node::operator=(const Node& other) {
  item = other.item;
  delete next;
  next = other.clone();
}

template<typename K, typename T, typename MapType>
typename HashMap<K, T, MapType>::Node& HashMap<K, T, MapType>::Node::operator=(Node&& other) {
  item = other.item;
  delete next;
  next = other.next;
  other.next = nullptr;
}

template<typename K, typename T, typename MapType>
typename HashMap<K, T, MapType>::Node* HashMap<K, T, MapType>::Node::clone() const {
  if(next)
    return new Node(*next);
  return nullptr;
}










template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::HashMap() {
}


template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::insert(const K& key, T value) {
  MapType::insert(key, value, buffer);
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::remove(const K& key) {
  MapType::remove(key, buffer);
}

template<typename K, typename T, typename MapType>
T& HashMap<K, T, MapType>::operator[](const K& key) {
  return MapType::find(key, buffer);
}

template<typename K, typename T, typename MapType>
std::size_t HashMap<K, T, MapType>::len() const { return buffer.length; }

template<typename K, typename T, typename MapType>
std::size_t HashMap<K, T, MapType>::size() const { return buffer.buffer_size; }


template<typename K, typename T, typename MapType>
pair<K ,T>* HashMap<K, T, MapType>::begin() { return buffer.begin(); }


template<typename K, typename T, typename MapType>
pair<K ,T>* HashMap<K, T, MapType>::begin() const { return buffer.begin(); }


template<typename K, typename T, typename MapType>
pair<K ,T>* HashMap<K, T, MapType>::end() { return buffer.end(); }


template<typename K, typename T, typename MapType>
pair<K ,T>* HashMap<K, T, MapType>::end() const { return buffer.end(); }

}

#endif // !HASH_TABLE_HPP
