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
//sus


int main(int argc, char *argv[]) {
  ArrayList<int> arr1 = {1 ,2 ,3 ,4, 5, 6};
  ArrayList<std::string> arr2 = {"abc", "def", "sus"};
  
  print_array_copy(arr1);
  print_array_copy(arr2);
  
  return 0;
}
