#pragma once

#include <stddef.h>
#include <stdint.h>

namespace Common
{
    void* Memset(void* Dest, int Value, size_t Size);

    void* Memcpy(void* Dest, const void* Source, size_t Size);

    void* Memmove(void* Dest, const void* Source, size_t Size);
} // namespace Common

namespace Common::Memory
{
    uint8_t* AlignToNearestHigher(const uint8_t* Ptr, size_t Alignment);
}
