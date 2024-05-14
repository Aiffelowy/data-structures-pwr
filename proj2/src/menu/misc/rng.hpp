#ifndef RNG_H
#define RNG_H


#include "../error/errors.hpp"
#include "../error/result.hpp"

#include <cstdlib>
#include <ctime>
#include <fstream>


namespace TestThings {

struct RNG {
private:
  int length;
  int* buffer = nullptr;

public:
  RNG() {}

  int* begin() {
    return &buffer[0];
  }

  int* end() {
    return &buffer[length];
  }

  int* begin() const {
    return &buffer[0];
  }

  int* end() const {
    return &buffer[length];
  }



  int* cbegin() const {
    return &buffer[0];
  }

  int* cend() const {
    return &buffer[length];
  }

  RNG(const RNG& other) {
    this->length = other.length;
    this->buffer = new int[this->length];
    std::copy(other.cbegin(), other.cend(), this->begin());
  }

  ~RNG() {
    delete [] buffer;
  }

  RNG& operator=(const RNG& other) {
    if(this == &other)
      return *this;

    this->length = other.length;
    if(this->buffer != nullptr)
      delete [] this->buffer;

    this->buffer = new int[length];
    std::copy(other.cbegin(), other.cend(), this->begin());
    return *this;
  }

  int& operator[](int index) {
    return buffer[index];
  }

  int& operator[](int index) const {
    return buffer[index];
  }

  int len() const {
    return length;
  }

  void generate_numbers(int nnumbers, int max_number = INT_MAX) {
    if(buffer != nullptr) 
      delete [] buffer;
  
    length = nnumbers;
    buffer = new int[nnumbers];
    srand(time(NULL));
    for(int i = 0; i < nnumbers; i++)
      buffer[i] = rand() % max_number;
  }


  Result::Result<Result::Nothing, Errors::IOError> save_to_file(std::string path) {
    std::ofstream file(path);
    if(file.fail()) 
      return Result::Err(Errors::IOError::CoulntOpenFile);

    file << this->length;
    for(const int& i : *this)
      file << i << " ";
    
    return Result::Ok();
  }

  Result::Result<Result::Nothing, Errors::IOError> from_file(std::string path) {
    std::ifstream file(path);
    if(file.fail()) 
      return Result::Err(Errors::IOError::CoulntOpenFile);

    std::string number_s;
    std::string length_s;
    int number;
    int length;
    std::getline(file, length_s, ' ');
    
    try {
      length = std::stoi(length_s);
    } catch(std::invalid_argument) {
      return Result::Err(Errors::IOError::InvalidArgument);
    } catch (std::out_of_range) {
      return Result::Err(Errors::IOError::OutOfRange);
    }

    if(this->buffer != nullptr) 
      delete [] buffer;
    
    this->length = length;
    this->buffer = new int[this->length];
    int i = 0;
    while(std::getline(file, number_s, ' ')) {
      if(i >= length)  
        break;
    
      try {
        number = std::stoi(number_s);
      } catch(std::invalid_argument) {
        return Result::Err(Errors::IOError::InvalidArgument);
      } catch (std::out_of_range) {
        return Result::Err(Errors::IOError::OutOfRange);
      }

      this->buffer[i] = number;
      i++;
    }

    return Result::Ok();
  }
};
}
#endif
