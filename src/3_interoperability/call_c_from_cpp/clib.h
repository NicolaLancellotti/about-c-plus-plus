#ifndef clib_h
#define clib_h

int sum(int x, int y);

typedef struct NLOpaqueFoo *NLFooRef;
NLFooRef NLFooCreate(int x);
void NLFooDelete(NLFooRef fooRef);
int NLFooGetValue(NLFooRef fooRef);

#endif
