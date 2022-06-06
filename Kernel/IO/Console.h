#pragma once

#include <Templates/EnableIf.h>
#include <Templates/TypeTraits.h>
#include <stddef.h>
#include <stdint.h>

namespace Kernel
{
    /*
     * Marker structs for formatting console text with operator<<
     */
    namespace FormatMarkers
    {
        struct Binary
        {
        };
        struct Decimal
        {
        };
        struct Hexadecimal
        {
        };
        struct BasePrefix
        {
        };
        struct NoBasePrefix
        {
        };
    } // namespace FormatMarkers

    class Console final
    {
    public:
        static void Initialize();

        static void Print(const char* String);

        static void PutChar(char Value);

        struct InternalOutImpl
        {
        public:
            template<typename T>
            Common::EnableIf<Common::IsIntegral<T>, InternalOutImpl&> operator<<(const T& Value);

            template<typename T>
            InternalOutImpl& operator<<(const T* Ptr);

            InternalOutImpl& operator<<(const char* String);
            InternalOutImpl& operator<<(char Value);

            InternalOutImpl& operator<<(FormatMarkers::Binary);
            InternalOutImpl& operator<<(FormatMarkers::Decimal);
            InternalOutImpl& operator<<(FormatMarkers::Hexadecimal);

            InternalOutImpl& operator<<(FormatMarkers::BasePrefix);
            InternalOutImpl& operator<<(FormatMarkers::NoBasePrefix);

        private:
            enum class Base
            {
                Binary,
                Decimal,
                Hexadecimal
            } m_Base = Base::Decimal;

            bool m_PrintBasePrefix = true;

            template<typename T>
            void PrintDecimalNumber(T Value);

            template<typename T>
            void PrintBinaryNumber(T Value);

            template<typename T>
            void PrintHexadecimalNumber(T Value);
        };

        static FormatMarkers::Binary Binary;
        static FormatMarkers::Decimal Decimal;
        static FormatMarkers::Hexadecimal Hexadecimal;

        static InternalOutImpl Out();

    private:
        static uint8_t s_Line;
        static uint8_t s_Column;

        static constexpr size_t BaseAddress = 0xB8000;
        static constexpr uint8_t TerminalWidth = 80;
        static constexpr uint8_t TerminalHeight = 24;
        static constexpr uint8_t DefaultAttributes = 0x07;
    };

    template<typename T>
    Common::EnableIf<Common::IsIntegral<T>, Console::InternalOutImpl&> Console::InternalOutImpl::operator<<(const T& Value)
    {
        switch (m_Base)
        {
        case Base::Decimal:
        default:
            PrintDecimalNumber(Value);
            break;
        case Base::Hexadecimal:
            PrintHexadecimalNumber(Value);
            break;
        case Base::Binary:
            PrintBinaryNumber(Value);
            break;
        }
        return *this;
    }

    template<typename T>
    void Console::InternalOutImpl::PrintDecimalNumber(T Value)
    {
        if (Value == 0)
        {
            Console::PutChar('0');
            return;
        }

        auto V = Value;
        if (V < 0)
            V = -V;

        uint32_t Pow = 1;
        uint32_t NumDigits = 0;
        while (V > 0)
        {
            V /= 10;
            Pow *= 10;
            NumDigits++;
        }

        if (Value < 0)
            Console::PutChar('-');
        V = Value;
        if (V < 0)
            V = -V;
        for (uint32_t DigitCounter = 0; DigitCounter < NumDigits; DigitCounter++)
        {
            auto Digit = V % Pow / (Pow / 10);
            Console::PutChar(static_cast<char>(Digit) + '0');
            Pow /= 10;
        }
    }

    template<typename T>
    void Console::InternalOutImpl::PrintHexadecimalNumber(T Value)
    {
        if (m_PrintBasePrefix)
            Console::Print("0x");
        char LookupTable[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
        uint32_t NumNibbles = sizeof(Value) * 2;
        bool FoundNonZeroNibble = false;
        for (int32_t Nibble = NumNibbles - 1; Nibble >= 0; Nibble--)
        {
            auto NibbleValue = (Value >> Nibble * 4) & 0x0F;
            if (NibbleValue == 0 && !FoundNonZeroNibble && Nibble != 0)
                continue;
            FoundNonZeroNibble = true;
            Console::PutChar(LookupTable[NibbleValue]);
        }
    }

    template<typename T>
    void Console::InternalOutImpl::PrintBinaryNumber(T Value)
    {
        if (m_PrintBasePrefix)
            Console::Print("0b");
        uint32_t NumBits = sizeof(Value) * 8;
        bool FoundNonZeroBit = false;
        for (int32_t Bit = NumBits - 1; Bit >= 0; Bit--)
        {
            auto BitValue = (Value >> Bit) & 1;
            if (BitValue == 0 && !FoundNonZeroBit && Bit != 0)
                continue;
            FoundNonZeroBit = true;
            Console::PutChar(BitValue + '0');
        }
    }

    template<typename T>
    Console::InternalOutImpl& Console::InternalOutImpl::operator<<(const T* Ptr)
    {
        PrintHexadecimalNumber(reinterpret_cast<size_t>(Ptr));
        return *this;
    }

} // namespace Kernel
