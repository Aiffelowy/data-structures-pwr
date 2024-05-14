#include "action_template.hpp"
#include "../src/menu/misc/misc.hpp"

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

CustomAction::CustomAction() : Action("Set Sample Size") {}


CustomAction::~CustomAction() {}


void CustomAction::action() {
  Misc::clear_screen();
  Misc::print("Type the new sample size: ");
  int size = Misc::user_input<int>().expect("Invalid data type");

  submenu_p->data["sample_size"] = size;
  submenu_p->additional_text["sample_size"] = Misc::format_string("{}", size);

}

}
