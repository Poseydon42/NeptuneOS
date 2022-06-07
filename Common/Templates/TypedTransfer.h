#pragma once

#include <cstddef>

namespace Common
{
    /*
     * Function for proper copying of typed data
     */
    template<typename T>
    struct TypedTransfer
    {
        /*
         * Copies Count elements from Source to Destination
         * Ranges should not overlap
         * Uses operator=(const T&) to copy elements
         */
        static void Copy(T* Dest, const T* Source, size_t Count);

        /*
         * Copies Count elements from Source to Destination
         * Ranges may overlap
         * Uses operator=(const T&) to copy elements
         */
        static void Move(T* Dest, const T* Source, size_t Count);
    };

    template<typename T>
    void TypedTransfer<T>::Copy(T* Dest, const T* Source, size_t Count)
    {
        while (Count--)
            *Dest++ = *Source++;
    }

    template<typename T>
    void TypedTransfer<T>::Move(T* Dest, const T* Source, size_t Count)
    {
        if (Source > Dest)
        {
            TypedTransfer<T>::Copy(Dest, Source, Count);
        }
        else
        {
            for (size_t Index = 0; Index < Count; Index++)
                Dest[Count - Index - 1] = Source[Count - Index - 1];
        }
    }
} // namespace Common
