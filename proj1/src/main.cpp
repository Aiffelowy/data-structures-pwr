#include "array_list/array_list.hpp"
#include "linked_list/linked_list.hpp"
#include "double_linked_list/double_linked_list.hpp"
#include "tests/test.hpp"

#include <chrono>
#include <iostream>

int main() {
  ArrayList<int> arr(5000);
  
  std::cout << test_time<std::chrono::microseconds>([arr]() mutable { arr.insert(1, 0); });

  return 0;
}
