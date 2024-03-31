#include "test.hpp"
#include "../menu/misc.hpp"
#include <filesystem>
#include <fstream>


void auto_test(RNG& rng, std::string path, const TIME_UNIT& time_unit, int number_of_runs) {
  ArrayList<DS_TO_TEST> dss_to_test = { LINKED_LIST, LINKED_LIST_T, DOUBLY_LINKED_LIST, DYNAMIC_ARRAY };
  ArrayList<RNG_CHOICE> rngs_to_test = { N5_000, N8_000, N10_000, N16_000, N20_000, N40_000, N60_000, N100_000 };
  //ANOTHER FOR
  for(int i = 1; i <= number_of_runs; i++) {
    std::filesystem::create_directories(format_string("{}/rng_states/run_{}", path, i));
    for(const RNG_CHOICE& rng_choice : rngs_to_test) {
      rng.generate_numbers(rng_choice);
      rng.save_to_file(format_string("{}/rng_states/run_{}/{}", path, i, rng_choice));
      for(const DS_TO_TEST& ds_to_test : dss_to_test) {
        println("Testing: {} {} run {}", ds_to_test, rng_choice, i);
        TestResults current_test(time_unit, ds_to_test);
        switch(ds_to_test) {
          case LINKED_LIST: { current_test.run_tests(rng.into_linkedlist()); break; }
          case LINKED_LIST_T: { current_test.run_tests(rng.into_linkedlistT()); break; }
          case DOUBLY_LINKED_LIST: { current_test.run_tests(rng.into_doublelinkedlist()); break; }
          case DYNAMIC_ARRAY: { current_test.run_tests(rng.into_array()); break; }
        }
        std::filesystem::create_directories(format_string("{}/{}/run_{}", path, ds_to_test, i));
        current_test.save_to_file(format_string("{}/{}/run_{}/{}", path, ds_to_test, i, rng_choice));
      }
    }
  }
}

void TestResults::print_tests() const {
  println("add_first: {} {}", add_first, time_unit);
  println("add_last: {} {}", add_last, time_unit);
  println("add_random: {} {}", add_random, time_unit);
  println("remove_first: {} {}", remove_first, time_unit);
  println("remove_last: {} {}", remove_last, time_unit);
  println("remove_random: {} {}", remove_random, time_unit);
  println("find: {} {}", find, time_unit);
}

bool TestResults::save_to_file(std::string path) const {
  std::ofstream file;
  file.open(path);
  if(!file.is_open()) { return false; }
  file << ds_type << "    " << "size: " << len << "\n";
  file << "add_first: " << add_first << " " << time_unit << "\n";
  file << "add_last: " << add_last << " " << time_unit << "\n";
  file << "add_random: " << add_random << " " << time_unit << "\n";
  file << "remove_first: " << remove_first << " " << time_unit << "\n";
  file << "remove_last: " << remove_last << " " << time_unit << "\n";
  file << "remove_random: " << remove_random << " " << time_unit << "\n";
  file << "find: " << find << " " << time_unit << "\n";
  return true;
}

