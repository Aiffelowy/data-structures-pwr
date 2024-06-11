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

template<typename K, typename T, typename HashGen = hash<K>>
struct HashMap {
private:
  struct Node {
    pair<K, T> item;
    Node* next;

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

}



#endif // !HASH_TABLE_HPP
