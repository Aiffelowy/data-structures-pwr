#include "run_tests.hpp"
#include "../src/menu/misc/testing.hpp"
#include "../src/data_structures/queue_list/Queue.h"
#include "../src/data_structures/queue_heap/priority_queue_heap.hpp"

#include <algorithm>
#include <any>

namespace MenuN::Actions {

#if defined(__linux__)
extern "C" {
  CustomAction* allocator(std::string d) {
    return new CustomAction(d);
  }

  void deleter(CustomAction* p) {
    delete p;
  }
}
#endif

#ifdef WIN32

extern "C" {

  __declspec (dllexport) CustomAction* allocator() {
    return new CustomAction();
  }

  __declspec (dllexport) void deleter(CustomAction* p) {
    delete p;
  }

}

#endif



using TestThings::TestSuite, TestThings::TestResult, TestThings::Test, TestThings::RNG;

void from_rng(PriorityQueue<int>& pq, const RNG& rng) {
  for(const auto& n : rng) {
    pq.insert(n, n);
  }
}

void from_rng(PriorityQueueHeap<int>& pq, const RNG& rng) {
  for(const auto& n : rng) {
    pq.insert(n, n);
  }
}



TestResult test_heap_push(const RNG& rng) {
  PriorityQueueHeap<int> pq;
  from_rng(pq, rng);
  long time = TestThings::test_time([&pq, &rng](){ pq.insert(rng[6], rng[9]); });
  return TestResult(true, time);
}

TestResult test_heap_pop(const RNG& rng) {
  PriorityQueueHeap<int> pq;
  from_rng(pq, rng);
  long time = TestThings::test_time([&pq](){ pq.extract_max(); });
  return TestResult(true, time);
}

TestResult test_heap_peek(const RNG& rng) {
  PriorityQueueHeap<int> pq;
  from_rng(pq, rng);
  long time = TestThings::test_time([&pq](){ pq.peek(); });
  return TestResult(true, time);
}

TestResult test_heap_change_priority(const RNG& rng) {
  PriorityQueueHeap<int> pq;
  from_rng(pq, rng);
  int index = rng[69] % rng.len();
  long time = TestThings::test_time([&pq, &rng, index](){ pq.modify_key(index, rng[69]); });
  return TestResult(true, time);
}

TestResult test_heap_len(const RNG& rng) {
  PriorityQueueHeap<int> pq;
  from_rng(pq, rng);
  long time = TestThings::test_time([&pq](){ pq.return_size(); });
  return TestResult(true, time);
}





TestResult test_list_push(const RNG& rng) {
  PriorityQueue<int> pq;
  from_rng(pq, rng);
  long time = TestThings::test_time([&pq, &rng](){ pq.insert(rng[6], rng[9]); });
  return TestResult(true, time);
}

TestResult test_list_pop(const RNG& rng) {
  PriorityQueue<int> pq;
  from_rng(pq, rng);
  long time = TestThings::test_time([&pq](){ pq.extract_max(); });
  return TestResult(true, time);
}

TestResult test_list_peek(const RNG& rng) {
  PriorityQueue<int> pq;
  from_rng(pq, rng);
  long time = TestThings::test_time([&pq](){ pq.peek(); });
  return TestResult(true, time);
}

TestResult test_list_change_priority(const RNG& rng) {
  PriorityQueue<int> pq;
  from_rng(pq, rng);
  int index = rng[69] % rng.len();
  long time = TestThings::test_time([&pq, &rng, index](){ pq.modify_key(index, rng[69]); });
  return TestResult(true, time);
}

TestResult test_list_len(const RNG& rng) {
  PriorityQueue<int> pq;
  from_rng(pq, rng);
  long time = TestThings::test_time([&pq](){ pq.return_size(); });
  return TestResult(true, time);
}



CustomAction::CustomAction(std::string s) : Action("Run Tests"), destination(s) {}


CustomAction::~CustomAction() {}


void CustomAction::action() {
  using namespace TestThings;

  if(!submenu_p->data.count("ds_type")) { return; }
  if(!submenu_p->data.count("data_sizes")) { return; }
  if(!submenu_p->data.count("test_size")) { return; }
  if(!submenu_p->data.count("sample_size")) { return; }

  TestSuite tests;
  tests
    .set_number_of_tests(std::any_cast<int>(submenu_p->data.at("test_size")))
    .set_number_of_tests_per_data(std::any_cast<int>(submenu_p->data.at("sample_size")));

  if(std::any_cast<bool>(submenu_p->data.at("ds_type"))) {
    tests
      .add_test(Test("insert", test_heap_push))
      .add_test(Test("extract-max", test_heap_pop))
      .add_test(Test("peek", test_heap_peek))
      .add_test(Test("modify-key", test_heap_change_priority))
      .add_test(Test("return-size", test_heap_len));
  } else {
    tests
      .add_test(Test("insert", test_list_push))
      .add_test(Test("extract-max", test_list_pop))
      .add_test(Test("peek", test_list_peek))
      .add_test(Test("modify-key", test_list_change_priority))
      .add_test(Test("return-size", test_list_len));
  }

  std::vector<int> d = std::any_cast<std::vector<int>>(submenu_p->data.at("data_sizes"));
  std::sort(d.begin(), d.end());
  for(const auto& s : d) {
    tests.add_data_size(s);
  }
  tests.test_full();
  menu_p->crossmenu_data["test_results"] = tests;
  menu_p->change_menu(destination);
}

}
