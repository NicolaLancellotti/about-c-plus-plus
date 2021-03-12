#include "../header.h"
#include <functional>

namespace functions {
// _____________________________________________________________________________
// Pass parameters by lvalue/rvalue reference

auto call_by_lvalue_reference_utility(std::string &x) -> std::string {
  return "by lvalue";
}
auto call_by_rvalue_reference_utility(std::string &&x) -> std::string {
  return "by rvalue";
}

auto call_by_lvalue_reference(std::string &x) -> std::string {
  call_by_lvalue_reference_utility(x);
  // call_by_rvalue_reference_utility(x); // Error
  return "by lvalue";
}

auto call_by_rvalue_reference(std::string &&x) -> std::string {
  call_by_lvalue_reference_utility(x);
  // call_by_rvalue_reference_utility(x); // Error
  return "by rvalue";
}

auto pass_parameters() -> void {
  {
    // By lvalue reference

    std::string string{"Hello, world"};
    assert(call_by_lvalue_reference(string) == "by lvalue");

    //  call_by_lvalue_reference(std::string{"Hello, world"}); // Error
  }

  {
    // By rvalue reference

    assert(call_by_rvalue_reference(std::string{"Hello, world"}) ==
           "by rvalue");

    std::string string{"Hello, world"};
    //    call_by_rvalue_reference(string); // Error

    assert(call_by_rvalue_reference(std::move(string)) == "by rvalue");
  }
}

// _____________________________________________________________________________
// Pass parameters by constant reference

auto concatenate(std::string const &x, std::string const &y) -> std::string {
  return x + y;
}

// _____________________________________________________________________________
// Parameters with default value

auto product(int x, int y = {}) -> int { return x * y; }

// _____________________________________________________________________________
// Inline function

inline auto sum(int x, int y) -> int { return x + y; }

// _____________________________________________________________________________
// constexpr & consteval
// The specifiers imply inline

// It is possible to evaluate the value of the function or variable at compile
// time
constexpr auto constexpr_function(int x) -> int { return x + 1; }

// It produces a compile time constant expression
consteval auto consteval_function(int x) -> int { return x + 1; }

auto constexpr_consteval_test(int x) -> void {
  int const v0 = constexpr_function(x);
  //  int constexpr v1 = constexpr_function(x); // Error
  int const v2 = constexpr_function(1);
  int constexpr v3 = constexpr_function(1);

  //  consteval_function(x); // Error
  int const v4 = consteval_function(1);
  int constexpr v5 = consteval_function(1);
}

// _____________________________________________________________________________
// Lambda

auto lambdas() -> void {

  {
    auto reference_sum{0};     // Capturing value by reference
    auto value{1};             // Capturing value by value
    std::string string{"abc"}; // Capture by move

    std::function<std::string const &(int)> lambda =
        [&sum = reference_sum, value,
         string = std::move(string)](int x) -> std::string const & {
      sum = x + value;
      return string;
    };

    assert(lambda(2) == "abc");
    assert(reference_sum == 3);
  }

  {
    // Capturing all Values by reference or value
    // = -> all by value
    // & -> all by reference
    auto reference_sum{0};
    auto lambda = [&](int value1, int value2) -> int {
      reference_sum = value1 + value2;
      return reference_sum;
    };

    assert(lambda(20, 30) == 50);
    assert(reference_sum == 50);
  }

  {
    // Mix
    auto value10{10};
    auto reference_sum5{0};
    auto lambda5 = [=, &reference_sum5](int value1, int value2) -> int {
      reference_sum5 = value1 + value2 + value10;
      return reference_sum5;
    };
    auto sum5{lambda5(20, 30)};
    assert(reference_sum5 == 60);
    assert(sum5 == 60);
  }

  {
    // mutable
    auto lambda6 = [value = 1](int x) mutable -> int {
      value++;
      return value + x;
    };
    assert(lambda6(1) == 3);
    assert(lambda6(1) == 4);
  }

  {
    // function pointer
    int (*functionPointer)(int) noexcept = [](int x) noexcept { return x; };
    assert(functionPointer(12) == 12);
  }
}
// _____________________________________________________________________________

auto run() -> void {
  // Passing parameters by lvalue/rvalue reference
  pass_parameters();

  // Passing parameters by constant reference
  assert(concatenate("a", "b") == "ab");

  // Parameters with default value
  assert(product(1) == 0);
  assert(product(1, 2) == 2);

  // Inline function
  assert(sum(1, 2) == 3);

  // Lambdas
  lambdas();
}

} // namespace functions
