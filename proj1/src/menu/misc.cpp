#include <iostream>
#include <sstream>
#include "misc.hpp"

void clear_screen(void) {
  std::system("clear");
}

void print(const char* fmt) {
  std::cout << fmt;
}

void fmt_string(std::stringstream& ss, const char* fmt) {
  ss << fmt;
}
