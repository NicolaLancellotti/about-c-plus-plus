#include "../header.h"
#include "05_manual_control_instantiation.h"
#include <array>
#include <vector>

namespace templates {

/*
 Template specialization: a version of a template for a specific template
 argument list

 Template instantiation: the process of generating a class or a function
 from a template plus a template argument list
 -  Dependent name binding: template instantiation time
    (nearest global or namespace scope)
 -  Nondependent name binding: template definition time
 */

// _____________________________________________________________________________
// Template Class
template <typename T> class TemplateClass {
public:
  // type aliases (associated types)
  using value_type = T;

  TemplateClass(T value = {}) : _value{value} {};

  auto member_function() -> T &;

  static auto static_member_function() -> void {}

  // cannot be virtual
  template <typename Z> auto template_member_function(Z z) const -> Z;

  // Avoid nested types unless they relay on every template parameter
  class A : T {
    T x;
  };

  // Friend
  template <typename K> friend class FriendClass;

private:
  T _value{};
};

// Definitions
template <typename T> auto TemplateClass<T>::member_function() -> T & {
  return _value;
}

template <typename T>
template <typename Z>
auto TemplateClass<T>::template_member_function(Z z) const -> Z {
  return z;
};

// Friend
template <typename T> struct FriendClass {
  static auto foo(TemplateClass<T> &x) -> T { return x._value; }
};

template <typename T> auto template_class_utility(T const &y) {
  // You need `typename` because by default a dependent name is
  // assumed to name something that is not a type
  typename T::value_type x{};

  // Naming a member template require the use of the keyword template
  // (y.template_member_function < int) > (10)
  y.template template_member_function<int>(10);

  y.template_member_function(10);
}

auto template_class() -> void {
  TemplateClass<int> x1{0};
  TemplateClass x2{0};

  // Members
  TemplateClass<int>::value_type value{x1.member_function()};
  TemplateClass<int>::static_member_function();

  x1.template_member_function<int>(false);
  template_class_utility(x1);

  // Friends
  FriendClass<int>::foo(x1);
}

// _____________________________________________________________________________
// Template parameters

struct UtilityClass {
  auto h(int b) -> void {}
  inline static int x{10};
};

template <typename T, typename U> class B {};

template <
    /* type parameters */
    typename T = int, typename Compare = std::less<T>,
    template <typename, typename> typename TemplateClass = B,
    /* a value parameter can be:
     - Integral or enumeration
     - Reference type
     - Pointer type
      */
    int N = T{}, int const &rx = UtilityClass::x, int const *px = nullptr,
    bool (*pf1)(T const &, T const &) = nullptr,
    void (UtilityClass::*pf2)(int) = nullptr>
struct TemplateParameters {
  TemplateParameters(Compare c = {}) : cmp{c} {};
  std::array<T, N> array{};
  Compare cmp{};
  TemplateClass<int, T> templateClass{};
};

auto template_parameters() -> void {
  auto cmp{[](int const &, int const &) -> bool { return false; }};

  TemplateParameters</* T = */ int, /* Compare = */ std::greater<int>,
                     /* TemplateClass = */ B,
                     /* N = */ 10,
                     /* rx = */ UtilityClass::x, /* px = */ &UtilityClass::x,
                     /* pf1 = */ cmp, /* A::*pf2 = */ &UtilityClass::h>
      x1{};

  TemplateParameters</*T = */ int, /* Compare = */ decltype(cmp), B,
                     /* N = */ 10,
                     /* rx = */ UtilityClass::x, /* px = */ &UtilityClass::x,
                     /* pf1 = */ cmp, /* A::*pf2 = */ &UtilityClass::h>
      x2{cmp};

  TemplateParameters x3{};
}

// _____________________________________________________________________________
// User Defined Specialization: Interface OR Implementation Specialization
//  - All specializations of a template must be declared in the same namespace
//    as the primary template (most general template).
//  - The most specialized version will be preferred over the others in
//    declarations
//  - There is no "inheritance" of members from the generic template to the
//    specialization.

// Primary template
template <typename T> class Vector {
public:
  explicit Vector(int size) : _buffer{new T[size]} {}
  ~Vector() { delete[] _buffer; }
  T &operator[](int index) { return _buffer[index]; };

private:
  T *_buffer;
};

// Complete specialization
template <> class Vector<void *> {
public:
  explicit Vector(int size) : _buffer{new void *[size]} {}
  ~Vector() { delete[] _buffer; }
  void *&operator[](int index) { return _buffer[index]; };

private:
  void **_buffer;
};

// Partial specialization
template <typename T> class Vector<T *> : private Vector<void *> {
  using Base = Vector<void *>;

public:
  using Base::Base;
  T *&operator[](int i) { return reinterpret_cast<T *&>(Base::operator[](i)); }
};

auto user_defined_specialization() -> void {
  {
    Vector<int> v{2};
    v[0] = 0;
    v[1] = 1;
    assert(v[0] == 0);
    assert(v[1] == 1);
  }

  {
    int a{0};
    int b{1};
    Vector<int *> v{2};
    v[0] = &a;
    v[1] = &b;
    assert(*v[0] == 0);
    assert(*v[1] == 1);
  }
}

// _____________________________________________________________________________
// Template Functions

template <typename T, int N = 1> auto increment(T val) -> T { return val + N; }

// overloading
template <typename T> auto foo(T x) -> T { return x; }

template <typename T> auto foo(std::vector<T> x) -> int { return x[0]; }

auto foo(int x) -> int { return x; }

// Specialization (only complete)
template <typename T> auto default_value() -> T; // only declaration

template <> auto default_value<int>() -> int { return 0; }

template <> auto default_value<char>() -> char { return 'a'; }

// With overloading
auto default_value(int /* dummy argument*/) -> int { return 0; }
auto default_value(char /* dummy argument*/) -> char { return 'a'; }

auto function_template() -> void {
  // Function template
  auto result{increment<float, 2>(10)};
  assert(result == 12);

  auto result2{increment(10)};
  assert(result2 == 11);

  // overloading

  auto x1{foo(1.1)};
  assert(x1 == 1.1);

  auto x2{foo(std::vector{'a'})};
  assert(x2 == 'a');

  auto x3{foo(1)};
  assert(x3 == 1);

  int x4{foo<int>('a')}; // explicit specialization
  assert(x4 == 97);

  // Specialization

  assert(default_value<int>() == 0);
  assert(default_value<char>() == 'a');
  assert(default_value(0) == 0);
  assert(default_value('z') == 'a');
}

// _____________________________________________________________________________
// Template Variables

template <typename T> constexpr T pi = 3.141592653589793238462643383;

template <> constexpr char const *pi<char const *>{"pi"};

auto variable_templates() -> void {
  assert(pi<int> == 3);
  assert(std::string{pi<const char *>} == "pi");
  assert(pi<double> > 3.14 || pi<double> < 3.15);
}

// _____________________________________________________________________________
// Template aliases

using TemplateAlias1 = TemplateClass<int>; // type

template <typename T> using TemplateAlias2 = TemplateClass<T>; // template

// _____________________________________________________________________________
// Generic lambdas

struct {
  template <typename T, typename U> auto operator()(T x, U y) const {
    return x + y;
  }
} lambda1{};

auto generic_lambdas() -> void {
  auto lambda = []<typename T, typename U>(T x, U y) { return x + y; };

  assert(lambda(1, 2) == 3);
  assert(lambda(std::string{"Hello"}, std::string{" world"}) == "Hello world");

  // equivalent to
  assert(lambda1(1, 2) == 3);
  assert(lambda1(std::string{"Hello"}, std::string{" world"}) == "Hello world");
}

// _____________________________________________________________________________
// Variadic Templates

// Fold expressions
template <typename... Args> auto fold_expression_unary_left_div(Args... args) {
  return (... / args); // (x1 / x2) / x3
}

template <typename... Args> auto fold_expression_binary_left_sum(Args... args) {
  return (0 + ... + args); // ((0 + x1) + x2) + x3
}

template <typename... Args> auto fold_expression_unary_right_sum(Args... args) {
  return (args + ...); // x1 + (x2 + x3)
}

template <typename... Args>
auto fold_expression_binary_right_sum(Args... args) {
  return (args + ... + 0); // x1 + (x2 + (x3 + 0))
}

// Variadic Functions
template <typename T, typename... Tail /* template parameter pack */,
          typename = std::enable_if_t<(std::is_same_v<T, Tail> && ...)>>
auto variadic_function_sum(T head, Tail... tail /* function parameter pack */)
    -> int {
  if constexpr (sizeof...(tail) > 0) {
    return head + variadic_function_sum(tail...); // pack expansion
  } else {
    return head;
  }
}

// Lambda: capture variadic
template <typename... Args> auto lambda_caputure_variadic(Args... args) -> int {
  auto lambda = [&args...]() -> int {
    return fold_expression_binary_left_sum(args...);
  };
  return lambda();
};

// Variadic Class
template <typename... Bases> class VariadicClass : public Bases... {
public:
  template <typename = std::enable_if_t<2 < sizeof...(Bases)>>
  VariadicClass(Bases const &...b) : Bases(b)... {}
};

auto variadic_templates() -> void {
  assert(variadic_function_sum(1, 2, 3) == 6);

  assert(fold_expression_unary_left_div(18, 2, 3) == 3);
  assert(fold_expression_binary_left_sum(1, 2, 3) == 6);
  assert(fold_expression_binary_left_sum() == 0);

  assert(fold_expression_unary_right_sum(1, 2, 3) == 6);
  assert(fold_expression_binary_right_sum(1, 2, 3) == 6);
  assert(fold_expression_binary_right_sum() == 0);

  assert(lambda_caputure_variadic(1, 2, 3) == 6);

  {
    struct A {
      A(int x){};
    };
    struct B {
      B(double x){};
    };
    struct C {
      C(bool x){};
    };

    VariadicClass<A, B, C> x1{1, 2.2, true};
    // VariadicClass<A, B> x2{1, 2.2}; // Error
  }
}

// _____________________________________________________________________________
// Substitution Failure Is Not An Error

// ignored for T = int*
template <typename T> auto sfinae(T &x) -> typename T::value_type & {
  return x[0];
}

auto substitution_failure_is_not_an_error() -> void {
  std::vector<int> a{11};
  assert(sfinae(a) == 11);

  // auto b{10};
  // sfinae(&b); // Error
}

// _____________________________________________________________________________
// Deduction guides

template <typename T1, typename T2> struct Simple_Pair {
  T1 first;
  T2 second;
};

// User-defined deduction guide
template <typename T1, typename T2> Simple_Pair(T1, T2) -> Simple_Pair<T1, T2>;

auto deduction_guide() { Simple_Pair s{1, 2}; }

// _____________________________________________________________________________
// Template Argument Deduction

template <typename T> auto template_argument_deduction(T &&x) {
  /*
      T   | T&&
      ___ | ___________
      U   | U&&
      U&  | U&&& = U&
      U&& | U&&&& = U&&
  */
  x++;
}

auto template_argument_deduction_test() -> void {
  auto x{1};
  template_argument_deduction(x); // T = &int -> T&& = int&
  assert(x == 2);

  template_argument_deduction(1); // T = int -> T&& = int&&
}

// _____________________________________________________________________________
// Forwarding
// A forward produces an rvalue from an rvalue only

/*
template< class T > struct remove_reference      {using type = T;};
template< class T > struct remove_reference<T&>  {using type = T;};
template< class T > struct remove_reference<T&&> {using type = T;};

template<typename T>
T&& forward(std::remove_reference<T>::type& t) noexcept {
  return static_cast<T&&>(t); // Return T&
}
template<typename T>
T&& forward(std::remove_reference<T>::type&& t) noexcept {
  return static_cast<T&&>(t); // Return T&&
}
*/

auto call_by_reference(std::string const &x) -> std::string { return "lvalue"; }
auto call_by_reference(std::string &&x) -> std::string { return "rvalue"; }

template <class T> auto forwarding(T &&x) -> std::string {
  // `forwarding` can be called with an lvalue or with an rvalue

  // All named values always evaluate as lvalues
  // This poses difficulties in preserving potential move semantics on template
  // functions that forward arguments to other functions
  assert(call_by_reference(x) == "lvalue");

  return call_by_reference(std::forward<T>(x));
}

auto forwarding_test() -> void {
  std::string x{"Hello"};
  assert(forwarding(x) == "lvalue");
  assert(forwarding(std::string{"world"}) == "rvalue");
}

// _____________________________________________________________________________
// Manual Control Instantiation

auto manual_control_instantiation() -> void {
  namespace mci = manual_control_instantiation;

  mci::MCI_Class<bool> x{};
  x.foo();

  mci::mci_func<int>();
}

// _____________________________________________________________________________
// CRTP - The curiously recurring template pattern

template <typename Derived, typename SuperBase>
struct BaseCRTP : public SuperBase {
  auto bar(Derived const &d) const -> int { return d.foo(); };
};

struct SuperBase {
  auto foo() const -> int { return 10; };
};

struct DerivedCRTP : public BaseCRTP<DerivedCRTP, SuperBase> {};

auto crtp() -> void {
  DerivedCRTP x{};
  assert(x.bar(x) == 10);
}

// _____________________________________________________________________________
// Concepts

template <typename T>
concept FooConcept = requires(T a) {
  { a.foo() } -> std::same_as<bool>;
};

template <typename T>
concept BarConcept = requires(T a) {
  { a.bar() } -> std::same_as<bool>;
};

template <typename T>
  requires FooConcept<T> && BarConcept<T>
auto func_with_concept_1(T *value) -> bool {
  return value->foo() && value->bar();
}

template <typename T>
auto func_with_concept_2(T *value) -> bool
  requires FooConcept<T> && BarConcept<T>
{
  return value->foo() && value->bar();
}

template <FooConcept T> auto func_with_concept_3(T *value) -> bool {
  return value->foo();
}

auto func_with_concept_4(FooConcept auto *value) -> bool {
  return value->foo();
}

class A {
public:
  auto foo() -> bool { return true; }
  auto bar() -> bool { return true; }
};

auto concepts() -> void {
  A a{};
  func_with_concept_1(&a);
  func_with_concept_1(&a);
  func_with_concept_3(&a);
}

// _____________________________________________________________________________

auto run() -> void {
  template_class();
  template_parameters();
  user_defined_specialization();
  function_template();
  variable_templates();
  generic_lambdas();
  variadic_templates();
  substitution_failure_is_not_an_error();
  deduction_guide();
  template_argument_deduction_test();
  forwarding_test();
  manual_control_instantiation();
  crtp();
  concepts();
}

} // namespace templates
