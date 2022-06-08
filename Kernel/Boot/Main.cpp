#include <Arch/Memory.h>
#include <Boot/Multiboot.h>
#include <Containers/Vector.h>
#include <IO/Console.h>
#include <Memory/KMalloc.h>
#include <Memory/PhysicalPageManager.h>
#include <Panic.h>

using namespace Kernel;

extern "C" [[maybe_unused]] void KernelMain(const void* MultibootInfoPtr, uint32_t MultibootMagic, void* PML4T)
{
    Console::Initialize();
    Console::Out() << "Kernel loaded... Printing from KernelMain()\n";

    KMallocInitialize();

    auto MultibootInfo = reinterpret_cast<multiboot_info_t*>(P2V(MultibootInfoPtr));
    if (MultibootMagic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        KernelPanic("Invalid multiboot bootloader magic number");
    }
    if (!(MultibootInfo->flags & MULTIBOOT_INFO_MEM_MAP))
    {
        KernelPanic("No valid memory map given by bootloader");
    }

    Common::Vector<PhysicalPageRange> PageRanges;
    for (const auto* Entry = reinterpret_cast<const multiboot_mmap_entry*>(P2V(MultibootInfo->mmap_addr));
         reinterpret_cast<size_t>(Entry) < P2V(MultibootInfo->mmap_addr) + MultibootInfo->mmap_length;
         Entry = reinterpret_cast<const multiboot_mmap_entry*>(reinterpret_cast<size_t>(Entry) + Entry->size +
                                                               sizeof(Entry->size)))
    {
        Console::Out() << "Address: " << Console::Hexadecimal << Entry->addr << " length: " << Entry->len
                       << " type: " << Entry->type << '\n';
        if (Entry->type == MULTIBOOT_MEMORY_AVAILABLE)
            PageRanges.PushBack({Entry->addr, Entry->len});
    }

    Console::Out() << "PML4T physical address: " << PML4T << '\n';
    PhysicalPageManager::Initialize(PageRanges, reinterpret_cast<PhysicalAddress>(PML4T));
}
