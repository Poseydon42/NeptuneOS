#include "PhysicalPageManager.h"

#include <Arch/Memory.h>
#include <Math/Bits.h>
#include <Memory/KMalloc.h>
#include <Panic.h>
#include <Utils/Memory.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
namespace Kernel
{
    PhysicalPageManager::PageRange* PhysicalPageManager::s_FirstPageRange;
    PhysicalAddress PhysicalPageManager::s_PML4TAddress;

    void PhysicalPageManager::Initialize(const Common::Vector<PhysicalPageRange>& Pages, PhysicalAddress PML4TAddress)
    {
        s_PML4TAddress = PML4TAddress;
        s_FirstPageRange = new PageRange[Pages.Size()];

        PageRange* Current = s_FirstPageRange;
        for (size_t Index = 0; Index < Pages.Size(); Index++)
        {
            Current->Address = Pages[Index].Base;
            // NOTE : we do this to round page range size to a multiple of PAGE_SIZE
            Current->Size = Pages[Index].Size / PAGE_SIZE * PAGE_SIZE;

            size_t NumberOfPages = Current->Size / PAGE_SIZE;
            size_t NumberOfBytesInBitmap = NumberOfPages / 8;
            NumberOfBytesInBitmap =
                (NumberOfBytesInBitmap + 3) / sizeof(PageRange::BitmapEntryType) * sizeof(PageRange::BitmapEntryType);

            Current->PagesLeft = NumberOfPages;
            Current->NumberOfBitmapElements = NumberOfBytesInBitmap / sizeof(PageRange::BitmapEntryType);
            Current->BitmapPtr = static_cast<PageRange::BitmapEntryType*>(
                KMalloc(NumberOfBytesInBitmap, alignof(PageRange::BitmapEntryType)));
            Common::Memset(Current->BitmapPtr, 0, NumberOfBytesInBitmap);

            if (Index >= Pages.Size() - 1)
                Current->Next = nullptr;
            else
                Current->Next = Current + 1;
            Current++;
        }

        // NOTE : mark page at address 0x00 as used so that we never return its address
        if (s_FirstPageRange->Address == 0)
            SetIsPageUsed(0, true);
        for (PhysicalAddress KernelPageAddress = V2P(&StartOfKernelMemory); KernelPageAddress < V2P(&EndOfKernelMemory);
             KernelPageAddress += PAGE_SIZE)
            SetIsPageUsed(KernelPageAddress, true);
    }

    bool PhysicalPageManager::IsPageUsed(PhysicalAddress Address)
    {
        PageRange* Current = s_FirstPageRange;
        while (Current)
        {
            if (Current->Address <= Address && Current->Address + Current->Size >= Address)
            {
                const auto BitsPerSingleBitmapElement = sizeof(PageRange::BitmapEntryType) * 8;

                auto PageOffset = (Address - Current->Address) / PAGE_SIZE;
                auto BitmapElementIndex = PageOffset / BitsPerSingleBitmapElement;
                auto BitmapBitIndex = BitsPerSingleBitmapElement - 1 - PageOffset % BitsPerSingleBitmapElement;

                return ((Current->BitmapPtr[BitmapElementIndex] & (1 << BitmapBitIndex)) != 0);
            }
            Current = Current->Next;
        }
        KernelPanic("Invalid page address");
        return false;
    }

    void PhysicalPageManager::SetIsPageUsed(PhysicalAddress Address, bool IsUsed)
    {
        PageRange* Current = s_FirstPageRange;
        while (Current)
        {
            if (Current->Address <= Address && Current->Address + Current->Size >= Address)
            {
                const auto BitsPerSingleBitmapElement = sizeof(PageRange::BitmapEntryType) * 8;

                auto PageOffset = (Address - Current->Address) / PAGE_SIZE;
                auto BitmapElementIndex = PageOffset / BitsPerSingleBitmapElement;
                auto BitmapBitIndex = BitsPerSingleBitmapElement - 1 - PageOffset % BitsPerSingleBitmapElement;

                if (IsUsed)
                    Current->BitmapPtr[BitmapElementIndex] |= 1 << BitmapBitIndex;
                else
                    Current->BitmapPtr[BitmapElementIndex] &= ~(1 << BitmapBitIndex);

                Current->PagesLeft--;
                return;
            }
            Current = Current->Next;
        }
        KernelPanic("Invalid page address");
    }

    PhysicalAddress PhysicalPageManager::AllocatePage()
    {
        auto* Current = s_FirstPageRange;
        while (Current)
        {
            if (Current->PagesLeft == 0)
            {
                Current = Current->Next;
                continue;
            }

            for (size_t Index = 0; Index < Current->NumberOfBitmapElements; Index++)
            {
                if (~Current->BitmapPtr[Index] == 0)
                    continue;
                auto BitIndex = Common::FindMostSignificantClearBit(Current->BitmapPtr[Index]);
                PhysicalAddress PageAddress = Current->Address + (Index * PageRange::PagesPerBitmapEntry +
                                                                  PageRange::PagesPerBitmapEntry - BitIndex - 1) *
                                                                     PAGE_SIZE;
                SetIsPageUsed(PageAddress, true);
                return PageAddress;
            }

            Current = Current->Next;
        }
        return 0;
    }
} // namespace Kernel

#pragma clang diagnostic pop
