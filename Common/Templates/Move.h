#pragma once

#include <Templates/RemoveReference.h>

namespace Common
{
    template<typename T>
    RemoveReference<T>&& Move(T&& Arg)
    {
        return static_cast<RemoveReference<T>&&>(Arg);
    }
} // namespace Common
