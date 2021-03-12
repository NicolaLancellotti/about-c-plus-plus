#include "../header.h"

namespace casts {

// _____________________________________________________________________________

class Base {
public:
  virtual ~Base() {}
};

class Derived : public Base {};

// _____________________________________________________________________________

auto run() -> void {
  float const float_value{3.14};
  int i;

  // _____________________________________
  // C style cast
  i = (int)float_value;

  // _____________________________________
  // Function style cast
  i = int(float_value);

  // _____________________________________
  // static_cast
  // Convert between related types
  i = static_cast<int>(float_value);
  Base *b{new Derived()};
  Derived *d2{static_cast<Derived *>(b)};

  // _____________________________________
  // reinterpret_cast
  // Convert between unrelated types
  void *p = nullptr;
  int *p2{reinterpret_cast<int *>(p)};

  // _____________________________________
  // const_cast
  // Convert between types that differ only
  // in const and volatile qualifiers
  const char *cs = nullptr;
  char *s{const_cast<char *>(cs)};

  // _____________________________________
  // dynamic_cast
  // Base must be polymorphic
  {
    Base *base{new Derived{}};
    Derived *derived{dynamic_cast<Derived *>(base)};
    assert(derived != nullptr);
    delete base;
  }

  {
    Derived derived{};
    Base &base_ref{derived};
    try {
      Derived &derived_ref{dynamic_cast<Derived &>(base_ref)};
    } catch (std::bad_cast) {
    }
  }
}

} // namespace casts
