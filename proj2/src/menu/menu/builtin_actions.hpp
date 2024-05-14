#include "menu.hpp"

namespace MenuN::Actions {

class ChangeMenu : public Action {
private:
  std::string destination;

public:
  ChangeMenu(std::string label, std::string target_menu_id);
  
  ~ChangeMenu();
  
  void action();

};

class Back : public Action {
private:
  std::string destination;

public:
  Back();
  
  ~Back();
  
  void action();

};

class Quit : public Action {
private:
  std::string destination;

public:
  Quit();
  
  ~Quit();
  
  void action();

};

}
