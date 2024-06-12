#include "hash_tables/hash_map.hpp"

using Node = Hash1::HashMap<int, int, Hash1::Cuckoo<int, int>>::Node;

int main(int argc, char *argv[]) {
  Hash1::HashMap<int, int, Hash1::Cuckoo<int, int>> map;

  map.buffer.resize_buffer(64);
  
  for(int i = 0; i < 50; i++) {
    map[i] = i;
  }

  map.buffer.resize_buffer(128);
  Hash1::Cuckoo<int, int>::rehash(map.buffer);
  map.buffer.resize_buffer(256);
  Hash1::Cuckoo<int, int>::rehash(map.buffer);

  for(int i = 0; i < 50; i++) {
      std::cout << map[i] << "\n";
  }
}
