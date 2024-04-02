#include "menu.hpp"
#include "enums.hpp"
#include "misc.hpp"
#include "rng.hpp"
#include <string>

void SubMenu::print_additional_info() {}

void SubMenu::print(int cursor_position, RNG& rng, DS_TO_TEST type, TIME_UNIT time_unit) {
  println("{}\n{}\n", title, subtitle);
  println("Data Structure: {}   RNG: {} {}   Time Unit: {}\n", type, rng.get_length(), rng.is_from_file() ? rng.loaded_from() : "", time_unit);
  print_additional_info();
  int i = 0;
  for(std::string choice : choices) {
    println("  {} {}", (i == cursor_position) ? ">" : " ", choice);
    i++;
  }
}

ACTION SubMenu::get_action(int cursor_position) {
  return actions[cursor_position];
}

constexpr MENUTYPE SubMenu::type() {
  return menu_type;
}

int SubMenu::cursor_limit() const {
  return choices.len()-1;
}

Menu::Menu() {}
Menu::~Menu() { delete current_submenu; }

void Menu::change_menu(MENUTYPE type, bool amongus) {
  if(amongus) { menu_history.push(current_submenu->type()); }
  cursor_position = 0;
  delete current_submenu;
  switch(type) {
    case TITLE: current_submenu = new TitleSubMenu(); break;
    case RNG_SELECT_NUMBER: current_submenu = new RNGSelectSubMenu(); break;
    case RNG_SCREEN: current_submenu = new RNGScreenSubMenu(); break;
    case DS_SELECT: current_submenu = new DSSelectSubMenu(); break;
    case DS_TEST: current_submenu = new DSResultSubMenu(); break;
    case TIME_UNIT_SELECT: current_submenu = new TimeUnitSubMenu(); break;
    case AUTO_TEST: current_submenu = new AutoTestSubMenu(); break;
  }
}

void Menu::action() {
  ACTION action = current_submenu->get_action(cursor_position);
  switch (action) {
    case CHANGE_MENU_TITLE: {
      change_menu(TITLE, true);
      break;
    }
    case CHANGE_MENU_RNG_SCREEN: {
      change_menu(RNG_SCREEN, true);
      break;
    }
    case CHANGE_MENU_RNG_SELECT: {
      change_menu(RNG_SELECT_NUMBER, true);
      break;
    }
    case CHANGE_MENU_DS_SELECT: {
      change_menu(DS_SELECT, true);
      break;
    }
    case CHANGE_MENU_AUTO_TEST: {
      change_menu(AUTO_TEST, true);
      break;
    }
    case CHANGE_MENU_DS_TEST: {
      change_menu(DS_TEST, true);
      TestResults result(time_unit, ds_to_test);
      switch(ds_to_test) {
        case LINKED_LIST: { result.run_tests(rng.into_linkedlist()); break; }
        case LINKED_LIST_T: { result.run_tests(rng.into_linkedlistT()); break; }
        case DOUBLY_LINKED_LIST: { result.run_tests(rng.into_doublelinkedlist()); break; }
        case DYNAMIC_ARRAY: { result.run_tests(rng.into_array()); break; }
      }
      //should never fail
      DSResultSubMenu* c_menu = dynamic_cast<DSResultSubMenu*>(current_submenu);
      c_menu->load_test_result(result);
      break;
    }
    case CHANGE_MENU_TIME_UNIT: {
      change_menu(TIME_UNIT_SELECT, true);
      break;
    }
    case LOAD_RNG: {
      clear_screen();
      print("Enter a filename of RNG file: ");
      std::string path = user_input<std::string>();
      if(!rng.from_file(path)) { println("FAILED TO LOAD {}", path); };
      current_submenu->print(cursor_position, rng, ds_to_test, time_unit);
      break;
    }
    case SAVE_RNG: {
      clear_screen();
      print("Enter a filename to save to: ");
      std::string path = user_input<std::string>();
      if(!rng.save_to_file(path)) { println("FAILED TO SAVE {}", path); }
      current_submenu->print(cursor_position, rng, ds_to_test, time_unit);
      break;
    }
    case SAVE_TEST_RESULT: {
      clear_screen();
      print("Enter a filename to save to: ");
      std::string path = user_input<std::string>();
      DSResultSubMenu* c_menu = dynamic_cast<DSResultSubMenu*>(current_submenu);
      if(!c_menu->get_result().save_to_file(path)) { println("FAILED TO SAVE {}", path); }
      current_submenu->print(cursor_position, rng, ds_to_test, time_unit);
      break;
    }
    case BACK: {
      back();
      break;
    }
    case QUIT: {
      running = false;
      break;
    }
    case RNG_5_000: { rng.generate_numbers(N5_000); break; }
    case RNG_8_000: { rng.generate_numbers(N8_000); break; }
    case RNG_10_000: { rng.generate_numbers(N10_000); break; }
    case RNG_16_000: { rng.generate_numbers(N16_000); break; }
    case RNG_20_000: { rng.generate_numbers(N20_000); break; }
    case RNG_40_000: { rng.generate_numbers(N40_000); break; }
    case RNG_60_000: { rng.generate_numbers(N60_000); break; }
    case RNG_100_000: { rng.generate_numbers(N100_000); break; }
    case SELECT_AL: { ds_to_test = DYNAMIC_ARRAY; break; }
    case SELECT_LL: { ds_to_test = LINKED_LIST; break; }
    case SELECT_LLT: { ds_to_test = LINKED_LIST_T; break; }
    case SELECT_DLL: { ds_to_test = DOUBLY_LINKED_LIST; break; }
    case TIME_NANO: { time_unit = NANOSECONDS; break; }
    case TIME_MICRO: { time_unit = MICROSECONDS; break; }
    case RUN_AUTO_TEST: {
      clear_screen();
      print("Enter a directory name where all results will be saved: ");
      std::string path = user_input<std::string>();
      AutoTestSubMenu* c_menu = dynamic_cast<AutoTestSubMenu*>(current_submenu);

      auto_test(rng, path, time_unit, 8);
      c_menu->set_done();
      break;
    }
  }
}

void Menu::back() {
  if(menu_history.len() == 0) { return; }
  MENUTYPE prev_menu = menu_history.pop();
  change_menu(prev_menu, false);
}

void Menu::test_ds() {}


void Menu::run() {
  rng.generate_numbers(RNG_CHOICE::N5_000);
  while(running) {
    clear_screen();
    current_submenu->print(cursor_position, rng, ds_to_test, time_unit);
    char input = user_input<char>();
    switch (input) {
      case 'w':
        if(this->cursor_position > 0) this->cursor_position--;
        break;
      case 's':
        if(this->cursor_position < current_submenu->cursor_limit()) this->cursor_position++;
        break;
      case 'd':
        action();
        break;
      case 'a':
        back();
        break;
      case 'q':
        running = false;
        break;
      default:
        break;
    }
  }
}
