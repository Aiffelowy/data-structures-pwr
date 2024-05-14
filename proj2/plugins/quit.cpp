#include "quit.hpp"
#include <memory>

namespace MenuN::Actions {

#if defined(__linux__)
extern "C" {
  Quit* allocator() {
    return new Quit();
  }

  void deleter(Quit* p) {
    delete p;
  }
}
#endif

#ifdef WIN32

extern "C" {

  __declspec (dllexport) Quit* allocator() {
    return new Quit;
  }

  __declspec (dllexport) void deleter(Quit* p) {
    delete p;
  }

}

#endif

Quit::Quit() : Action("Quit") {}


Quit::~Quit() {}


void Quit::action() {
  menu_p->stop();
}

}
