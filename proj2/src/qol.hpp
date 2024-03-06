#ifndef ML
    #define ML

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <limits>

#if defined(__clang__)
  #define COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
  #define COMPILER_GCC
#endif


// unsigned ints
typedef uint8_t        u8;
typedef uint16_t       u16;
typedef uint32_t       u32;
typedef uint64_t       u64;
typedef size_t         usize;

// signed ints
typedef int8_t         i8;
typedef int16_t        i16;
typedef int32_t        i32;
typedef int64_t        i64;

// floats
typedef float          f32;
typedef double         f64;

//strings
typedef std::string    str;
typedef const char*    const_str;

// colors
#ifdef __linux__
    static const_str WHITE = "\x1b[30m";
    static const_str RED = "\x1b[31m";
    static const_str GREEN = "\x1b[32m";
    static const_str YELLOW = "\x1b[33m";
    static const_str BLUE = "\x1b[34m";
    static const_str MAGENTA = "\x1b[35m";
    static const_str CYAN = "\x1b[36m";
    static const_str BLACK = "\x1b[37m";
    
    static const_str RESET = "\x1b[30;0m";
    static const_str BOLD = "\x1b[30;1m";
    static const_str ITALIC = "\x1b[30;3m";
    static const_str UNDERLINE = "\x1b[30;4m";
#endif

//macros
#define fn auto
#define Vec std::vector

#define loop while(true)

#define array_size(__ARR) sizeof(__ARR) / sizeof(__ARR[0])

#define to_str(TYPE) ((const char*)#TYPE)

#define for_range(__COUNT)\
    for(u32 i = 0; i < __COUNT; i++)

#define ml_inline static inline

// functions

ml_inline Vec<str> split_str(str string, char delimiter) {
  std::stringstream ss(string);
  str segment;
  Vec<str> vec;
  while(std::getline(ss, segment, delimiter)) {
    vec.push_back(segment);
  }
  return vec;
}

ml_inline void replace_first(str &string, str const &to_replace,  str const &new_text) {
  usize pos = string.find(to_replace);
  if(pos == std::string::npos) {return;}
  string.replace(pos, to_replace.length(), new_text);
}

ml_inline void print(const_str fmt) {
  std::cout << fmt;
}

template<typename T, typename... Args>
ml_inline void print(const_str fmt, T v , Args... args) {
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
ml_inline void println(const_str fmt, Args... args) {
  print(fmt, args...);
  std::cout << std::endl;
}


ml_inline void eprint(const_str fmt) {
  std::cerr << fmt;
}

template<typename T, typename... Args>
ml_inline void eprint(const_str fmt, T v, Args... args) {
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
ml_inline void eprintln(const_str fmt, Args... args) {
  eprint(fmt, args...);
  std::cerr << std::endl;
}


template <typename T>
fn user_input() -> T {
  T string;
  while(!(std::cin >> string)) {
    println("{}wrong data type!{}", RED, RESET);
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return string;
}


#endif
