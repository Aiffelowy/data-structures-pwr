#include "builtin_actions.hpp"

namespace MenuN::Actions {


ChangeMenu::ChangeMenu(std::string label, std::string destination) : Action(label), destination(destination) {}
ChangeMenu::~ChangeMenu() {}
void ChangeMenu::action() {
  menu_p->change_menu(destination);
}


Back::Back() : Action("Back") {}
Back::~Back() {}
void Back::action() {
  menu_p->back();
}


Quit::Quit() : Action("Quit") {}
Quit::~Quit() {}
void Quit::action() {
  menu_p->stop();
}

}
