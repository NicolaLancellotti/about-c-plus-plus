#ifndef hierarchies_h
#define hierarchies_h

#include <string>

namespace hierarchies {

// ____________________________________________________________________________
// Abstract class

class AbstractClass {
public:
  // A virtual member function is sometimes called method
  virtual auto abstract_member() -> void = 0; // Pure virtual method

  virtual ~AbstractClass(){};
};

// ____________________________________________________________________________
// Some classes

class ClassA {
public:
  explicit ClassA(int value) {}

  virtual ~ClassA(){};

  auto public_member() -> void{};

  auto abstract_member() -> std::string { return "ClassA: abstract_member"; }

protected:
  // visibility: members functions and friends of this class and
  //             members functions and friends of derived classes
  auto protected_member() -> void{};
};

class ClassB {
public:
  ClassB() {}

  explicit ClassB(bool value) {}

  virtual ~ClassB(){};

  auto abstract_member() -> std::string { return "ClassB: abstract_member"; }
};

// ____________________________________________________________________________
/*
A derived class inherits access to every member of a base class except:
its constructors and its destructors
its assignment operators
its friends
its private members

Access Control of base classes
--------------------------------------------------------------------------
base access | base memebers    | derived memebers | default for | subtype
------------|------------------| -----------------|-------------|---------
public      | public           | public           | struct      | YES
            | protected        | protected        |             |
------------|------------------| -----------------|-------------|---------
protected   | public/protected | protected        |             | NO
------------|------------------| -----------------|-------------|---------
private     | public/protected | private          | class       | NO
*/

class Derived : public AbstractClass, protected ClassA, private ClassB {
public:
  // _____________________________________
  // Constructors
  Derived(bool value) : ClassA(10), ClassB(value) {}

  // Default constructor for the other classes
  Derived() : ClassA(10) {}

  // Override
  // Final memebers: prevent futher overriding
  auto abstract_member() -> void override final {
    public_member();
    protected_member();

    {
      // Explicit qualification ensures that the virtual mechanism is not used
      std::string x{ClassA::abstract_member()};

      Derived y{};
      y.abstract_member();
      y.ClassA::abstract_member();
    }
  }

  virtual ~Derived(){};
};

// Final classes: prevent futher overriding
class FinalClass final : public Derived {
  auto baz() -> void {
    ClassA::abstract_member();
    // ClassB::abstract_member(); // Error
  }
};

} // namespace hierarchies

#endif
