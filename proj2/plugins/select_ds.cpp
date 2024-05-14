#include "action_template.hpp"
#include <any>

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

CustomAction::CustomAction() : Action("Toggle type of data structure") {}


CustomAction::~CustomAction() {}


void CustomAction::action() {
  if(!submenu_p->data.count("ds_type")) {
    submenu_p->data["ds_type"] = true;
  }

  bool type = std::any_cast<bool>(submenu_p->data["ds_type"]);
  submenu_p->data["ds_type"] = !type;
  if(!type) {
    submenu_p->additional_text["ds_type"] = "Priority Queue List";
  } else {
    submenu_p->additional_text["ds_type"] = "Priority Queue Heap";
  }
}

}
