#include "action_template.hpp"
#include <memory>

namespace MenuN::Actions {

#if defined(__linux__)
extern "C" {
  CustomAction* allocator() {
    return new CustomAction();
  }

  void deleter(CustomAction* p) {
    delete p;
  }
}
#endif

#ifdef WIN32

extern "C" {

  __declspec (dllexport) CustomAction* allocator() {
    return new CustomAction();
  }

  __declspec (dllexport) void deleter(CustomAction* p) {
    delete p;
  }

}

#endif

CustomAction::CustomAction() : Action("CustomAction") {}


CustomAction::~CustomAction() {}


void CustomAction::action() {}

}

