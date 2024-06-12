#include "hash_tables/hash_map.hpp"
#include "testing.hpp"
#include <chrono>

using TimeUnit = std::chrono::nanoseconds;

void from_rng(Hash1::HashMap<int, int>& map, const TestThings::RNG& rng) {
  for(const auto& n : rng) {
    map[n] = n;
  }
}

TestThings::TestResult test_insert(const TestThings::RNG& rng) {
  Hash1::HashMap<int, int> map(rng.len()*2);
  from_rng(map, rng);
  int index = rng[69] % rng.len();
  long time = M_test_time(TimeUnit, { map.insert(index, index); });
  return TestThings::TestResult(true, time);
}

TestThings::TestResult test_remove(const TestThings::RNG& rng) {
  Hash1::HashMap<int, int> map(rng.len()+100);
  from_rng(map, rng);
  int index = rng[69] % rng.len();
  long time = M_test_time(TimeUnit, { map.remove(index); });
  return TestThings::TestResult(true, time);
}



int main(int argc, char *argv[]) {
  TestThings::TestSuite<std::chrono::nanoseconds> tests;

  tests
    .add_data_size(5000)
    .add_data_size(8000)
    .add_data_size(16000)
    .add_data_size(20000)
    .add_data_size(40000)
    .add_data_size(60000)
    .add_data_size(80000)
    .add_data_size(100000)
    .add_test(TestThings::Test("insert", test_insert))
    .add_test(TestThings::Test("remove", test_remove))
    .set_number_of_tests(3)
    .set_number_of_tests_per_data(3)
    .test_full();

  tests.to_csv("result.csv");
  
}
