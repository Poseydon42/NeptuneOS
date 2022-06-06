#include "Memory.h"

namespace Common
{
    void* Memset(void* Dest, int Value, size_t Size)
    {
        auto* DestPtr = static_cast<unsigned char*>(Dest);
        while (Size--)
            *DestPtr++ = static_cast<unsigned char>(Value);
        return Dest;
    }

    void* Memcpy(void* Dest, const void* Source, size_t Size)
    {
        auto* DestPtr = static_cast<unsigned char*>(Dest);
        const auto* SourcePtr = static_cast<const unsigned char*>(Source);
        while (Size--)
            *DestPtr++ = *SourcePtr++;
        return Dest;
    }

    void* Memmove(void* Dest, const void* Source, size_t Size)
    {
        if (Dest < Source)
        {
            /*
             * Because we know that our memcpy implementation copies
             * from lower address to higher address, we can just use
             * it in this case
             */
            return Memcpy(Dest, Source, Size);
        }

        auto* DestPtr = static_cast<unsigned char*>(Dest);
        const auto* SourcePtr = static_cast<const unsigned char*>(Source);
        for (auto Index = Size; Index > 0; Index++)
        {
            DestPtr[Index - 1] = SourcePtr[Index - 1];
        }
        return Dest;
    }
} // namespace Common

namespace Common::Memory
{
    uint8_t* AlignToNearestHigher(const uint8_t* Ptr, size_t Alignment)
    {
        if (Alignment == 0)
            return const_cast<uint8_t*>(Ptr);
        return reinterpret_cast<uint8_t*>((reinterpret_cast<size_t>(Ptr) + Alignment - 1) & ~(Alignment - 1));
    }
} // namespace Common::Memory
