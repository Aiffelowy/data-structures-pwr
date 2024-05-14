#include "menu/menu/menu.hpp"
#include "menu/menu/parser.hpp"

int main(void) {
  MenuN::Parser::Parser parser("menu.cfg");
  MenuN::Menu menu = parser.to_menu().expect("there was a problem while parsing the config");

  menu.run();
}
