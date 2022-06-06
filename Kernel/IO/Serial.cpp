#include "Serial.h"

#include <Arch/CPU.h>

#define HACKY_SERIAL

namespace Kernel
{

    Serial& Serial::The()
    {
        static Serial Instance;
        return Instance;
    }

    void Serial::PutChar(char C)
    {
        // FIXME: find out why this turns into an endless loop
#ifndef HACKY_SERIAL
        while ((GetLineStatus() & EmptyTransmitterHoldingRegister) == 0)
            ;
#endif
        CPU::WriteByteToPort(DataRegister, *reinterpret_cast<uint8_t*>(&C));
    }

    void Serial::Print(const char* String)
    {
        while (*String)
            PutChar(*String++);
    }

    Serial::Serial()
    {
        /* Disable all interrupts */
        CPU::WriteByteToPort(InterruptEnableRegister, 0x00);

        /* Set baud rate */
        CPU::WriteByteToPort(LineControlRegister, 0x80); // Enable DLAB bit
        CPU::WriteByteToPort(BaudDivisorLSBRegister, BaudDivisorLow);
        CPU::WriteByteToPort(BaudDivisorMSBRegister, BaudDivisorHigh);

        /* Set line control register */
        CPU::WriteByteToPort(LineControlRegister, LineControlRegisterValue);

        /* Set FIFO control register value */
        CPU::WriteByteToPort(FIFOControlRegister, FIFOControlRegisterValue);

        /* Set modem control register value */
        CPU::WriteByteToPort(ModemControlRegister, 0x0F);
    }

    uint8_t Serial::GetLineStatus() const
    {
        return CPU::ReadByteFromPort(LineStatusRegister);
    }

} // namespace Kernel
