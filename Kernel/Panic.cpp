#include <Panic.h>

#include <IO/Console.h>

namespace Kernel
{
    void KernelPanic(const char* Message)
    {
        Console::Out() << "KERNEL PANIC: " << Message;
        while (1);
    }
}