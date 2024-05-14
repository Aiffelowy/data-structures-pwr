#include "menu.hpp"
#include "../misc/misc.hpp"


namespace MenuN::Submenu {


SubMenu::SubMenu(std::string id) : id(id), menu_p(nullptr) {}


SubMenu::SubMenu(const SubMenu& other) : id(other.id), title(other.title), subtitle(other.subtitle),
                  menu_p(other.menu_p), cursor_limit(other.cursor_limit) {
  for(auto action : other.actions) {
    this->actions.push_back(action);
    this->actions.back()->set_submenu_p(this);
    this->actions.back()->set_menu_p(menu_p);
  }
}


SubMenu::~SubMenu() {}


void SubMenu::draw() {
  Misc::println("{}\n{}\n\n", title, subtitle);
  for(const auto& [t, r] : additional_text) {
    Misc::println("{}: {}", t, r);
  }
  int i = 0;
  for(auto action : actions) {
    Misc::println("  {} {}", (i == menu_p->cursor_position) ? ">" : " ", action->get_label());
    i++;
  }
}


void SubMenu::perform_action(int cursor_position) {
  actions[cursor_position]->action();
}


SubMenu& SubMenu::set_title(std::string s) {
  title = s;
  return *this;
}


SubMenu& SubMenu::set_subtitle(std::string s) {
  subtitle = s;
  return *this;
}


SubMenu& SubMenu::add_action(std::shared_ptr<Actions::Action> action) {
  action->set_submenu_p(this);
  action->set_menu_p(menu_p);
  actions.push_back(action);
  cursor_limit++;
  return *this;
}


}
