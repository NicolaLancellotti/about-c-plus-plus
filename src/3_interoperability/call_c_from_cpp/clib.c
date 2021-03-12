#include "clib.h"
#include <stdlib.h>

int sum(int x, int y) {
  return x + y;
}

typedef struct {
  int x;
} Foo;

inline static Foo* unwrap(NLFooRef fooRef) {
  return (Foo*)fooRef;
}

NLFooRef NLFooCreate(int value) {
    NLFooRef p = malloc(sizeof(Foo));
    unwrap(p)->x = value;
    return p;
}

void NLFooDelete(NLFooRef fooRef) {
    free(fooRef);
}

int NLFooGetValue(NLFooRef fooRef) {
  return unwrap(fooRef)->x;
}
