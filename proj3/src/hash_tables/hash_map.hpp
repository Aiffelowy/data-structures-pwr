#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <concepts>

#include "../option.hpp"


namespace Hash1 {

//structure that will generete our hashes, can be specialized for specific data types
template<typename K>
struct HashGenerator;

//just a simple pair
template<typename F, typename S>
struct pair {
  F first;
  S second;

  pair() {}
  //violating some memory safety
  pair(F t): first(t) {}
  pair(F f, S s): first(f), second(s) {}
};

//structure that will define our hash table behavior, this will turn our hashmap into a cuckoo hashing one
template<typename K, typename T, typename HashGen = HashGenerator<K>>
struct Cuckoo;

//a very generic HashMap, should support any type of hashmap (i like template meta programming)
template<typename Key, typename Value, typename MapType = Cuckoo<Key, Value>>
struct HashMap {
public:
  struct Node;
private:
    using BufType = std::shared_ptr<Node>;
public:
  //define our node, can also be used as a simple linked list
  struct Node {
    private:
      //clone the list
      std::shared_ptr<Node> clone() const;

    public:
      pair<Key, Value> item;
      bool occupied;
      //give the ability to make a linked list for closed adressing hashmaps
      std::shared_ptr<Node> next = nullptr;
      //destructor
      ~Node();
      //constructors
      Node(): occupied(false) {}
      Node(const Key& k): item(k), occupied(true) {}
      Node(const Key& k, Value t): item(k ,t), occupied(true), next(nullptr) {}
      Node(pair<Key, Value> p): item(p), occupied(true), next(nullptr) {}
      Node(const Key& k, Value t, std::shared_ptr<Node> n): item(k, t), occupied(true), next(n) {}
      Node(pair<Key, Value> p, std::shared_ptr<Node> n): item(p), occupied(true), next(n) {}

      //copy constructor
      Node(const Node&);
      //move constructor
      Node(Node&&);

      //copy assignment
      Node& operator=(const Node&);
      //move assignment
      Node& operator=(Node&&);
  };
  //a wrapper for the buffer
  struct Buffer { 

    BufType buffer;
    std::size_t buffer_size;
    std::size_t length;
    
    //constructor
    Buffer();
    //constructor with preallocated size
    Buffer(std::size_t);
    //destructor
    ~Buffer();

    //copy constructor
    Buffer(const Buffer&);
    //move constructor
    Buffer(Buffer&&);

    //copy assignment
    Buffer& operator=(const Buffer&);
    //move assignment
    Buffer& operator=(Buffer&&);
    
    //subscript operator
    Node& operator[](const std::size_t&);

    //resize the buffer
    void resize_buffer(const std::size_t&);
    //set everyting to nullptr
    void nullout();
    //clone the buffer
    BufType clone_buffer() const;

    //frees the item and sets nullptr in that place
    void remove(const std::size_t&);
    //replaces the item at a position
    void emplace(const std::size_t&, Node);

    std::size_t size() const;
    std::size_t len() const;
    //iterators
    Node* begin();
    Node* begin() const;
    Node* end();
    Node* end() const;
  } buffer;

  //constructors
  HashMap();
  HashMap(std::size_t);

  //subscript operator, if key is not found creates a new item in that place
  Value& operator[](const Key&);

  //inserts an item
  void insert(const Key&, Value);
  //removes and item
  void remove(const Key&);
  //finds an item
  Option::Option<Value&> find(const Key&);

  std::size_t len() const;
  std::size_t size() const;
};




//i like template meta programming
//this defines what functions our hash structure needs to provide for it to work with our other structures
template<typename HashGen, typename Key>
//input that our structure needs to take
concept has_hash = requires(const Key& key, const int& var) {
  //here we define that the structure has to contain a static function that takes (const Key&, const int&, const int&)
  { HashGen::hash(key, var) } -> std::convertible_to<std::size_t>;
};

//same here, this is a bare minimum what our MapType should provide
template<typename MapType, typename Key, typename Value>
//inputs that our structure needs to take
concept is_valid_map_t = requires(typename HashMap<Key, Value, MapType>::Node node, HashMap<Key, Value, MapType>::Buffer& buffer, const Key& key) {
  //it needs to have a static function named insert that takes a pointer to a Node and a reference to a buffer and returns void
  { MapType::insert(node, buffer) } -> std::convertible_to<void>;
  //this should take a key and the buffer, returns the pointer to an array with possible options as the first pair value and the size of that array as the second value
  { MapType::find(key, buffer) } -> std::convertible_to<pair<std::size_t*, std::size_t>>;
  //this should perform a rehash on our table
  { MapType::rehash(buffer) } -> std::convertible_to<void>;
};

//HashGenerator definition
template<typename Key>
struct HashGenerator{
  static std::size_t hash(const Key&, const int& = 0) = 0;
};

//specialize HashGenerator for ints
template<>
struct HashGenerator<int> {
  static std::size_t hash(const int& key, const int& hash_seed = 0) {
    unsigned long hash = 5381;
    std::string key_str = std::to_string(key);
    for(const char& c : key_str) {
      hash = ((hash << 5) + hash) + c;
    }
  return hash;
  }
};

//specialize HashGenerator for strings
template<>
struct HashGenerator<std::string> {
  static std::size_t hash(const std::string& key, const int& hash_seed = 0) {
    unsigned long hash = 5381;
    for(const char& c : key) {
      hash = ((hash << 5) + hash) + c;
    }
    return hash;
  }
};


//specialize HashGenerator for longs
template<>
struct HashGenerator<long> {
  static pair<std::size_t, std::size_t> hash(const long& key, const int& hash_seed = 0) {
    unsigned long hash = 5381;
    std::string key_str = std::to_string(key);
    for(const char& c : key_str) {
      hash = ((hash << 5) + hash) + c;
    }
    return hash;
  }

};

//black magic
template<typename K, typename T, typename HashGen>
struct Cuckoo {
  using Buffer = HashMap<K, T, Cuckoo>::Buffer;
  using Node = HashMap<K, T, Cuckoo>::Node;

  static constexpr float LOAD_FACTOR = 0.5;
  static constexpr int TRIES = 20;

  //split our buffer in two, for cuckoo hashing
  static std::size_t hash(const K& key, const std::size_t& buffer_size, const bool& type) {
    std::size_t buff_half_size = buffer_size/2;
    if(buff_half_size == 0)
      buff_half_size = 1;
    if(type)
      return (HashGen::hash(key) % buff_half_size);
    return (HashGen::hash(key) % buff_half_size) + buff_half_size;

  }
  //put a thing in the buffer, returns a pointer to the pushed out item
  static pair<bool, Node> put_in_buffer(Node node, Buffer& buff, const std::size_t& index) {
    if(!buff[index].occupied) {
      buff.emplace(index, node);
      return pair(false, Node());
    } else if(buff[index].item.first == node.item.first) {
      buff[index].item.second = node.item.second;
      return pair(false, Node());
    }

    Node t = buff[index];
    buff[index] = node;
    return pair(true, t);
  }
  //black magic - basically we put a new thing in the first part of the array
  //and if we had a collision we take the old item out the first array and put in the other array using a different hash algoritm. if that collides we push out the old item from the second array and we try to insert it in the first one
  //we try this until we find a place to put all items, if not, we rehash the table
  static void cuc_insert(Node node, Buffer& buff, int depth) {
    if(depth == 0) {
      buff.resize_buffer(buff.size()*2);
      cuc_insert(node, buff, TRIES);
      return;
    }

    if(buff.len() > buff.size()*LOAD_FACTOR || buff.size() == 0) {
      if(buff.size() == 0) {
        buff.resize_buffer(2);
      }
      else {
        buff.resize_buffer(buff.size()*2);
      }
    }

    std::size_t hash_result = hash(node.item.first, buff.size(), depth % 2);
        
    auto pushed_out = put_in_buffer(node, buff, hash_result);
    if(!pushed_out.first)
      return;
    
    cuc_insert(pushed_out.second, buff, depth-1);
  }
  //call the cuckoo_insert;
  static void insert(Node node, Buffer& buffer) {
    cuc_insert(node, buffer, TRIES);
  }

  static void rehash(Buffer& buffer) {
    const Buffer temp(buffer);
    buffer.nullout();
    for(const auto& n : temp) {
      if(n.occupied) {
        cuc_insert(Node(n.item), buffer, TRIES);
      }
    }
  }

  static pair<std::size_t*, std::size_t> find(const K& key, Buffer& buffer) {
    std::size_t* ret = new size_t[2]();
    std::size_t size = 0;

    std::size_t h1 = hash(key, buffer.size(), false);
    std::size_t h2 = hash(key, buffer.size(), true);

    if(buffer[h1].occupied) {
      ret[0] = h1;
      size++;
    }
    if(buffer[h2].occupied) {
      ret[1] = h2;
      size++;
    }

    return pair(ret, size);
  }
};




template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer::Buffer():
  buffer(nullptr), buffer_size(0), length(0) {}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer::Buffer(std::size_t prealloc_size):
  buffer(new std::shared_ptr<Node>[prealloc_size], std::default_delete<std::shared_ptr<Node>[]>()), buffer_size(prealloc_size), length(0) {}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer::~Buffer() {}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer::Buffer(const Buffer& other):
  buffer(other.clone_buffer()), buffer_size(other.buffer_size), length(other.length) {}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer::Buffer(Buffer&& other):
 buffer(other.buffer), buffer_size(other.buffer_size), length(other.length) {
   other.buffer = nullptr;
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer& HashMap<K, T, MapType>::Buffer::operator=(const Buffer& other) {
  buffer = other.clone_buffer();
  buffer_size = other.buffer_size;
  length = other.length;
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer& HashMap<K, T, MapType>::Buffer::operator=(Buffer&& other) {
  buffer = other.buffer;
  length = other.length;
  buffer_size = other.buffer_size;
  other.buffer = nullptr;
}

template<typename K, typename T, typename MapType>
typename HashMap<K, T, MapType>::Node& HashMap<K, T, MapType>::Buffer::operator[](const std::size_t& i) {
  return buffer.get()[i];
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::Buffer::nullout() {
  for(Node& n : *this) {
    if(n.occupied)
      n = Node();
  }
  length = 0;
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::BufType HashMap<K, T, MapType>::Buffer::clone_buffer() const {
  BufType new_buffer = BufType(new Node[buffer_size], std::default_delete<Node[]>());
  for(std::size_t i = 0; i < buffer_size; i++)
    if(buffer.get()[i].occupied)
      new_buffer.get()[i] = Node(buffer.get()[i]);

  return new_buffer;
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::Buffer::resize_buffer(const std::size_t& new_size) {
  BufType new_buffer = BufType(new Node[new_size], std::default_delete<Node[]>());
  if(new_size < length)
    length = new_size;

  if(buffer_size != 0) {
    int funny = 0;
    for(std::size_t i = 0; i < buffer_size; i++) {
      if(buffer.get()[i].occupied) {
        new_buffer.get()[funny] = Node(buffer.get()[i]);
        funny++;
      }
    }
  }
  buffer_size = new_size;
  buffer = new_buffer;
  MapType::rehash(*this);
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::Buffer::emplace(const std::size_t& index, Node new_node) {
  if(!buffer.get()[index].occupied)
    length++;

  new_node.occupied = true;
  buffer.get()[index] = new_node;
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::Buffer::remove(const std::size_t& position) {
  length--;
  buffer[position] == nullptr;
}


template<typename K, typename T, typename MapType>
std::size_t HashMap<K, T, MapType>::Buffer::size() const { return buffer_size; }

template<typename K, typename T, typename MapType>
std::size_t HashMap<K, T, MapType>::Buffer::len() const { return length; }

template<typename K, typename T, typename MapType>
typename HashMap<K, T, MapType>::Node* HashMap<K, T, MapType>::Buffer::begin() {
  return &buffer.get()[0];
}

template<typename K, typename T, typename MapType>
typename HashMap<K, T, MapType>::Node* HashMap<K, T, MapType>::Buffer::begin() const {
  return &buffer.get()[0];
}

template<typename K, typename T, typename MapType>
typename HashMap<K, T, MapType>::Node* HashMap<K, T, MapType>::Buffer::end() {
  return &buffer.get()[buffer_size];
}

template<typename K, typename T, typename MapType>
typename HashMap<K, T, MapType>::Node* HashMap<K, T, MapType>::Buffer::end() const {
  return &buffer.get()[buffer_size];
}







template<typename Key, typename Value, typename MapType>
HashMap<Key, Value, MapType>::Node::~Node() {
  //delete next; //this, now this is some smart behavior right here: delete calls the destructor of the object to delete so it will recursively call the destructor on the whole list; we dont even need to check if the pointer is null because the delete operator does this check anyway
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Node::Node(const Node& other):
  item(other.item), occupied(other.occupied), next(other.clone()) {}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Node::Node(Node&& other):
  item(other.item), occupied(other.occupied), next(other.next) {
    other.next = nullptr;
  }

template<typename K, typename T, typename MapType>
typename HashMap<K, T, MapType>::Node& HashMap<K, T, MapType>::Node::operator=(const Node& other) {
  item = other.item;
  occupied = other.occupied;
  next = other.clone();

  return *this;
}

template<typename K, typename T, typename MapType>
typename HashMap<K, T, MapType>::Node& HashMap<K, T, MapType>::Node::operator=(Node&& other) {
  item = other.item;
  occupied = other.occupied;
  next = other.next;
  other.next = nullptr;

  return *this;
}

template<typename K, typename T, typename MapType>
std::shared_ptr<typename HashMap<K, T, MapType>::Node> HashMap<K, T, MapType>::Node::clone() const {
  if(next != nullptr)
    return std::make_shared<Node>(*next);
  return nullptr;
}










template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::HashMap() {
  static_assert(is_valid_map_t<MapType, K, T>, "\nMapType has to implement:\n 'static void insert(Node*, Buffer&)'\n 'static void rehash(Buffer&)'\n static pair<std::size_t*, std::size_t> hash(const K&)");
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::HashMap(std::size_t prealloc_size): buffer(prealloc_size) {
  static_assert(is_valid_map_t<MapType, K, T>, "\nMapType has to implement:\n 'static void insert(Node*, Buffer&)'\n 'static void rehash(Buffer&)'\n static pair<std::size_t*, std::size_t> hash(const K&)");
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::insert(const K& key, T value) {
  MapType::insert(Node(key, value), buffer);
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::remove(const K& key) {
  auto found = MapType::find(key, buffer);

  for(int i = 0; i < found.second; i++) {
    if(buffer[found.first[i]] != nullptr && buffer[found.first[i]]->item.first == key)
      buffer.remove(found.first[i]);
  }
  delete [] found.first;
}

template<typename K, typename T, typename MapType>
Option::Option<T&> HashMap<K, T, MapType>::find(const K& key) {
  auto found = MapType::find(key, buffer);

  for(int i = 0; i < found.second; i++) {
    if(buffer[found.first[i]].occupied && buffer[found.first[i]].item.first == key) {
      std::size_t temp = found.first[i];
      delete [] found.first;
      return Option::Some(&buffer[temp].item.second);
    }
  }
  delete [] found.first;
  return Option::None();
}

template<typename K, typename T, typename MapType>
T& HashMap<K, T, MapType>::operator[](const K& key) {
  if(buffer.size() == 0)
    buffer.resize_buffer(2);

  Option::Option<T&> n = find(key);
  if(!n.is_none())
    return n.unwrap();
  else {
    MapType::insert(Node(key), buffer);
    Option::Option<T&> t = find(key);
    return t.unwrap();
  }
}

template<typename K, typename T, typename MapType>
std::size_t HashMap<K, T, MapType>::len() const { return buffer.length; }

template<typename K, typename T, typename MapType>
std::size_t HashMap<K, T, MapType>::size() const { return buffer.buffer_size; }

}

#endif // !HASH_TABLE_HPP
