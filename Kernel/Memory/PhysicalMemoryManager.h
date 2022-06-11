#pragma once

#include <Arch/Memory.h>
#include <Containers/Vector.h>

namespace Kernel
{
    struct PhysicalPageRange
    {
        PhysicalAddress Base;
        PhysicalSize Size;
    };

    class PhysicalMemoryManager
    {
    public:
        static void Initialize(const Common::Vector<PhysicalPageRange>& Pages, PhysicalAddress PML4TAddress);

        static PhysicalAddress AllocatePage();
        static void FreePage(PhysicalAddress Address);

    private:
        struct PageRange
        {
            using BitmapEntryType = uint32_t;
            static constexpr uint32_t PagesPerBitmapEntry = sizeof(BitmapEntryType) * 8;

            PhysicalAddress Address;
            PhysicalSize Size;
            size_t PagesLeft;
            BitmapEntryType* BitmapPtr;
            size_t NumberOfBitmapElements;
            PageRange* Next;
        };

        static PageRange* s_FirstPageRange;
        static PhysicalAddress s_PML4TAddress;

        static bool GetPageAddressInBitmap(PhysicalAddress Address, PageRange*& Range, size_t& Entry, uint32_t& BitIndex);

        static bool IsPageUsed(PhysicalAddress Address);
        static void SetIsPageUsed(PhysicalAddress Address, bool IsUsed);
    };
} // namespace Kernel
