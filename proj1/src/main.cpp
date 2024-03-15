#include "array_list/array_list.hpp"
#include <iostream>

template<typename T>
void print_array_copy(ArrayList<T> arr) {
  std::cout << "len: " << arr.len() << " size: " << arr.size() << std::endl;
  for(T item : arr) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
}




int main(int argc, char *argv[]) {
  ArrayList<int> arr1 = {1, 2, 3, 4, 5};
  arr1.resize(2);
  print_array_copy(arr1);

  return 0;
}
