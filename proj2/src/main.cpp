#include "menu/misc/testing.hpp"
#include "menu/misc/misc.hpp"
#include "data_structures/queue_heap/priority_queue_heap.hpp"
#include "data_structures/queue_list/Queue.h"


using TestThings::TestSuite, TestThings::TestResult, TestThings::Test, TestThings::RNG;

template<typename T>
void from_rng(T& pq, const RNG& rng) {
  for(const auto& n : rng) {
    pq.insert(n, n);
  }
}

template<typename T>
TestResult test_heap_push(const RNG& rng) {
  T pq;
  from_rng(pq, rng);
  long time = TestThings::test_time([&pq, &rng](){ pq.insert(rng[6], rng[9]); });
  return TestResult(true, time);
}

template<typename T>
TestResult test_heap_pop(const RNG& rng) {
  T pq;
  from_rng(pq, rng);
  long time = TestThings::test_time([&pq](){ pq.extract_max(); });
  return TestResult(true, time);
}

template<typename T>
TestResult test_heap_peek(const RNG& rng) {
  T pq;
  from_rng(pq, rng);
  long time = TestThings::test_time([&pq](){ pq.peek(); });
  return TestResult(true, time);
}

template<typename T>
TestResult test_heap_change_priority(const RNG& rng) {
  T pq;
  from_rng(pq, rng);
  int index = rng[69] % rng.len();
  long time = TestThings::test_time([&pq, &rng, index](){ pq.modify_key(index, rng[69]); });
  return TestResult(true, time);
}

template<typename T>
TestResult test_heap_len(const RNG& rng) {
  T pq;
  from_rng(pq, rng);
  long time = TestThings::test_time([&pq](){ pq.return_size(); });
  return TestResult(true, time);
}


using DS = PriorityQueue<int>;

int main(void) {
  TestSuite test;

  test
    .add_test(Test("insert", test_heap_push<DS>))
    .add_test(Test("extract_max", test_heap_pop<DS>))
    .add_test(Test("peek", test_heap_peek<DS>))
    .add_test(Test("modify_key", test_heap_change_priority<DS>))
    .add_test(Test("return_size", test_heap_len<DS>))
    .add_data_size(5000)
    .add_data_size(8000)
    .add_data_size(10000)
    .add_data_size(16000)
    .add_data_size(20000)
    .add_data_size(40000)
    .add_data_size(60000)
    .add_data_size(100000)
    .set_number_of_tests(10)
    .set_number_of_tests_per_data(10);

  test.test_full();
  test.to_csv("list.csv");
}
