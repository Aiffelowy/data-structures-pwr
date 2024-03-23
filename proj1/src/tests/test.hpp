#ifndef TESTS
#define TESTS

#include "../array_list/array_list.hpp"
#include "../double_linked_list/double_linked_list.hpp"
#include "../linked_list/linked_list.hpp"

#include <iostream>


//#############
//### ARRAY ###
//#############

template<typename T>
void print_array(ArrayList<T>& arr) {
  std::cout << "len: " << arr.len() << " size: " << arr.size() << std::endl;
  for(T item : arr) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
}

template<typename T>
void print_array_copy(ArrayList<T> arr) {
  std::cout << "len: " << arr.len() << " size: " << arr.size() << std::endl;
  for(T item : arr) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
}

#endif
