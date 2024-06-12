#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <iostream>
#include <string>
#include <concepts>


namespace Hash1 {

template<typename K>
struct HashGenerator;

template<typename F, typename S>
struct pair {
  F first;
  S second;

  pair(F t): first(t) {}
  pair(F f, S s): first(f), second(s) {}
};

template<typename K, typename T, typename HashGen = HashGenerator<K>>
struct Cuckoo;

template<typename Key, typename Value, typename MapType = Cuckoo<Key, Value>>
struct HashMap {
public:
  struct Node {
    private:
      Node* clone() const;

    public:
      pair<Key, Value> item;
      Node* next = nullptr;

      ~Node();

      Node(const Key& k): item(k) {}
      Node(const Key& k, Value t): item(k ,t), next(nullptr) {}
      Node(pair<Key, Value> p): item(p), next(nullptr) {}
      Node(const Key& k, Value t, Node* n): item(k, t), next(n) {}
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
    Buffer(std::size_t);
    ~Buffer();

    Buffer(const Buffer&);
    Buffer(Buffer&&);

    Buffer& operator=(const Buffer&);
    Buffer& operator=(Buffer&&);

    Node*& operator[](const std::size_t&);

    void free();
    void resize_buffer(const std::size_t&);
    Node** clone_buffer() const;

    void remove(const std::size_t&);
    void emplace(const std::size_t&, Node*);

    std::size_t size() const;
    std::size_t len() const;
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
  Value* find(const Key&);

  std::size_t len() const;
  std::size_t size() const;

  pair<Key, Value>* begin();
  pair<Key, Value>* begin() const;

  pair<Key, Value>* end();
  pair<Key, Value>* end() const;

};





template<typename HashGen, typename Key>
concept has_hash = requires(const Key& key, const int& buffer_size, const int& var) {
  { HashGen::hash(key, buffer_size, var) } -> std::convertible_to<std::size_t>;
};

template<typename MapType, typename Key, typename Value>
concept is_valid_map_t = requires(HashMap<Key, Value, MapType>::Node* node, HashMap<Key, Value, MapType>::Buffer& buffer, const Key& key) {
  { MapType::insert(node, buffer) } -> std::convertible_to<void>;
  { MapType::find(key, buffer) } -> std::convertible_to<pair<typename HashMap<Key, Value, MapType>::Node**, std::size_t>>;
  { MapType::rehash(buffer) } -> std::convertible_to<void>;
};


template<typename Key>
struct HashGenerator{
  static std::size_t hash(const Key&, const int&) = 0;
};

template<>
struct HashGenerator<int> {
  static std::size_t hash(const int& key, const int& buffer_size, const int& hash_seed) {
    int hash = hash_seed;
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
  static std::size_t hash(const std::string& key, const int& bs, const int& hash_seed) {
    int hash = hash_seed;
    for(const char& c : key) {
      hash = ((hash << 5) + hash) + c;
    }
    if(!bs)
      return hash %1;
    return hash % bs; 
  }
};

template<>
struct HashGenerator<long> {
  static pair<std::size_t, std::size_t> hash(const long& key, const int& buffer_size, const int& hash_seed) {
    int hash = 5381;
    std::string key_str = std::to_string(key);
    for(const char& c : key_str) {
      hash = ((hash << 5) + hash) + c;
    }
    if(buffer_size == 0)
      return hash % 1;
    return hash % buffer_size;
  }

};


template<typename K, typename T, typename HashGen>
struct Cuckoo {
  using Buffer = HashMap<K, T, Cuckoo>::Buffer;
  using Node = HashMap<K, T, Cuckoo>::Node;

  static const int TRIES = 20;

  static std::size_t hash(const K& key, const std::size_t& buffer_size, const bool& type) {
    std::size_t buff_half_size = buffer_size/2;
    if(buff_half_size == 0)
      buff_half_size = 1;
    if(type)
      return (HashGen::hash(key, buffer_size, 0) % buff_half_size);
    else
      return (HashGen::hash(key, buffer_size, 0) % buff_half_size) + buff_half_size;

  }

  static Node* put_in_buffer(Node* node, Buffer& buff, const std::size_t& index) {
    if(buff[index] == nullptr) {
      buff.emplace(index, node);
      return nullptr;
    } else if(buff[index]->item.first == node->item.first) {
      buff[index]->item.second = node->item.second;
      return nullptr;
    }

    Node* t = buff[index];
    buff[index] = node;
    return t;
  }

  static void cuc_insert(Node* node, Buffer& buff, int depth) {
    if(depth == 0) {
      buff.resize_buffer(buff.size()*2);
      rehash(buff);
    }
    if(buff.len() > buff.size()*0.3 || buff.size() == 0) {
      if(buff.size() == 0) {
        buff.resize_buffer(2);
        rehash(buff);
      }
      else {
        buff.resize_buffer(buff.size()*2);
        rehash(buff);
      }
    }

    std::size_t hash_result = hash(node->item.first, buff.size(), depth % 2);
        
    Node* pushed_out = put_in_buffer(node, buff, hash_result);
    if(!pushed_out)
      return;
    
    cuc_insert(node, buff, depth-1);
  }

  static void insert(Node* node, Buffer& buffer) {
    cuc_insert(node, buffer, TRIES);
  }

  static void rehash(Buffer& buffer) {
    const Buffer temp(buffer);
    buffer.free();
    buffer.resize_buffer(temp.size());
    for(Node* n : temp) {
      if(n != nullptr) {
        cuc_insert(new Node(n->item.first, n->item.second), buffer, TRIES);
      }
    }
  }

  static pair<Node**, std::size_t> find(const K& key, Buffer& buffer) {
    Node** ret = new Node*[2]();
    std::size_t size = 0;
    Node* n1 = buffer[hash(key, buffer.size(), false)];
    Node* n2 = buffer[hash(key, buffer.size(), true)];
    if(n1 != nullptr) {
      size++;
      ret[0] = n1;
    }
    if(n2 != nullptr) {
      size++;
      ret[1] = n2;
    }
    return pair(ret, size);
  }
};




template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer::Buffer():
  buffer(nullptr), buffer_size(0), length(0) {}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer::Buffer(std::size_t prealloc_size):
  buffer(new Node*[prealloc_size]()), buffer_size(prealloc_size), length(0) {}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Buffer::~Buffer() {
  this->free();
}

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
HashMap<K, T, MapType>::Node*& HashMap<K, T, MapType>::Buffer::operator[](const std::size_t& i) {
  return buffer[i];
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::Buffer::free() {
  if(!buffer)
    return;

  for(int i = 0; i < buffer_size; i++) {
    delete buffer[i];
  }

  delete [] buffer;
  buffer = nullptr;
  buffer_size = 0;
  length = 0;
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Node** HashMap<K, T, MapType>::Buffer::clone_buffer() const {
  Node** new_buffer = new Node*[buffer_size]();
  for(std::size_t i = 0; i < buffer_size; i++)
    if(buffer[i] != nullptr)
      new_buffer[i] = new Node(*buffer[i]);

  return new_buffer;
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::Buffer::resize_buffer(const std::size_t& new_size) {
  Node** new_buffer = new Node*[new_size]();
  if(new_size < length)
    length = new_size;

  if(buffer_size != 0) {
    int funny = 0;
    for(std::size_t i = 0; i < buffer_size; i++) {
      if(buffer[i] != nullptr) {
        new_buffer[funny] = new Node(*buffer[i]);
        funny++;
      }
    }
  }
  this->free();
  buffer_size = new_size;
  buffer = new_buffer;
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::Buffer::emplace(const std::size_t& index, Node* new_node) {
  if(buffer[index] == nullptr) {
    buffer[index] = new_node;
    length++;
  } else {
    delete buffer[index];
    buffer[index] = new_node;
  }
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::Buffer::remove(const std::size_t& position) {
  length--;
  delete buffer[position];
  buffer[position] == nullptr;
}


template<typename K, typename T, typename MapType>
std::size_t HashMap<K, T, MapType>::Buffer::size() const { return buffer_size; }

template<typename K, typename T, typename MapType>
std::size_t HashMap<K, T, MapType>::Buffer::len() const { return length; }

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
  return &buffer[buffer_size];
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::Node** HashMap<K, T, MapType>::Buffer::end() const {
  return &buffer[buffer_size];
}







template<typename Key, typename Value, typename MapType>
HashMap<Key, Value, MapType>::Node::~Node() {
  if(next == nullptr)
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
  static_assert(is_valid_map_t<MapType, K, T>, "\nMapType has to implement:\n 'static void insert(Node*, Buffer&)'\n 'static void rehash(Buffer&)'\n static pair<std::size_t*, std::size_t> hash(const K&)");
}

template<typename K, typename T, typename MapType>
HashMap<K, T, MapType>::HashMap(std::size_t prealloc_size): buffer(prealloc_size) {
  static_assert(is_valid_map_t<MapType, K, T>, "\nMapType has to implement:\n 'static void insert(Node*, Buffer&)'\n 'static void rehash(Buffer&)'\n static pair<std::size_t*, std::size_t> hash(const K&)");
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::insert(const K& key, T value) {
  MapType::insert(new Node(key, value), buffer);
}

template<typename K, typename T, typename MapType>
void HashMap<K, T, MapType>::remove(const K& key) {
  buffer.remove(MapType::hash(key));
}

template<typename K, typename T, typename MapType>
T* HashMap<K, T, MapType>::find(const K& key) {
  pair<Node**, std::size_t> found = MapType::find(key, buffer);
  if(found.second == 0)
    return nullptr;

  for(int i = 0; i < found.second; i++) {
    Node* n = found.first[i];
    if(n != nullptr && n->item.first == key) {
      delete [] found.first;
      return &n->item.second;
    }
  }
  return nullptr;
}

template<typename K, typename T, typename MapType>
T& HashMap<K, T, MapType>::operator[](const K& key) {
  if(buffer.size() == 0)
    buffer.resize_buffer(2);

  T* n = find(key);
  if(n != nullptr)
    return *n;
  else {
    MapType::insert(new Node(key), buffer);
    T* t = find(key);
    return *t;
  }
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
