#pragma once

#include <Templates/TypeTraits.h>
#include <cstdint>

namespace Common
{
    /*
     * Returns number of bits to the right from most significant set bit
     * NOTE : when Value is zero behaviour is undefined
     */
    template<typename T>
        requires(Integral<T>)
    uint32_t FindMostSignificantSetBit(T Value)
    {
        // TODO : intrinsics
        uint32_t Result = 0;
        while (Value)
        {
            Result++;
            Value /= 2;
        }
        return Result - 1;
    }

    /*
     * Returns number of bits to the right from most significant clear bit
     * NOTE : when Value is equal to 2^n - 1, where n is size in bits of Value, behaviour is undefined
     */
    template<typename T>
        requires(Integral<T>)
    uint32_t FindMostSignificantClearBit(T Value)
    {
        // TODO : intrinsics
        uint32_t Result = 0;
        for (uint32_t BitIndex = 0; BitIndex < sizeof(Value) * 8; BitIndex++)
        {
            if ((Value & (1 << BitIndex)) == 0)
                Result = BitIndex;
        }
        return Result;
    }
} // namespace Common
