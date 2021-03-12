#include "../header.h"
#include "call_c_from_cpp/cpplib.h"
#include "call_cpp_from_c/NLPersonRef.h"
#include <cstring>
#include <iostream>

// _____________________________________________________________________________
// Call C from C++

auto call_c_from_cpp() -> void {
  assert(clib::sum(1, 2) == 3);
  assert(std::strcmp("ABC", "abc") == -1);

  auto fooPointer = clib::createFooSmartPointer(1);
  assert(clib::NLFooGetValue(fooPointer.get()) == 1);
}

// _____________________________________________________________________________
// Call C++ from C

auto call_cpp_from_c() -> void {
  NLPersonRef person{NLPersonCreate("Name1")};
  {
    char *name{NLPersonGetName(person)};
    assert(!strcmp(name, "Name1"));
    NLStringDelete(name);
  }

  NLPersonSetName(person, "Name2");

  {
    char *name{NLPersonGetName(person)};
    assert(!strcmp(name, "Name2"));
    NLStringDelete(name);
  }

  NLPersonDelete(person);
}

// _____________________________________________________________________________

extern "C" void interoperability_func_1(); // declaration
extern "C" int interoperability_x1;        // declaration

extern "C" {
void interoperability_func_2(); // declaration
extern int interoperability_x2; // declaration
int interoperability_x3{};      // definition
};

auto interoperability_func_1() -> void {} // definition
auto interoperability_func_2() -> void {} // definition
int interoperability_x1{0};               // definition
int interoperability_x2{0};               // definition

// _____________________________________________________________________________

namespace interoperability {

auto run() -> void {
  call_c_from_cpp();
  call_cpp_from_c();
}

} // namespace interoperability
