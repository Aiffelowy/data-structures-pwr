#include "rng.hpp"
#include "misc.hpp"
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <istream>
#include <string>

RNG::RNG() {}

RNG::RNG(const RNG& other) {
  this->length = other.length;
  this->buffer = new int[this->length];
  this->official_len = other.official_len;
  std::copy(other.cbegin(), other.cend(), this->cbegin());
}

RNG& RNG::operator=(const RNG& other) {
  if(this == &other) { return *this; }

  this->length = other.length;
  this->official_len = other.official_len;
  if(this->buffer != nullptr) { delete [] this->buffer; }
  this->buffer = new int[length];
  std::copy(other.cbegin(), other.cend(), this->begin());
  return *this;

}

RNG::~RNG() {
  delete [] buffer;
}

void RNG::generate_numbers(RNG_CHOICE number_of_numbers) {
  if(buffer != nullptr) { delete [] buffer; }
  official_len = number_of_numbers;
  length = number_of_numbers;
  buffer = new int[number_of_numbers];
  srand(time(NULL));
  for(int i = 0; i < number_of_numbers; i++) {
    buffer[i] = rand() % 10000;
  }
}

bool RNG::from_file(std::string path) {
  std::ifstream file;
  std::string number_s;
  int number;
  std::string length_s;
  int length;

  file.open(path);
  if(!file.is_open()) { return false; }
  getline(file, length_s, ' ');
  length = std::stoi(length_s);
  
  if(this->buffer != nullptr) { delete [] buffer; }
  this->length = length;
  this->buffer = new int[this->length];
  this->official_len = RNG_CHOICE(length);
  
  int i = 0;
  while(getline(file, number_s, ' ')) {
    if(i >= length) { break; }
    number = std::stoi(number_s);
    this->buffer[i] = number;
    i++;
  }
  loaded_from_file = true;
  filename = path;
  return true;
}

bool RNG::save_to_file(std::string path) {
  std::ofstream file;
  file.open(path);
  if(!file.is_open()) { return false; }
  file << this->length << " ";
  for( int i : *this ) {
    file << i << " ";
  }
  return true;
}


ArrayList<int> RNG::into_array() {
  ArrayList<int> arr;
  arr.resize(this->length+10);
  for(int i : *this) {
    arr.push(i);
  }
  return arr;
}

LinkedList<int> RNG::into_linkedlist() {
  LinkedList<int> list;
  for(int i : *this) {
    list.push(i);
  }
  return list;
}

LinkedListT<int> RNG::into_linkedlistT() {
  LinkedListT<int> list;
  for(int i : *this) {
    list.push(i);
  }
  return list;
}

DoubleLinkedList<int> RNG::into_doublelinkedlist() {
  DoubleLinkedList<int> list;
  for(int i : *this) {
    list.push(i);
  }
  return list;
}


bool RNG::is_from_file() const { return loaded_from_file; }
std::string RNG::loaded_from() const { return filename; }
RNG_CHOICE RNG::get_length() const { return official_len; }
int* RNG::begin() { return &this->buffer[0]; }
int* RNG::end() { return &this->buffer[length]; }
int* RNG::cbegin() const { return &this->buffer[0]; }
int* RNG::cend() const { return &this->buffer[length]; }

int RNG::operator[](int i) const {
  return buffer[i];
}



