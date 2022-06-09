#include <Panic.h>

#include <IO/Console.h>

namespace Kernel
{
    [[noreturn]] void KernelPanic(const char* Message)
    {
        Console::Out() << "KERNEL PANIC: " << Message;
        while (true);
    }
}