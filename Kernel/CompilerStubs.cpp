#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-reserved-identifier"

extern "C"
{
    [[maybe_unused]] void __cxa_guard_acquire(void)
    {
    }

    [[maybe_unused]] void __cxa_guard_release(void)
    {
    }
}
#pragma clang diagnostic pop