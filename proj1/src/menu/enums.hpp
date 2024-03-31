#ifndef ENUMS_H
#define ENUMS_H

#include<iostream>

enum TIME_UNIT {
  NANOSECONDS,
  MICROSECONDS
};

std::ostream& operator<<(std::ostream&, const TIME_UNIT&);

enum MENUTYPE {
  TITLE,
  RNG_SCREEN,
  RNG_SELECT_NUMBER,
  DS_SELECT,
  DS_TEST,
  TIME_UNIT_SELECT,
  AUTO_TEST
};

enum DS_TO_TEST {
  LINKED_LIST,
  LINKED_LIST_T,
  DOUBLY_LINKED_LIST,
  DYNAMIC_ARRAY
};

std::string to_string(const DS_TO_TEST&);
std::ostream& operator<<(std::ostream&, const DS_TO_TEST&);

enum ACTION {
  CHANGE_MENU_TITLE,
  CHANGE_MENU_RNG_SCREEN,
  CHANGE_MENU_RNG_SELECT,
  CHANGE_MENU_DS_SELECT,
  CHANGE_MENU_DS_TEST,
  CHANGE_MENU_TIME_UNIT,
  CHANGE_MENU_AUTO_TEST,
  RUN_AUTO_TEST,
  LOAD_RNG,
  SAVE_RNG,
  SAVE_TEST_RESULT,
  RNG_5_000,
  RNG_8_000,
  RNG_10_000,
  RNG_16_000,
  RNG_20_000,
  RNG_40_000,
  RNG_60_000,
  RNG_100_000,
  SELECT_LL,
  SELECT_LLT,
  SELECT_DLL,
  SELECT_AL,
  TIME_NANO,
  TIME_MICRO,
  BACK,
  QUIT
};

enum RNG_CHOICE {
  N5_000 = 5000,
  N8_000 = 8000,
  N10_000 = 10000,
  N16_000 = 16000,
  N20_000 = 20000,
  N40_000 = 40000,
  N60_000 = 60000,
  N100_000 = 100000,
};

std::string to_string(const RNG_CHOICE&);
std::ostream& operator<<(std::ostream&, const RNG_CHOICE&);




#endif
