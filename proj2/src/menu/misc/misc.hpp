#ifndef MISC_H
#define MISC_H

#include <iostream>
#include <sstream>

#include "../error/result.hpp"
#include "../error/errors.hpp"


namespace Misc {

void print(const char * fmt);

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
void println(const char * fmt, Args... args) {
  print(fmt, args...);
  std::cout << std::endl;
}


void eprint(const char * fmt);

template<typename T, typename... Args>
void eprint(const char * fmt, T v, Args... args) {
  for(; *fmt != '\0'; fmt++) {
    if(*fmt == '{' && *(fmt + 1) == '}') {
      std::cerr << v;
      eprint(fmt+2, args...);
      return;
    }
    std::cerr << *fmt;
  }
}

template<typename... Args>
void eprintln(const char * fmt, Args... args) {
  eprint(fmt, args...);
  std::cerr << std::endl;
}


template<typename T>
Result::Result<T, Errors::InputError> user_input() {
  T input;
  if(!(std::cin >> input)) {
    return Result::Err(Errors::InputError::WrongDataType);
  }
  return Result::Ok(input);
}

void fix_cin();

void fmt_string(std::stringstream& ss, const char* fmt);

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


}

#endif
