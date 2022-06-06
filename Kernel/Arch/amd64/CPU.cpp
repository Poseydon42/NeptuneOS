#include "CPU.h"

#ifdef SERENITY_AMD64

extern "C" void WriteByteToPortImpl(uint16_t Port, uint8_t Value);
extern "C" void WriteWordToPortImpl(uint16_t Port, uint16_t Value);
extern "C" void WriteDwordToPortImpl(uint16_t Port, uint32_t Value);

extern "C" uint8_t ReadByteFromPortImpl(uint16_t Port);

namespace Kernel
{
    void CPU::WriteByteToPort(uint16_t Port, uint8_t Value)
    {
        WriteByteToPortImpl(Port, Value);
    }

    void CPU::WriteWordToPort(uint16_t Port, uint16_t Value)
    {
        WriteWordToPortImpl(Port, Value);
    }

    void CPU::WriteDwordToPort(uint16_t Port, uint32_t Value)
    {
        WriteDwordToPortImpl(Port, Value);
    }

    uint8_t CPU::ReadByteFromPort(uint16_t Port)
    {
        return ReadByteFromPortImpl(Port);
    }
}

#endif