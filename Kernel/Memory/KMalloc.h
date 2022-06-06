#pragma once

#include <stddef.h>
#include <stdint.h>

namespace Kernel
{
    void KMallocInitialize();

    void* KMalloc(size_t Size, size_t Alignment = 8);

    void KFree(void* Ptr);

} // namespace Kernel

[[nodiscard]] void* operator new(size_t Size);
[[nodiscard]] void* operator new[](size_t Size);
[[nodiscard]] void* operator new(size_t Size, size_t Alignment);
[[nodiscard]] void* operator new[](size_t Size, size_t Alignment);
void operator delete(void* Ptr);
void operator delete[](void* Ptr);
void operator delete(void* Ptr, size_t Alignment);
void operator delete[](void* Ptr, size_t Alignment);
