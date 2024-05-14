#include "changemenu.hpp"
#include <memory>
#include <string>

namespace MenuN::Actions {

#if defined(__linux__)
extern "C" {
  ChangeMenu* allocator(std::string label, std::string target_menu_id) {
    return new ChangeMenu(label, target_menu_id);
  }

  void deleter(ChangeMenu* p) {
    delete p;
  }
}
#endif

#ifdef WIN32

extern "C" {

  __declspec (dllexport) ChangeMenu* allocator(std::string label, std::string target_menu_id) {
    return new ChangeMenu(label, target_menu_id);
  }

  __declspec (dllexport) void deleter(ChangeMenu* p) {
    delete p;
  }

}

#endif


ChangeMenu::ChangeMenu(std::string label, std::string target_menu_id) 
  : Action(label), destination(target_menu_id) {}


ChangeMenu::~ChangeMenu() {}


void ChangeMenu::action() {
  menu_p->change_menu(destination);
}

}
