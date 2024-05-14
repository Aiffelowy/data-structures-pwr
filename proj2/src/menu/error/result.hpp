#ifndef RESULT_H
#define RESULT_H

#include <source_location>
#include <string>
#include <iostream>
#include "panic.hpp"


namespace Result {

struct Nothing {};

template<typename E>
struct Err {
private:
  E err;
  constexpr E get_error() const {
    return err;
  }

public:
  constexpr Err(E error) : err(error) {}
  
  template<typename Frien, typename FriendlyError>
  friend struct Result;
};


template<typename T = Nothing>
struct Ok {
private:
  T item;

  constexpr T get_item() const {
    return item;
  }

public:
  constexpr Ok(T value) : item(value) {}
  
  constexpr Ok() : item(Nothing()) {}
  
  template<typename Frien, typename FriendlyError>
  friend struct Result;
};


template<typename T, typename E>
struct Result {
private:
  bool is_error;

  union R {
    Ok<T> ok;
    Err<E> err;

    R(Ok<T> ok) : ok(ok) {}
    R(Err<E> err): err(err) {}

    ~R() {}
  } item;

public:
  constexpr Result(Ok<T> ok): is_error(false), item(ok) {}
  
  constexpr Result(Err<E> err): is_error(true), item(err) {}
  
  constexpr Result(Ok<typename std::remove_reference<T>::type*> ok): is_error(false), item(*ok.item) {}

  constexpr bool is_ok() const {
    return !is_error;
  }

  constexpr bool is_err() const {
    return is_error;
  }

  constexpr T unwrap(std::source_location loc = std::source_location::current()) {
    if(is_error) {
      Panic::panic(Panic::panic_format("Called unwrap() on an Err value", loc)); 
    } 
    return item.ok.get_item();
  }

  constexpr E error(std::source_location loc = std::source_location::current()) {
    if(!is_error) {
      Panic::panic(Panic::panic_format("Called error() on not Err value", loc)); 
    }
    return item.err.get_error();
  }

  constexpr T expect(std::string msg, std::source_location loc = std::source_location::current()) {
    if(is_error) {
      Panic::panic(Panic::panic_format(msg, loc));
    }
    return item.ok.get_item();
  }

  constexpr operator E() {
    if(!is_error) 
      return E::Ok;

    return item.err.get_error(); 
  }
  
  constexpr friend  std::ostream& operator<<(std::ostream& os, const Result& result) {
    if(result.is_error) {
      os << format_string("Err({})", result.item.err.get_error());
    } else {
      os << format_string("Ok({})", result.item.ok.get_item());
    }
    return os;
}

};

#define matchr(Res, Body)\
[&Res]() constexpr {\
  switch(Res) { Body }\
}()

}




#endif
