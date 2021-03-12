#ifndef cpplib_h
#define cpplib_h

#include <memory>

namespace clib {
extern "C" {
#include "clib.h"
}

struct NLFooDeleter {
  void operator()(clib::NLFooRef fooRef) { NLFooDelete(fooRef); }
};

using NLFooSmartPointer =
    std::unique_ptr<struct clib::NLOpaqueFoo, NLFooDeleter>;

inline NLFooSmartPointer createFooSmartPointer(int value) {
  return NLFooSmartPointer(clib::NLFooCreate(value));
}

#pragma GCC poison NLFooCreate

} // namespace clib

#endif
