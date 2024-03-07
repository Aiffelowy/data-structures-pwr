#include "array_list/array_list.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  ArrayList<int> array = {1 ,2 ,3 ,4, 5, 6};
  std::cout << array.test_s() << " " << array.test_l() << std::endl;
  for(int a : array) {
    std::cout << a << " ";
  }
  std::cout << std::endl;
  return 0;
}
