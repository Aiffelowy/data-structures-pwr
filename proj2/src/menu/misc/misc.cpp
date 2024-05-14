#include "misc.hpp"

#include <limits>
#include <iostream>
#include <sstream>

namespace Misc {

void print(const char * fmt) {
  std::cout << fmt;
}


void eprint(const char * fmt) {
  std::cerr << fmt;
}


void fix_cin() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


void fmt_string(std::stringstream& ss, const char* fmt) {
  ss << fmt;
}


void clear_screen() {
 #if defined(_WIN32) || defined(__WIN32__) || defined(_MSC_VER)
    [[maybe_unused]] int shut_up = std::system("cls");
  #else
    [[maybe_unused]] int shut_up = std::system("clear");
  #endif
}


}
