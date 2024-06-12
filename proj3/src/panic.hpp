#ifndef PANIC_H
#define PANIC_H

#include <cstdlib>
#include <iostream>
#include <source_location>
#include <thread>
#include <type_traits>
#include <string>
#include <sstream>

#include "colors.hpp"

[[maybe_unused]] static void eprint(const char * fmt) {
  std::cerr << fmt;
}

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


[[maybe_unused]] static void fmt_string(std::stringstream& ss, const char* fmt) {
  ss << fmt;
}

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

namespace Panic {

template<typename ...Args>
struct panic_format {
  const char * format_string;
  std::source_location loc;

  template<typename T>
  panic_format(const T& fmt, std::source_location location = std::source_location::current()) : format_string(fmt), loc(location) {}

  panic_format(const std::string& fmt, std::source_location location = std::source_location::current()) : format_string(fmt.c_str()), loc(location) {}
};

template<typename ...Args>
void panic(panic_format<std::type_identity_t<Args>...> fmt, Args... args) {
  eprintln("{}thread '{}' panicked at {}({}:{}):{}\n{}", Colors::RED_BOLD, std::this_thread::get_id(), fmt.loc.file_name(), fmt.loc.line(), fmt.loc.column(), 
                 Colors::RESET, format_string(fmt.format_string, args...));
  std::exit(1);
}

template<typename ...Args>
void panic_unformated(panic_format<std::type_identity_t<Args>...> fmt, Args... args) {
  eprintln(fmt.format_string, args...);
  std::abort();
}

}



#endif
