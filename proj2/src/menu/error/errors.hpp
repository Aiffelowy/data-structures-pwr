#ifndef ERRORS_H
#define ERRORS_H

namespace Errors {

enum class IOError {
  Ok,
  FileNotFound,
  PermissionDenied,
  CoulntOpenFile,
  InvalidArgument,
  OutOfRange
};

enum class InputError {
  Ok,
  WrongDataType,
  Overflow,
};

}

#endif
