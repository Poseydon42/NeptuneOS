#include "KMalloc.h"

#include <Utils/Memory.h>

// NOTE : currently, total kernel size must not be greater than
//        1 MiB because this is how much memory we map at boot
#define INITIAL_KMALLOC_POOL_SIZE 512 * 1024 // 512 KiB

namespace Kernel
{
    struct AllocationHeader
    {
        size_t Size;
        size_t OriginalFreeBlockAddress;
    };

    struct FreeBlockHeader
    {
        FreeBlockHeader* Next;
        size_t Size;
    };

    alignas(4096) uint8_t InitialKMallocPool[INITIAL_KMALLOC_POOL_SIZE];
    FreeBlockHeader* FirstFreeBlock;

    void KMallocInitialize()
    {
        Common::Memset(InitialKMallocPool, 0, sizeof(InitialKMallocPool));
        auto* FreeBlock = reinterpret_cast<FreeBlockHeader*>(InitialKMallocPool);
        FreeBlock->Size = INITIAL_KMALLOC_POOL_SIZE;
        FreeBlock->Next = nullptr;

        FirstFreeBlock = FreeBlock;
    }

    void* KMalloc(size_t Size, size_t Alignment)
    {
        if (Size == 0)
            return nullptr;

        if (Alignment < alignof(AllocationHeader))
            Alignment = alignof(AllocationHeader);

        FreeBlockHeader* PreviousFreeBlock = nullptr;
        auto* FreeBlock = FirstFreeBlock;
        while (FreeBlock)
        {
            auto* BlockStart = reinterpret_cast<uint8_t*>(FreeBlock);
            auto* PossibleAllocationStart = BlockStart + sizeof(AllocationHeader);
            auto* AlignedAllocationStart = Common::Memory::AlignToNearestHigher(PossibleAllocationStart, Alignment);
            size_t TotalSizeOfPossibleAllocation = AlignedAllocationStart - BlockStart + Size;
            if (FreeBlock->Size >= TotalSizeOfPossibleAllocation)
            {
                if (FreeBlock->Size - TotalSizeOfPossibleAllocation >= sizeof(FreeBlockHeader))
                {
                    auto* NewFreeBlockHeader = reinterpret_cast<FreeBlockHeader*>(
                        reinterpret_cast<uint8_t*>(FreeBlock) + TotalSizeOfPossibleAllocation);
                    NewFreeBlockHeader->Size = FreeBlock->Size - TotalSizeOfPossibleAllocation;
                    NewFreeBlockHeader->Next = FreeBlock->Next;

                    if (PreviousFreeBlock)
                        PreviousFreeBlock->Next = NewFreeBlockHeader;
                    else
                        FirstFreeBlock = NewFreeBlockHeader;

                    auto* Allocation = reinterpret_cast<AllocationHeader*>(AlignedAllocationStart) - 1;
                    Allocation->Size = TotalSizeOfPossibleAllocation;
                    Allocation->OriginalFreeBlockAddress = reinterpret_cast<size_t>(BlockStart);

                    uint8_t* Data = reinterpret_cast<uint8_t*>(Allocation) + sizeof(AllocationHeader);
                    return Data;
                }
                else
                {
                    if (PreviousFreeBlock)
                        PreviousFreeBlock->Next = FreeBlock->Next;
                    else
                        FirstFreeBlock = FreeBlock->Next;

                    auto* Allocation = reinterpret_cast<AllocationHeader*>(AlignedAllocationStart) - 1;
                    Allocation->Size = FreeBlock->Size;
                    Allocation->OriginalFreeBlockAddress = reinterpret_cast<size_t>(BlockStart);

                    auto* Data = reinterpret_cast<uint8_t*>(Allocation + 1);
                    return Data;
                }
            }

            PreviousFreeBlock = FreeBlock;
            FreeBlock = FreeBlock->Next;
        }

        return nullptr;
    }

    void KFree(void* Ptr)
    {
        if (!Ptr)
            return;

        AllocationHeader* Allocation = reinterpret_cast<AllocationHeader*>(Ptr) - 1;
        size_t TotalSize = Allocation->Size;
        auto* NewFreeBlock = reinterpret_cast<FreeBlockHeader*>(Allocation->OriginalFreeBlockAddress);
        NewFreeBlock->Size = TotalSize;

        FreeBlockHeader* PreviousFreeBlock = nullptr;
        auto* FreeBlock = FirstFreeBlock;
        while (FreeBlock)
        {
            if (PreviousFreeBlock < NewFreeBlock && FreeBlock > NewFreeBlock)
            {
                // Left merge
                if (PreviousFreeBlock && reinterpret_cast<uint8_t*>(PreviousFreeBlock) + PreviousFreeBlock->Size ==
                                             reinterpret_cast<uint8_t*>(NewFreeBlock))
                {
                    PreviousFreeBlock->Size += TotalSize;
                    NewFreeBlock = PreviousFreeBlock;
                }
                else
                {
                    if (PreviousFreeBlock)
                        PreviousFreeBlock->Next = NewFreeBlock;
                    else
                        FirstFreeBlock = NewFreeBlock;
                }

                // Right merge
                if (reinterpret_cast<uint8_t*>(NewFreeBlock) + NewFreeBlock->Size ==
                    reinterpret_cast<uint8_t*>(FreeBlock))
                {
                    NewFreeBlock->Size += FreeBlock->Size;
                    NewFreeBlock->Next = FreeBlock->Next;
                }
                else
                {
                    NewFreeBlock->Next = FreeBlock;
                }

                return;
            }

            PreviousFreeBlock = FreeBlock;
            FreeBlock = FreeBlock->Next;
        }
    }
} // namespace Kernel

void* operator new(size_t Size)
{
    return Kernel::KMalloc(Size, __STDCPP_DEFAULT_NEW_ALIGNMENT__);
}

void* operator new[](size_t Size)
{
    return Kernel::KMalloc(Size, __STDCPP_DEFAULT_NEW_ALIGNMENT__);
}

void* operator new(size_t Size, size_t Alignment)
{
    return Kernel::KMalloc(Size, Alignment);
}

void* operator new[](size_t Size, size_t Alignment)
{
    return Kernel::KMalloc(Size, Alignment);
}

void operator delete(void* Ptr)
{
    Kernel::KFree(Ptr);
}

void operator delete[](void* Ptr)
{
    Kernel::KFree(Ptr);
}

void operator delete(void* Ptr, size_t)
{
    Kernel::KFree(Ptr);
}

void operator delete[](void* Ptr, size_t)
{
    Kernel::KFree(Ptr);
}
