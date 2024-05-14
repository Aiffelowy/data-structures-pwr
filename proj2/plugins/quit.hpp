#ifndef ACTION_QUIT_H
#define ACTION_QUIT_H

#include "../src/menu/menu/menu.hpp"
#include <memory>

namespace MenuN::Actions {

class Quit : public Action {
public:
  Quit();
  
  ~Quit();
  
  void action() override;

};

}

#endif
