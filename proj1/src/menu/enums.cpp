#include "enums.hpp"
#include <iostream>
#include <string>

std::string to_string(const DS_TO_TEST& type) {
  std::string s;
  switch (type) {
    case LINKED_LIST: s = "Linked List"; break;
    case LINKED_LIST_T: s = "Linked List (with tail)"; break;
    case DOUBLY_LINKED_LIST: s = "Doubly Linked List"; break;
    case DYNAMIC_ARRAY: s = "Dynamic Array"; break;
  }
  return s;
}

std::string to_string(const RNG_CHOICE& choice) {
  std::string out_string;
  switch (choice) {
    case N5_000:
      out_string = "5_000";
      break;
    case N8_000:
      out_string = "8_000";
      break;
    case N10_000:
      out_string = "10_000";
      break;
    case N16_000:
      out_string = "16_000";
      break;
    case N20_000:
      out_string = "20_000";
      break;
    case N40_000:
      out_string = "40_000";
      break;
    case N60_000:
      out_string = "60_000";
      break;
    case N100_000:
      out_string = "100_000";
      break;
  }
  return out_string;
}

std::ostream& operator<<(std::ostream& ostream, const TIME_UNIT& unit) {
  std::string s = "sus";

  switch (unit) {
    case NANOSECONDS: s = "nano"; break;
    case MICROSECONDS: s = "micro"; break;
  }

  ostream << s;
  return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const DS_TO_TEST& type) {
  ostream << to_string(type);
  return ostream;
}


std::ostream& operator<<(std::ostream& ostream, const RNG_CHOICE& choice) {
  ostream << to_string(choice);
  return ostream;
}



