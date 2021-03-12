#ifndef Classes_h
#define Classes_h

#include <utility>

namespace classes {

// ____________________________________________________________________________
// A Class is a namespace containing its members
// in a non-const member function of class X, `this` is an rvalue of type X*
// in a const member function of class X, `this` is an rvalue of type const X*

class ClassWithMembers {
public:
  // _____________________________________
  // Member functions

  auto member_function() const -> void;
  static auto member_function_static() -> void;

  // Inline
  inline auto member_function_inlined_1() -> void;
  auto member_function_inlined_2() -> void{};

private:
  // Visibility: members functions and friends

  auto caputure_this() -> void {
    auto f1 = [this /* caputure by reference */]() { this->member_function(); };

    auto f2 = [*this /* caputure by value */]() { this->member_function(); };
  }

  // _____________________________________
  // Data members

  int _data_member{};
  static int _data_member_static_1;          // Declaration
  static inline int _data_member_static_2{}; // Definition

  // Const
  int const _data_member_const{};
  static int const _data_member_static_const{};

  // Constexpr
  // Error: non-static data member cannot be constexpr
  // int constexpr _data_member_constexpr{0};
  static int constexpr _data_member_static_constexpr{};
};

inline auto member_function_inlined_1() -> void{};

// ____________________________________________________________________________
// Move and copy class

class MoveAndCopyClass {
public:
  // Constructor
  explicit MoveAndCopyClass(int value) : _value{new int{value}} {}

  // Destructor
  ~MoveAndCopyClass() { delete _value; }

  // Copy constructor
  MoveAndCopyClass(MoveAndCopyClass const &instance)
      : _value{instance._value ? new int{*instance._value} : nullptr} {}

  // Move constructor
  MoveAndCopyClass(MoveAndCopyClass &&instance) : MoveAndCopyClass{} {
    this->swap(instance);
  }

  // Assignment
  MoveAndCopyClass &operator=(MoveAndCopyClass instance) {
    this->swap(instance);
    return *this;
  }

private:
  int *_value{nullptr};

  MoveAndCopyClass() = default;

  void swap(MoveAndCopyClass &instance) noexcept {
    std::swap(this->_value, instance._value);
  }
};

// ____________________________________________________________________________
// Move only class

class MoveOnlyClass {
public:
  // Delete copy constructor & copy assignment
  MoveOnlyClass(MoveOnlyClass const &) = delete;
  MoveOnlyClass &operator=(MoveOnlyClass const &) = delete;

  // Constructor
  explicit MoveOnlyClass(int value) : _value{new int{value}} {}

  // Destructor
  ~MoveOnlyClass() { delete _value; }

  // Move constructor
  MoveOnlyClass(MoveOnlyClass &&instance) : MoveOnlyClass() {
    this->swap(instance);
  }

  // Move assignment
  MoveOnlyClass &operator=(MoveOnlyClass &&instance) {
    this->swap(instance);
    return *this;
  }

private:
  int *_value{nullptr};

  MoveOnlyClass() = default;

  void swap(MoveOnlyClass &instance) noexcept {
    std::swap(this->_value, instance._value);
  }
};

// ____________________________________________________________________________
// Const and logical constnes

class ConstAndLogicalConstnes {
private:
  int _value{};

  // Mutable mean that it can be modified even in const object
  mutable int _logical_constness{};

public:
  auto set_value(int value) -> void { _value = value; }

  // _____________________________________
  // Constant Member Functions
  // The member functions of a const object can only be called if they are
  // themselves specified as const members
  // Const does not apply to objects accessed through pointers or references
  auto value() const -> int {
    _logical_constness++;
    // _value++; // Error
    return _value;
  }
};

// _____________________________________________________________________________
// Class with friends
// The members declared friend have access to the implementation of the class

class A; // Forward Declaration

class ClassWithFriends {
public:
  friend auto friend_function(ClassWithFriends const &x) -> int;
  friend class FriendClass;
  friend A;

private:
  int _value{10};
};

// ____________________________________________________________________________
// Members of structs or unions have public access by default

struct AStruct {
  auto foo() -> void {}
};

union AUnion {
  int intValue;
  float floatValue;
  auto foo() -> void {}
};

} // namespace classes

#endif
