#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <string>
#include <concepts>


namespace Hash1 {

template<typename K>
struct HashGenerator;

template<typename F, typename S>
struct pair {
  F first;
  S second;

  pair(F f, S s): first(f), second(s) {}
};

template<typename K, typename T, typename HashGen = HashGenerator<K>>
struct Cuckoo;

template<typename Key, typename Value, typename MapType = Cuckoo<Key, Value>>
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

public:
  struct Buffer { 
    Node** buffer;
    std::size_t buffer_size;
    std::size_t length;

    Buffer();
    Buffer(std::size_t);
    ~Buffer();

    Buffer(const Buffer&);
    Buffer(Buffer&&);

    Buffer& operator=(const Buffer&);
    Buffer& operator=(Buffer&&);

    Node* operator[](const std::size_t&);

    void free();
    void resize_buffer(const std::size_t&);
    Node** clone_buffer() const;

    void push(Node*);
    void remove(const std::size_t&);
    Value pop();

    Node** begin();
    Node** begin() const;
    Node** end();
    Node** end() const;
  } buffer;

  HashMap();
  HashMap(std::size_t);

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





template<typename HashGen, typename Key>
concept has_hash = requires(const Key& key, const int& buffer_size) {
  { HashGen::hash(key, buffer_size) } -> std::convertible_to<std::size_t>;
};

template<typename MapType, typename Key, typename Value>
concept is_valid_map_t = requires(const Key& key, Value value, HashMap<Key, Value, MapType>::Buffer& buffer) {
  { MapType::insert(key, value, buffer) } -> std::convertible_to<void>;
  { MapType::remove(key, buffer) } -> std::convertible_to<void>;
  { MapType::find(key, buffer) } -> std::convertible_to<Value&>;
};


template<typename Key>
struct HashGenerator{
  static std::size_t hash(const Key&, const int&) = 0;
};

template<>
struct HashGenerator<int> {
  static std::size_t hash(const int& key, const int& buffer_size) {
    int hash = 42069;
    std::string key_str = std::to_string(key);
    for(const char& c : key_str) {
      hash = ((hash << 5) + hash) + c;
    }
    if(buffer_size == 0)
      return hash % 1;
    return hash % buffer_size;
  }
};

template<>
struct HashGenerator<std::string> {
  static std::size_t hash(const std::string& key, const int& bs) {
    int hash = 42069;
    for(const char& c : key) {
      hash = ((hash << 5) + hash) + c;
    }
    if(!bs)
      return hash %1;
    return hash % bs; 
  }
};


template<typename K, typename T, typename HashGen>
struct Cuckoo {
  using Buffer = HashMap<K, T, Cuckoo>::Buffer;
  static void insert(const K& key, T item, Buffer& buffer) {}
  static void remove(const K&, Buffer&) {}
  static T& find(const K&, Buffer&) {}
};






template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer::Buffer():
  buffer(nullptr), buffer_size(0), length(0) {}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer::Buffer(std::size_t prealloc_size):
  buffer(new Node*[prealloc_size]), buffer_size(prealloc_size), length(0) {}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer::~Buffer() {
  this->free();
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer::Buffer(const Buffer& other):
  buffer_size(other.buffer_size), length(other.length) {
    buffer = other.clone_buffer();
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer::Buffer(Buffer&& other):
 buffer(other.buffer), buffer_size(other.buffer_size), length(other.length) {
   other.buffer = nullptr;
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer& HashMap<K, T, MapType>::Buffer::operator=(const Buffer& other) {
  this->free();
  buffer = other.clone_buffer();
  buffer_size = other.buffer_size;
  length = other.length;
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer& HashMap<K, T, MapType>::Buffer::operator=(Buffer&& other) {
  this->free();
  buffer = other.buffer;
  length = other.length;
  buffer_size = other.buffer_size;
  other.buffer = nullptr;
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Node* HashMap<K, T, MapType>::Buffer::operator[](const std::size_t& i) {
  return buffer[i];
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::Buffer::free() {
  if(!buffer)
    return;

  for(Node** n = &buffer[0]; n != &buffer[length]; n++) {
    delete *n;
  }
  delete [] buffer;
  buffer = nullptr;
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Node** HashMap<K, T, MapType>::Buffer::clone_buffer() const {
  Node** new_buffer = new Node*[buffer_size];
  for(std::size_t i = 0; i < length; i++)
    new_buffer[i] = new Node(*buffer[i]);

  return new_buffer;
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::Buffer::resize_buffer(const std::size_t& new_size) {
  Node** new_buffer = new Node*[new_size];
  if(new_size < length)
    length = new_size;

  for(std::size_t i = 0; i < length; i++)
    new_buffer[i] = new Node(*buffer[i]);

  this->free();
  buffer_size = new_size;
  buffer = new_buffer;
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::Buffer::push(Node* new_node) {
  if(length == buffer_size) {
    if(buffer_size == 0)
      this->resize_buffer(2);
    else
      this->resize_buffer(buffer_size*2);
  }

  buffer[length] = new_node;
  length++;
}

template<typename K, typename T, typename MapType>
T HashMap<K, T, MapType>::Buffer::pop() {
  length--;
  return buffer[length];
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::Buffer::remove(const std::size_t& position) {
  length--;
  for(auto i = position; i < length; i++) {
    buffer[i] = buffer[i+1];
  }
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Node** HashMap<K, T, MapType>::Buffer::begin() {
  return &buffer[0];
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Node** HashMap<K, T, MapType>::Buffer::begin() const {
  return &buffer[0];
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Node** HashMap<K, T, MapType>::Buffer::end() {
  return &buffer[length];
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Node** HashMap<K, T, MapType>::Buffer::end() const {
  return &buffer[length];
}







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
  static_assert(is_valid_map_t<MapType, K, T>, "\nMapType has to implement:\n 'static void insert(const Key&, Value)'\n 'static void remove(const Key&)'\n 'static Value& find(const Key&)'");
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::HashMap(std::size_t prealloc_size): buffer(prealloc_size) {
  static_assert(is_valid_map_t<MapType, K, T>, "\nMapType has to implement:\n 'static void insert(const Key&, Value)'\n 'static void remove(const Key&)'\n 'static Value& find(const Key&)'");
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
