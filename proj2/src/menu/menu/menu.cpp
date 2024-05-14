#include "menu.hpp"
#include "../misc/misc.hpp"
#include "../error/errors.hpp"

namespace MenuN {


Menu::Menu() {}


Menu::Menu(const Menu& other) : submenus(other.submenus), history() {
  for(auto& submenu : this->submenus) {
    submenu.menu_p = this;

    for(auto& action : submenu.actions) {
      action->set_menu_p(this);
    }
  }
}


Menu::~Menu() {}


Result::Result<Menu&, MenuError> Menu::add_menu(Submenu::SubMenu submenu) {
  for(const auto& submenu_here : submenus) {
    if(submenu_here.id == submenu.id) {
      return Result::Err(MenuError::AlreadyExists);
    }
  }

  submenu.menu_p = this;
  for(auto a : submenu.actions) {
    a->set_menu_p(this);
  }

  submenus.push_back(submenu);
  return Result::Ok(this);
}


void Menu::run() {
  if(submenus.size() == 0) {
    Misc::eprintln("idiot"); return;
  }

  current = &submenus[0];
  while(running) {
    Misc::clear_screen();
    current->draw();
    auto input = Misc::user_input<char>();
    char c_action = matchr(input, {
      case Errors::InputError::Ok: return input.unwrap();
      case Errors::InputError::WrongDataType: Misc::fix_cin();
      default: return ' ';
    });

    MenuAction action = MenuAction(c_action);
    switch(action) {
      case MenuAction::UP: { 
        if(cursor_position > 0) 
          cursor_position--;
        break;
      }
      case MenuAction::DOWN: { 
        if(cursor_position < current->cursor_limit-1)
          cursor_position++;
        break;
      }
      case MenuAction::ACT: {
        current->perform_action(cursor_position);
        break;
      }
      case MenuAction::BACK: { 
        back();
        break;
      }
      case MenuAction::QUIT: { 
        running = false; 
        break;
      }
      default: break;
    }
  }

  for(auto& library : open_shared_libraries) {
    library.close_lib();
  }
}


}
