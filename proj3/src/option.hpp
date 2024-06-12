#ifndef OPTION_H
#define OPTION_H

#include "panic.hpp"

namespace Option {

template<typename T>
struct Option;


struct None {
public:
  constexpr None() {}
};


template<typename T>
struct Some {
private:
  T item;
  constexpr T get_item() const {
    return item;
  }

public:
  constexpr Some(T value) : item(value) {}
  
  template<typename Frien>
  friend struct Option;
};


template<typename T>
struct Option {
private:
  bool holds_item;

  union S {
    None none;
    Some<T> some;

    S(None n): none(n) {}
    S(Some<T> s): some(s) {}

    ~S() {}
  } item;

public:
  constexpr Option(None value) : holds_item(false), item(value) {}

  constexpr Option(Some<T> value) : holds_item(true), item(value) {}

  constexpr Option(Some<typename std::remove_reference<T>::type*> value) : holds_item(true), item(*value.item) {}

  constexpr bool is_none() const {
    return !holds_item;
  }

  enum class O { Some, None };

  constexpr T unwrap(std::source_location loc = std::source_location::current()) {
    if(!holds_item) { 
      Panic::panic(Panic::panic_format("Called unwrap on a None value", loc)); 
    }
    return item.some.get_item();
  }

  constexpr T unwrap_or_default() {
    if(!holds_item) {
      return T();
    }
    return item.some.get_item();
  }

  constexpr T expect(std::string msg, std::source_location loc = std::source_location::current()) {
    if(!holds_item) {
      Panic::panic(Panic::panic_format(msg, loc));
    }
    return item.some.get_item();
  }

  constexpr operator O() {
    if(holds_item)
      return O::Some;
    return O::None;
  }

  constexpr friend std::ostream& operator<<(std::ostream& os, const Option& option ) {
    if(option.holds_item) {
      os << format_string("Some({})", option.item.some.get_item());
    } else {
      os << "None";
    }
    return os;
  }

};



#define matcho(Opt, Body)\
[&]<typename T>(Option::Option<T>& o) constexpr {\
  typedef typename Option::Option<T>::O O;\
  using O::Some, O::None;\
  switch(o) { Body }\
}(Opt)



}

#endif
