#ifndef Person_hpp
#define Person_hpp

#include <string>

namespace call_cpp_from_c {

class Person {
  std::string _name;

public:
  Person(std::string name) : _name{name} {};

  auto name() -> std::string { return _name; }

  auto setName(std::string name) -> void { _name = name; }
};

} // namespace call_cpp_from_c

#endif
