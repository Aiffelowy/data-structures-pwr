#include "action_template.hpp"
#include "../src/menu/misc/testing.hpp"

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

CustomAction::CustomAction() : Action("Save test results to .csv file") {}


CustomAction::~CustomAction() {}


void CustomAction::action() {
  if(!menu_p->crossmenu_data.count("test_results"))
    return;

  Misc::clear_screen();
  Misc::print("Enter name of the file: ");
  std::string filename = Misc::user_input<std::string>().expect("Invalid data type");
  std::any_cast<TestThings::TestSuite<>>(menu_p->crossmenu_data["test_results"]).to_csv(filename);
}

}
