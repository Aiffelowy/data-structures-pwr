#ifndef RNG_H
#define RNG_H

#include "../array_list/array_list.hpp"
#include "../linked_list/linked_list.hpp"
#include "../linked_list/linked_list_tail.hpp"
#include "../double_linked_list/Double_Linked_List.h"
#include "enums.hpp"

#include <string>

struct RNG {
private:
  int* buffer = nullptr;
  int length;
  RNG_CHOICE official_len;
  bool loaded_from_file = false;
  std::string filename;

public:
  RNG();
  RNG(const RNG&);
  RNG& operator=(const RNG&);
  ~RNG();
  int* begin();
  int* end();
  int* cbegin() const;
  int* cend() const;
  void generate_numbers(RNG_CHOICE);
  bool from_file(std::string);
  bool save_to_file(std::string);
  bool is_from_file() const;
  std::string loaded_from() const;
  RNG_CHOICE get_length() const;
  ArrayList<int> into_array(void);
  LinkedList<int> into_linkedlist(void);
  LinkedListT<int> into_linkedlistT(void);
  DoubleLinkedList<int> into_doublelinkedlist(void);
  int operator[](int) const;
};




#endif
