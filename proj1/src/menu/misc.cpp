#include <iostream>
#include <sstream>
#include "misc.hpp"

void clear_screen(void) {
  #if defined(_WIN32) || defined(__WIN32__) || defined(_MSC_VER)
    std::system("cls");
  #else
    std::system("clear");
  #endif
}

void print(const char* fmt) {
  std::cout << fmt;
}

void fmt_string(std::stringstream& ss, const char* fmt) {
  ss << fmt;
}
