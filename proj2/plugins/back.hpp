#ifndef ACTION_BACK_H
#define ACTION_BACK_H

#include "../src/menu/menu/menu.hpp"
#include <memory>

namespace MenuN::Actions {

class Back : public Action {
public:
  Back();
  
  ~Back();
  
  void action();

};

}

#endif
