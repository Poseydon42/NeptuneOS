#pragma once

#include <Templates/Move.h>

namespace Common
{
    template<typename T>
    void Swap(T& A, T& B)
    {
        T Tmp = Move(A);
        A = Move(B);
        B = Move(Tmp);
    }
} // namespace Common
