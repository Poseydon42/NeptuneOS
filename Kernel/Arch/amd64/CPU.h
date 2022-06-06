#pragma once

#include <stddef.h>
#include <stdint.h>

namespace Kernel
{

    class CPU
    {
    public:
        static void WriteByteToPort(uint16_t Port, uint8_t Value);

        static void WriteWordToPort(uint16_t Port, uint16_t Value);

        static void WriteDwordToPort(uint16_t Port, uint32_t Value);

        static uint8_t ReadByteFromPort(uint16_t Port);
    };
} // namespace Kernel