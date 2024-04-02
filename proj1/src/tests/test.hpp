#ifndef TESTS_H
#define TESTS_H

#include <chrono>
#include <string>
#include "../menu/enums.hpp"
#include "../menu/rng.hpp"

template<typename TimeUnit, typename F>
int test_time(F&& test_function) {
  auto start = std::chrono::high_resolution_clock::now();
  test_function();
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<TimeUnit>(stop-start);
  return duration.count();
}

void auto_test(RNG&, std::string, const TIME_UNIT&, int);

struct TestResults {
  int len;
  TIME_UNIT time_unit;
  DS_TO_TEST ds_type;

  int add_first;
  int add_last;
  int add_random;

  int remove_first;
  int remove_last;
  int remove_random;

  int find;
  
  TestResults() {}
  TestResults(TIME_UNIT time, DS_TO_TEST type) : time_unit(time), ds_type(type) {}
  template<typename DS>
  void run_tests(DS structure_to_test);
  void print_tests() const;
  bool save_to_file(std::string) const;
};

template<typename DS>
void TestResults::run_tests(DS structure_to_test) {
  len = structure_to_test.len();
  int random_place = rand() % structure_to_test.len();
  
  auto add_first_test = [ds = structure_to_test]() mutable { ds.insert(69, 0); };
  auto add_last_test = [ds = structure_to_test]() mutable { ds.push(69); };
  auto add_random_test = [ds = structure_to_test, random_place]() mutable { ds.insert(69, random_place); };
  auto remove_first_test = [ds = structure_to_test]() mutable { ds.remove(0); };
  auto remove_last_test = [ds = structure_to_test]() mutable { ds.pop(); };
  auto remove_random_test = [ds = structure_to_test, random_place]() mutable { ds.remove(random_place); };
  auto find_test = [ds = structure_to_test]() { ds.find([](const int& x) { return x == 420; }); };
  switch(time_unit) {
    case NANOSECONDS: {
      add_first = test_time<std::chrono::nanoseconds>(add_first_test);
      add_last = test_time<std::chrono::nanoseconds>(add_last_test);
      add_random = test_time<std::chrono::nanoseconds>(add_random_test);

      remove_first = test_time<std::chrono::nanoseconds>(remove_first_test);
      remove_last = test_time<std::chrono::nanoseconds>(remove_last_test);
      remove_random = test_time<std::chrono::nanoseconds>(remove_random_test);

      find = test_time<std::chrono::nanoseconds>(find_test);
      break;
    }

    case MICROSECONDS: {

      add_first = test_time<std::chrono::microseconds>(add_random_test);
      add_last = test_time<std::chrono::microseconds>(add_last_test);
      add_random = test_time<std::chrono::microseconds>(add_random_test);

      remove_first = test_time<std::chrono::microseconds>(remove_first_test);
      remove_last = test_time<std::chrono::microseconds>(remove_last_test);
      remove_random = test_time<std::chrono::microseconds>(remove_random_test);
      
      find = test_time<std::chrono::microseconds>(find_test);
      break;
    }
    default:
      add_first = 42069;
      break;

  }

}




#endif
