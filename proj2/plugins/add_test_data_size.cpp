#include "action_template.hpp"
#include "../src/menu/misc/misc.hpp"
#include <any>
#include <vector>

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

CustomAction::CustomAction() : Action("Add Data Size") {}


CustomAction::~CustomAction() {}


void CustomAction::action() {
  Misc::clear_screen();
  Misc::print("Type data size to add: ");
  int size = Misc::user_input<int>().expect("Invalid data type");

  if(!submenu_p->data.count("data_sizes")) {
    submenu_p->data["data_sizes"] = std::vector<int>();
  }

  std::any_cast<std::vector<int>&>(submenu_p->data.at("data_sizes")).push_back(size);
  submenu_p->additional_text["data_sizes"].append(Misc::format_string(" {}", size));
}

}
