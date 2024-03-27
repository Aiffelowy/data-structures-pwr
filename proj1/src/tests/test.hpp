#ifndef TESTS
#define TESTS

#include <chrono>


template<typename TimeUnit, typename F>
double test_time(F&& test_function) {
  auto start = std::chrono::high_resolution_clock::now();
  test_function();
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<TimeUnit>(stop-start);
  return duration.count();
}



#endif
