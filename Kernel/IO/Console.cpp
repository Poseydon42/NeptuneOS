#include "Console.h"

#include <Arch/Memory.h>
#include <IO/Serial.h>
#include <Utils/Memory.h>

#define TTY_SERIAL

namespace Kernel
{

    uint8_t Console::s_Line = 0;
    uint8_t Console::s_Column = 0;

    void Console::Print(const char* String)
    {
        while (*String)
        {
            PutChar(*String++);
        }
    }

    void Console::Initialize()
    {
        auto* Buffer = reinterpret_cast<uint16_t*>(P2V(BaseAddress));
        Common::Memset(Buffer, 0, TerminalWidth * TerminalHeight * 2);
    }

    void Console::PutChar(char Value)
    {
        auto CurrentAddress = BaseAddress + (s_Line * TerminalWidth + s_Column) * 2;
        auto VirtualAddress = P2V(CurrentAddress);
        auto Buffer = reinterpret_cast<uint8_t*>(VirtualAddress);
        if (Value != '\n')
        {
            Buffer[0] = Value;
            Buffer[1] = DefaultAttributes;
            s_Column++;
            if (s_Column == TerminalWidth)
            {
                s_Column = 0;
                s_Line++;
            }
        }
        else
        {
            s_Column = 0;
            s_Line++;
        }
        if (s_Line == TerminalHeight)
            Common::Memmove(reinterpret_cast<void*>(P2V(BaseAddress)),
                    reinterpret_cast<const void*>(P2V(BaseAddress + TerminalWidth * 2)),
                    TerminalWidth * (TerminalHeight - 1) * 2);

#ifdef TTY_SERIAL
        Serial::The().PutChar(Value);
#endif
    }

    Console::InternalOutImpl Console::Out()
    {
        return {};
    }

    Console::InternalOutImpl& Console::InternalOutImpl::operator<<(const char* String)
    {
        Console::Print(String);
        return *this;
    }

    Console::InternalOutImpl& Console::InternalOutImpl::operator<<(char Value)
    {
        Console::PutChar(Value);
        return *this;
    }

    Console::InternalOutImpl& Console::InternalOutImpl::operator<<(FormatMarkers::Binary)
    {
        m_Base = Base::Binary;
        return *this;
    }

    Console::InternalOutImpl& Console::InternalOutImpl::operator<<(FormatMarkers::Decimal)
    {
        m_Base = Base::Decimal;
        return *this;
    }

    Console::InternalOutImpl& Console::InternalOutImpl::operator<<(FormatMarkers::Hexadecimal)
    {
        m_Base = Base::Hexadecimal;
        return *this;
    }

    Console::InternalOutImpl& Console::InternalOutImpl::operator<<(FormatMarkers::NoBasePrefix)
    {
        m_PrintBasePrefix = false;
        return *this;
    }

    Console::InternalOutImpl& Console::InternalOutImpl::operator<<(FormatMarkers::BasePrefix)
    {
        m_PrintBasePrefix = true;
        return *this;
    }

} // namespace Kernel
