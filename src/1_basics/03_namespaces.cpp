#include "../header.h"

static int value;

namespace namespaces {

// _____________________________________________________________________________

namespace foo {

struct Value {
  int value;
};

int value;

auto something(int x) -> int; // declaration

} // namespace foo

// Definition
auto foo::something(int x) -> int { return x; }

// Namespaces are open
namespace foo {
auto bar(Value x) -> int { return x.value; } // declaration & definition
} // namespace foo

auto namespace_members() -> void {
  assert(foo::something(1) == 1);
  assert(foo::bar({1}) == 1);

  int value;
  value = 0;
  foo::value = 1;
  ::value = 2; // assign to global value
  assert(value == 0);
  assert(foo::value == 1);
  assert(::value == 2);
}

auto argument_dependent_lookup() -> void {
  // ADL Argument-dependent lookup
  // If a function is not found in the context of its use, we look in the
  // namespaces of its arguments
  auto value{foo::Value{10}};
  assert(bar(value) == 10);
}

// _____________________________________________________________________________
// Namespace aliases

auto namespace_aliases() -> void {
  namespace newName = foo;
  newName::value = 300;
  assert(foo::value == 300);
}

// _____________________________________________________________________________
// Unnamed Namespaces
// Useful to protect against the possibility of name clashes
// An unnamed namespace has an implied using directive

namespace {
auto foobaz() -> int { return 1; }
} // namespace

auto unnamed_namespaces() -> void { assert(foobaz() == 1); }

// _____________________________________________________________________________
// Nested namespace

namespace X::Y {}

// Instead of

namespace X {
namespace Y {}
} // namespace X

// _____________________________________________________________________________
// Versioning

namespace MyLib {

inline namespace V2 {
auto f(int x, int y) -> int { return x + y; };
} // namespace V2

namespace V1 {
auto f(int x) -> int { return x + 1; };
} // namespace V1

} // namespace MyLib

auto versioning() -> void {
  assert(MyLib::f(1, 2) == 3);
  assert(MyLib::V1::f(1) == 2);
}

// _____________________________________________________________________________
// Using directive does not add a name to a local scope

auto using_directive() -> void {
  value = 1; // global value

  using namespace foo;
  value = 2; // foo::value

  int value{3}; // local value

  assert(::value == 1);    // local value
  assert(foo::value == 2); // foo::value
  assert(value == 3);      // local value
}

// _____________________________________________________________________________
// Using declaration add a name to a local scope

auto using_declaration() -> void {
  using foo::something;

  value = 1;        // global value
  using foo::value; // hide global value
  value = 2;        // foo::value

  // int value; // Error name "value" already in scope

  assert(::value == 1);    // local value
  assert(foo::value == 2); // foo::value
}

// _____________________________________________________________________________
// Namespace composition

namespace baz {
using namespace foo;
using std::string;
auto bar() -> int { return 0; }
} // namespace baz

auto namespace_composition() -> void {
  assert(baz::bar() == 0);
  assert(baz::something(10) == 10);
  baz::string s{"Hello world"};
}

// _____________________________________________________________________________
// Overloaded functions
// Return types are not considered in overload resolution

namespace overloading {

auto f(int x) -> std::string { return "f_int"; }
auto f(double x) -> std::string { return "f_double"; }

auto overloading_in_namespace() -> void {
  assert(overloading::f(1.0) == "f_double");

  auto f(int x)->std::string;

  auto double_value = 12.90;
  assert(f(double_value) == "f_int");
}

auto g(int x) -> std::string { return "g_int"; }

} // namespace overloading

auto g(double x) -> std::string { return "g_double"; }

auto overloading_func() -> void {
  overloading::overloading_in_namespace();
  auto double_value = 12.90;
  auto int_value = 12;
  {
    assert(g(double_value) == "g_double");
    assert(g(int_value) == "g_double");
  }
  {
    using namespace overloading;
    assert(g(double_value) == "g_double");
    assert(g(int_value) == "g_int");
  }
  {
    using overloading::g;
    assert(g(double_value) == "g_int");
    assert(g(int_value) == "g_int");
  }
  {
    auto (*f)(int)->std::string{&overloading::f};
    assert(f(double_value) == "f_int");
    assert(f(int_value) == "f_int");
  }
}

// _____________________________________________________________________________

auto run() -> void {
  argument_dependent_lookup();
  namespace_members();
  namespace_aliases();
  unnamed_namespaces();
  versioning();
  using_directive();
  using_declaration();
  namespace_composition();
  overloading_func();
}

} // namespace namespaces
