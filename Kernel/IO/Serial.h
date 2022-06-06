#pragma once

#include <stdint.h>

namespace Kernel
{
    class Serial
    {
    public:
        static Serial& The();

        void PutChar(char C);

        void Print(const char* String);

    private:
        Serial();

        [[nodiscard]] uint8_t GetLineStatus() const;

        static constexpr uint16_t BaseAddress = 0x03F8;
        static constexpr uint16_t BaudDivisorLSBRegister = BaseAddress + 0;
        static constexpr uint16_t BaudDivisorMSBRegister = BaseAddress + 1;
        static constexpr uint16_t DataRegister = BaseAddress + 0;
        static constexpr uint16_t InterruptEnableRegister = BaseAddress + 1;
        static constexpr uint16_t FIFOControlRegister = BaseAddress + 2;
        static constexpr uint16_t LineControlRegister = BaseAddress + 3;
        static constexpr uint16_t ModemControlRegister = BaseAddress + 4;
        static constexpr uint16_t LineStatusRegister = BaseAddress + 5;

        static constexpr uint32_t BaseFrequency = 115200;
        static constexpr uint32_t BaudRate = 9600;
        static constexpr uint16_t BaudDivisor = (BaseFrequency / BaudRate);
        static constexpr uint8_t BaudDivisorLow = BaudDivisor & 0x00FF;
        static constexpr uint8_t BaudDivisorHigh = (BaudDivisor >> 8) & 0x00FF;

        static constexpr uint8_t BitsPerTransferMask = 0x03;
        static constexpr uint8_t StopBitsMask = 0x00;
        static constexpr uint8_t ParityModeMask = 0x00;
        static constexpr uint8_t LineControlRegisterValue = BitsPerTransferMask | StopBitsMask | ParityModeMask;

        static constexpr uint8_t EnableFIFOMask = 0x01;
        static constexpr uint8_t ClearReceiveFIFOMask = 0x02;
        static constexpr uint8_t ClearTransmitFIFOMask = 0x04;
        static constexpr uint8_t TriggerLevel4Mask = 0xC0;
        static constexpr uint8_t FIFOControlRegisterValue =
            EnableFIFOMask | ClearReceiveFIFOMask | ClearTransmitFIFOMask | TriggerLevel4Mask;

        static constexpr uint8_t RequestToSendMask = 0x02;
        static constexpr uint8_t DataTerminalReadyMask = 0x01;
        static constexpr uint8_t ModemControlRegisterValue = RequestToSendMask | DataTerminalReadyMask;

        static constexpr uint8_t EmptyTransmitterHoldingRegister = 0x20;
    };
} // namespace Kernel
