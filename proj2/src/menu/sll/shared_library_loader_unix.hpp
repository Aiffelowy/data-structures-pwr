#ifndef SHARED_LIBRARY_LOADER_UNIX_H
#define SHARED_LIBRARY_LOADER_UNIX_H

#include "../error/result.hpp"

#include <dlfcn.h>
#include <iostream>
#include <memory>

enum class SLLError {
  Ok,
  CannotOpen,
};

namespace slloader {

template<typename T>
class SLLoader {
private:
  void* handle;
  std::string path;
  std::string allocator_symbol;
  std::string deleter_symbol;

public:
  SLLoader(std::string path_to_lib,
           std::string allocator_symbol = "allocator",
           std::string deleter_symbol = "deleter") :
  handle(nullptr), path(path_to_lib), allocator_symbol(allocator_symbol),
    deleter_symbol(deleter_symbol) {}

  ~SLLoader() = default;

  std::string get_path() const { return path; }

  Result::Result<Result::Nothing, SLLError> open_lib() {
    if(!(handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LAZY))) {
      std::cerr << dlerror() << std::endl;
      return Result::Err(SLLError::CannotOpen);
    }
    return Result::Ok();
  }

  void close_lib() {
    if(dlclose(handle) != 0) {
      std::cerr << dlerror() << std::endl;
    }
  }


  template<typename ...Args>
  std::shared_ptr<T> get_instance(Args... args) {
    using allocator = T*(*)(Args...);
    using deleter = void (*)(T*);

    auto allocator_fn = reinterpret_cast<allocator>(dlsym(handle, allocator_symbol.c_str()));
    auto delete_fn = reinterpret_cast<deleter>(dlsym(handle, deleter_symbol.c_str()));

    if(!allocator_fn || !delete_fn) {
      close_lib();
      std::cerr << dlerror() << std::endl;
    }

    return std::shared_ptr<T>(allocator_fn(args...), [delete_fn](T* p){delete_fn(p);});
  }
};

}

#endif
