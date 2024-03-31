#ifndef MISC_H
#define MISC_H

#include <iostream>
#include <sstream>
#include <string>

template<typename T>
T user_input(T default_value = T()) {
  while(!(std::cin >> default_value)) {
    std::cin.clear();
    std::cin.ignore(100000, '\n');
  }
  return default_value;
}

void print(const char*);

template<typename T, typename... Args>
void print(const char * fmt, T v , Args... args) {
  for (; *fmt != '\0'; fmt++) {
    if(*fmt == '{' && *(fmt+1) == '}') {
      std::cout << v;
      print(fmt + 2, args...);
      return;
    }
    std::cout << *fmt;
  }
}

template<typename... Args>
void println(const char* fmt, Args... args) {
  print(fmt, args...);
  std::cout << std::endl;
}

void fmt_string(std::stringstream&, const char*);

template<typename T, typename... Args>
void fmt_string(std::stringstream& ss, const char* fmt, T v, Args... args) {
  for(; *fmt != '\0'; fmt++) {
    if(*fmt == '{' && *(fmt+1) == '}') {
      ss << v;
      fmt_string(ss, fmt+2, args...);
      return;
    }
    ss << *fmt;
  }
}

template<typename... Args>
std::string format_string(const char* fmt, Args... args) {
  std::stringstream ss;
  fmt_string(ss, fmt, args...);
  return ss.str();
}



void clear_screen(void);

#endif
