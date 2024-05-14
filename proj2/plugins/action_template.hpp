#ifndef ACTION_TEMPLATE_H
#define ACTION_TEMPLATE_H


#include "../src/menu/menu.hpp"
#include <memory>

namespace MenuN::Actions {

class CustomAction : public Action {
public:
  CustomAction();

  ~CustomAction();

  void action();

};


}

#endif
