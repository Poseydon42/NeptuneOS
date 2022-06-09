#pragma once

#include <Panic.h>

#define TO_STRING_1(Val) #Val
#define TO_STRING(Val) TO_STRING_1(Val)

#define ASSERT(Expr) \
    if (!(Expr)) \
        KernelPanic("Assertion failed(" __FILE__ ":" TO_STRING(__LINE__) ")\n");
