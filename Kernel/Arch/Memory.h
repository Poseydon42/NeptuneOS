#pragma once

#include <cstddef>

#ifdef NEPTUNE_AMD64
#    include <Arch/amd64/Memory.h>
#endif

namespace Kernel
{
    extern "C" void* StartOfKernelMemory;
    extern "C" void* EndOfKernelMemory;
} // namespace Kernel
