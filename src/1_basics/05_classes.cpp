#include "../header.h"
#include "04_classes.h"

namespace classes {

// _____________________________________
// Members

// Member functions
auto ClassWithMembers::member_function() const -> void{};
auto ClassWithMembers::member_function_static() -> void{};
// Data members
int ClassWithMembers::_data_member_static_1{0};

// _____________________________________________________________________________
// Friends

auto friend_function(ClassWithFriends const &x) -> int { return x._value; }

class FriendClass {
public:
  auto value(ClassWithFriends &x) -> int { return x._value; }
};

class A {};

// _____________________________________________________________________________

auto run() -> void {
  {
    // Const and logical constnes
    ConstAndLogicalConstnes x{};
    x.set_value(10);
    assert(x.value() == 10);

    auto const &ref_x{x};
    assert(ref_x.value() == 10);
    // ref_x.set_value(10); // Error
  }
  {
    // Delete Constructor
    struct Z {
      Z(double) {}     // can initialize with a double
      Z(int) = delete; // but not with an integer
    };
    Z z{1.1};
    // Z z{1}; // Error
  }
}
} // namespace classes
