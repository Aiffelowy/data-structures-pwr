#include "misc.hpp"
#include "rng.hpp"

#include <chrono>
#include <cstdint>
#include <ostream>
#include <string>
#include <fstream>
#include <functional>

#define M_test_time(TimeUnit, Body)\
[&]() -> uint64_t {\
  auto start = std::chrono::high_resolution_clock::now();\
  Body\
  auto stop = std::chrono::high_resolution_clock::now();\
  return std::chrono::duration_cast<TimeUnit>(stop - start).count();\
}()

namespace TestThings {

template<typename F, typename TimeUnit = std::chrono::nanoseconds>
int test_time(F&& fn_to_test) {
  auto start = std::chrono::high_resolution_clock::now();
  fn_to_test();
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<TimeUnit>(stop - start);
  return duration.count();
}

struct TestResult {
  bool passed;
  uint64_t time;

  TestResult(bool passed, uint64_t time) : passed(passed), time(time) {}

  void operator+=(const TestResult& other) {
    this->time += other.time;
    if(this->passed)
      this->passed = other.passed;
  }
};


struct Test {
  std::string label;
  std::function<TestResult(const RNG&)> test;

  Test(std::string label, std::function<TestResult(const RNG&)> fn) : label(label), test(fn) {}
};


template<typename TimeUnit = std::chrono::nanoseconds>
struct TestRunner {
private:
  std::vector<TestResult> test_results;
  int samples_per_test;
  int number_of_tests;
  int data_size;

public:
  
  TestRunner(int samples_per_test, int number_of_tests, int data_size) : samples_per_test(samples_per_test), number_of_tests(number_of_tests), data_size(data_size) {}

  void test(std::function<TestResult(const RNG&)> fn_test, std::string name) {
    RNG rng;
    for(int i = 0; i < number_of_tests; i++) {
      rng.generate_numbers(data_size);
      TestResult sum(0, true);

      for(int j = 0; j < samples_per_test; j++) {
        Misc::println("Testing \"{}\" on data size {}: run: {} sample: {}", name, data_size, i+1, j+1);
        sum += fn_test(rng);
      }
      sum.time = sum.time/samples_per_test;
      test_results.push_back(sum);
    }
  }

  friend std::ostream& operator<<(std::ostream& os, const TestRunner& test) {
    TestResult  avg(true, 0);
    for(const auto& result : test.test_results) {
      avg += result;
    }
    avg.time = avg.time/test.number_of_tests;
    os << avg.time;
    return os;
  }

};


template<typename TimeUnit = std::chrono::nanoseconds>
class TestSuite {
private:

  int samples_per_test = 0;
  int number_of_tests = 0;
  std::vector<Test> test_types;
  std::vector<int> data_sizes;
  std::vector<TestRunner<TimeUnit>> tests;

public:
  TestSuite() {}

  ~TestSuite() {}

  TestSuite& set_number_of_tests_per_data(int number) {
    samples_per_test = number;
    return *this;
  }

  TestSuite& set_number_of_tests(int number) {
    number_of_tests = number;
    return *this;
  }

  TestSuite& add_test(Test test) {
    test_types.push_back(test);
    return *this;
  }

  TestSuite& add_data_size(int size) {
    data_sizes.push_back(size);
    return *this;
  }

  Result::Result<Result::Nothing, Errors::IOError> to_csv(std::string path) {
    std::ofstream file(path);
    
    if(file.fail())
      return Result::Err(Errors::IOError::CoulntOpenFile);

    file << "data size/operation,";
    for(int i = 0; i < test_types.size(); i++) {
      file << test_types[i].label;
      if(i != test_types.size()-1)
        file << ',';
    }
    file << '\n';

    int difference = data_sizes.size() - test_types.size();
    for(int data_sizes_i = 0; data_sizes_i < data_sizes.size(); data_sizes_i++) {
      file << data_sizes[data_sizes_i] << ',';
      for(int tests_i = 0; tests_i < test_types.size(); tests_i++) {
        int index = (data_sizes.size()-difference)*data_sizes_i + tests_i;
        file << tests[index];
        if(tests_i != test_types.size()-1)
          file << ',';
      }
      file << '\n';
    }

    return Result::Ok();
  }


  void test_full() {
    for(const auto& data_size : data_sizes) {
      for(const auto& test_t : test_types) {
        TestRunner<> test(samples_per_test, number_of_tests, data_size);
        test.test(test_t.test, test_t.label);
        tests.push_back(test);
      }
    }
  }
};

}
