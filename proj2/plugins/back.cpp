
#include "back.hpp"
#include <memory>

namespace MenuN::Actions {

#if defined(__linux__)
extern "C" {
  Back* allocator() {
    return new Back();
  }

  void deleter(Back* p) {
    delete p;
  }
}
#endif

#ifdef WIN32

extern "C" {

  __declspec (dllexport) Back* allocator() {
    return new Back();
  }

  __declspec (dllexport) void deleter(Back* p) {
    delete p;
  }

}

#endif

Back::Back() : Action("Back") {}


Back::~Back() {}


void Back::action() {
  menu_p->back();
}

}
