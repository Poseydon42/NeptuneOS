#pragma once

namespace Common
{
    template<typename T>
    struct RemoveReferenceImpl
    {
        using Type = T;
    };

    template<typename T>
    struct RemoveReferenceImpl<T&>
    {
        using Type = T;
    };

    template<typename T>
    struct RemoveReferenceImpl<T&&>
    {
        using Type = T;
    };

    template<typename T>
    using RemoveReference = typename RemoveReferenceImpl<T>::Type;
} // namespace Common