#include "../src/menu/menu/menu.hpp"
#include <memory>

namespace MenuN::Actions {

class ChangeMenu : public Action {
private:
  std::string destination;

public:
  ChangeMenu(std::string label, std::string target_menu_id);
  
  ~ChangeMenu();
  
  void action();

};

}
