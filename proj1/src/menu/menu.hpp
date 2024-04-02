#ifndef MENU_H
#define MENU_H

#include "misc.hpp"
#include "rng.hpp"
#include "../tests/test.hpp"
#include "enums.hpp"
#include <string>

struct Menu;

class SubMenu {
protected:
  MENUTYPE menu_type;
  ArrayList<ACTION> actions;
  
  std::string title;
  std::string subtitle;
  ArrayList<std::string> choices;


  DS_TO_TEST ds_type;

public:
  virtual ~SubMenu() {}
  SubMenu() {}

  void print(int, RNG&, DS_TO_TEST, TIME_UNIT);
  ACTION get_action(int);
  constexpr MENUTYPE type();
  virtual void print_additional_info() = 0;
  int cursor_limit() const;
};

class TitleSubMenu : public SubMenu {
public:
  ~TitleSubMenu() {}
  TitleSubMenu() {
    menu_type = TITLE;
    actions = {CHANGE_MENU_RNG_SCREEN, CHANGE_MENU_DS_SELECT, CHANGE_MENU_TIME_UNIT, CHANGE_MENU_DS_TEST, CHANGE_MENU_AUTO_TEST, QUIT};
    title = "DATA STRUCTURE DEMO";
    subtitle = "up: w, down: s, select: d, back: a";
    choices = { "Configure RNG", "Select Data Structure", "Change Time Unit", "Test Data Structure", "Auto Test", "Quit" };
  }
  void print_additional_info() {}
};

class RNGScreenSubMenu : public SubMenu {
public:
  ~RNGScreenSubMenu() {}
  RNGScreenSubMenu() {
    menu_type = RNG_SCREEN;
    actions = {CHANGE_MENU_RNG_SELECT, SAVE_RNG, LOAD_RNG, BACK};
    title = "RNG Configuration";
    subtitle = "";
    choices = { "Change the amount of numbers", "Save the current RNG status to file", "Load the RNG status from a file", "Back" };
  }
  void print_additional_info() {}
};

class RNGSelectSubMenu : public SubMenu {
public:
  ~RNGSelectSubMenu() {}
  RNGSelectSubMenu() {
    menu_type = RNG_SELECT_NUMBER;
    actions = { RNG_5_000, RNG_8_000, RNG_10_000, RNG_16_000, RNG_20_000, RNG_40_000, RNG_60_000, RNG_100_000, BACK };
    title = "RNG Configuration";
    subtitle = "please select the amount of numbers";
    choices = { "5_000", "8_000", "10_000", "16_000", "20_000", "40_000", "60_000", "100_000", "Back" };
  }
  void print_additional_info() {}
};

class DSSelectSubMenu : public SubMenu {
public:
  ~DSSelectSubMenu() {}
  DSSelectSubMenu() {
    menu_type = DS_SELECT;
    actions = { SELECT_LL, SELECT_LLT, SELECT_DLL, SELECT_AL, BACK };
    title = "DATA STRUCTURE SELECTION";
    subtitle = "please select the data structure you wold like to test";
    choices = { "Linked List", "Linked List (with tail)", "Doubly Linked List", "Dynamic Array", "Back" };
  }
  void print_additional_info() {}
};

class DSResultSubMenu : public SubMenu {
private:
  TestResults result;
public:
  ~DSResultSubMenu() {}
  DSResultSubMenu() {
    menu_type = DS_TEST;
    actions = { SAVE_TEST_RESULT, BACK };
    title = "Data Structure Test Result";
    subtitle = "";
    choices = { "Save test results to the file", "Back" };
  }
  void load_test_result(TestResults r) { result = r; } 
  void print_additional_info() {
    result.print_tests();
  }
  const TestResults& get_result() const { return result; }
};

class TimeUnitSubMenu : public SubMenu {
public:
  ~TimeUnitSubMenu() {}
  TimeUnitSubMenu() {
    menu_type = TIME_UNIT_SELECT;
    actions = { TIME_NANO, TIME_MICRO, BACK };
    title = "Time Unit Selection";
    subtitle = "please select the time unit";
    choices = { "Nanoseconds", "Microseconds", "Back" };
  }
  void print_additional_info() {}
};

class AutoTestSubMenu : public SubMenu {
  bool done = false;
public:
  ~AutoTestSubMenu() {}
  AutoTestSubMenu() {
    menu_type = AUTO_TEST;
    actions = { RUN_AUTO_TEST, BACK };
    title = "Auto Test";
    subtitle = "Will test all structures with all possible RNG configurations";
    choices = { "Run Auto Test", "Back" };
  }
  void set_done() { done = true; }
  void print_additional_info() {
    if(done) { println("Done!"); }
  }
};


struct Menu {
private:
  RNG rng;
  SubMenu* current_submenu = new TitleSubMenu();
  int cursor_position = 0;
  //not a stack, but i will use it like one
  ArrayList<MENUTYPE> menu_history;
  bool running = true;
  DS_TO_TEST ds_to_test = LINKED_LIST;
  TIME_UNIT time_unit = MICROSECONDS;
  TestResults test;
  
  void print_menu(std::string, std::string, ArrayList<std::string>);
  void change_menu(MENUTYPE, bool);
  void action();
  void back();

  void test_ds(void);
  
public:
  Menu();
  ~Menu();
  
  void run();
};





#endif
