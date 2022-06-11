#include "PhysicalMemoryManager.h"

#include <Arch/Memory.h>
#include <Assert.h>
#include <Math/Bits.h>
#include <Memory/KMalloc.h>
#include <Panic.h>
#include <Utils/Memory.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
namespace Kernel
{
    PhysicalMemoryManager::PageRange* PhysicalMemoryManager::s_FirstPageRange;
    PhysicalAddress PhysicalMemoryManager::s_PML4TAddress;

    void PhysicalMemoryManager::Initialize(const Common::Vector<PhysicalPageRange>& Pages, PhysicalAddress PML4TAddress)
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

    bool PhysicalMemoryManager::GetPageAddressInBitmap(PhysicalAddress Address, PageRange*& Range, size_t& Entry,
                                                       uint32_t& BitIndex)
    {
        auto* Current = s_FirstPageRange;
        while (Current)
        {
            if (Current->Address <= Address && Current->Address + Current->Size > Address)
            {
                auto PageOffset = (Address - Current->Address) / PAGE_SIZE;
                auto BitmapElementIndex = PageOffset / PageRange::PagesPerBitmapEntry;

                Range = Current;
                Entry = BitmapElementIndex;
                BitIndex = PageRange::PagesPerBitmapEntry - 1 - PageOffset % PageRange::PagesPerBitmapEntry;

                return true;
            }

            Current = Current->Next;
        }
        return false;
    }

    bool PhysicalMemoryManager::IsPageUsed(PhysicalAddress Address)
    {
        PageRange* Range;
        size_t BitmapEntryIndex;
        uint32_t EntryBitIndex;
        if (!GetPageAddressInBitmap(Address, Range, BitmapEntryIndex, EntryBitIndex))
            KernelPanic("Could not find required page");
        return ((Range->BitmapPtr[BitmapEntryIndex] & (1 << EntryBitIndex)) != 0);
    }

    void PhysicalMemoryManager::SetIsPageUsed(PhysicalAddress Address, bool IsUsed)
    {
        PageRange* Range;
        size_t BitmapEntryIndex;
        uint32_t EntryBitIndex;
        if (!GetPageAddressInBitmap(Address, Range, BitmapEntryIndex, EntryBitIndex))
            KernelPanic("Could not find required page");

        if (IsUsed)
            Range->BitmapPtr[BitmapEntryIndex] |= 1 << EntryBitIndex;
        else
            Range->BitmapPtr[BitmapEntryIndex] &= ~(1 << EntryBitIndex);
    }

    PhysicalAddress PhysicalMemoryManager::AllocatePage()
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

    void PhysicalMemoryManager::FreePage(PhysicalAddress Address)
    {
        ASSERT(Address % PAGE_SIZE == 0);
        ASSERT(IsPageUsed(Address));
        SetIsPageUsed(Address, false);
    }
} // namespace Kernel

#pragma clang diagnostic pop
