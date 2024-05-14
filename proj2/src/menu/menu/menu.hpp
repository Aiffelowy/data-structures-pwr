#ifndef MENU_H
#define MENU_H

#include <any>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../error/result.hpp"
#include "../sll/shared_library_loader_unix.hpp"


namespace MenuN {

struct Menu;

enum MenuError {
  Ok,
  AlreadyExists,
  NotFound,
};

namespace Submenu {
struct SubMenu;
}

namespace Actions {

class Action {
protected:
  std::string label = "";
  Submenu::SubMenu* submenu_p = nullptr;
  Menu* menu_p = nullptr;
  
  Action(std::string label) : label(label) {}
  

public:
  virtual ~Action() {}
  
  virtual void action() = 0;

  void set_submenu_p(Submenu::SubMenu* p) { submenu_p = p; }

  void set_menu_p(Menu* p) { menu_p = p; }


  void set_label(std::string s) { label = s; }

  std::string get_label() const { return label; }

};

}


namespace Submenu {


struct SubMenu {
private:
  std::vector<std::shared_ptr<Actions::Action>> actions;
  std::string id;
  std::string title;
  std::string subtitle;
  Menu* menu_p = nullptr;
  int cursor_limit = 0;

  void draw();

  void perform_action(int cursor_position);

public:

  std::map<std::string, std::any> data;
  
  std::map<std::string, std::string> additional_text;

  SubMenu(std::string id);

  SubMenu(const SubMenu& other);

  ~SubMenu();

  SubMenu& set_title(std::string s);

  SubMenu& set_subtitle(std::string s);

   SubMenu& add_action( std::shared_ptr<Actions::Action> action );
  
  friend Menu;
};

}

namespace Parser {
struct Parser;
}

enum class MenuAction {
  UP = 'w',
  DOWN = 's',
  BACK = 'a',
  ACT = 'd',
  QUIT = 'q',
  NOTHING = ' '
};


struct Menu {
private:
  int cursor_position = 0;
  std::vector<Submenu::SubMenu> submenus;
  std::vector<Submenu::SubMenu*> history;
  std::vector<slloader::SLLoader<Actions::Action>> open_shared_libraries;
  Submenu::SubMenu* current;
  bool running = true;

public:
  std::map<std::string, std::any> crossmenu_data;

  Menu();

  Menu(const Menu& other);

  ~Menu();

  Result::Result<Menu&, MenuError> add_menu(Submenu::SubMenu submenu);

  void run();

  Result::Result<Result::Nothing, MenuError> change_menu(std::string menu_id) {
    for(auto& submenu : submenus) {
      if(submenu.id != menu_id)
        continue;

      cursor_position = 0;
      history.push_back(current);
      current = &submenu;
      return Result::Ok();
  }

    return Result::Err(MenuError::NotFound);
  }


  void back() {
    if(history.size() == 0)
      return;

    cursor_position = 0;
    this->current = history.back();
    history.pop_back();
  }

  void stop() {
    running = false;
  }

  friend struct Submenu::SubMenu;
  friend struct Parser::Parser;
};


}

#endif
