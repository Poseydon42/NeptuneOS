#pragma once

#include <Templates/Move.h>
#include <Templates/Swap.h>
#include <Templates/TypedTransfer.h>
#include <cstddef>
#include <initializer_list>
#include <new>

namespace Common
{
    template<typename T>
    class Vector
    {
    public:
        Vector() = default;
        explicit Vector(size_t InitialSize, T InitialValue = T {});

        Vector(const Vector& Other);
        Vector& operator=(const Vector& Other);

        Vector(Vector&& Other);
        Vector& operator=(Vector&& Other);

        ~Vector();

        const T& operator[](size_t Index) const;
        T& operator[](size_t Index);

        const T* Data() const;
        T* Data();

        void Resize(size_t Size, T Element = T {});
        void Reserve(size_t NewCapacity);

        [[nodiscard]] size_t Size() const;
        [[nodiscard]] size_t Capacity() const;

        void PushBack(const T& Element);
        void PushBack(T&& Element);

    private:
        T* m_Data = nullptr;
        size_t m_Size = 0;
        size_t m_Capacity = 0;
    };

    template<typename T>
    Vector<T>::Vector(size_t InitialSize, T InitialValue)
        : m_Data(new T[InitialSize])
        , m_Size(InitialSize)
        , m_Capacity(InitialSize)
    {
        for (size_t Index = 0; Index < m_Size; Index++)
            m_Data[Index] = InitialValue;
    }

    template<typename T>
    Vector<T>::Vector(const Vector& Other)
    {
        *this = Other;
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(const Vector& Other)
    {
        if (this == &Other)
            return *this;

        m_Data = new T[Other.m_Capacity];
        TypedTransfer<T>::Copy(m_Data, Other.m_Data, Other.m_Size);
        m_Size = Other.m_Size;
        m_Capacity = Other.m_Capacity;

        return *this;
    }

    template<typename T>
    Vector<T>::Vector(Vector&& Other)
    {
        *this = Move(Other);
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(Vector&& Other)
    {
        if (this == &Other)
            return *this;

        delete[] m_Data;
        m_Data = nullptr;

        Swap(m_Data, Other.m_Data);
        Swap(m_Size, Other.m_Size);
        Swap(m_Capacity, Other.m_Capacity);

        return *this;
    }

    template<typename T>
    Vector<T>::~Vector()
    {
        delete[] m_Data;
    }

    template<typename T>
    const T& Vector<T>::operator[](size_t Index) const
    {
        return m_Data[Index];
    }

    template<typename T>
    T& Vector<T>::operator[](size_t Index)
    {
        return m_Data[Index];
    }

    template<typename T>
    const T* Vector<T>::Data() const
    {
        return m_Data;
    }

    template<typename T>
    T* Vector<T>::Data()
    {
        return m_Data;
    }

    template<typename T>
    void Vector<T>::Resize(size_t Size, T Element)
    {
        if (Size == m_Size)
            return;

        if (Size > m_Size)
        {
            if (Size > m_Capacity)
                Reserve(Size);

            for (size_t Index = m_Size; Index < Size; Index++)
                new (m_Data + Index) T(Element);
        }
        else
        {
            for (size_t Index = Size; Index < m_Size; Index++)
            {
                m_Data[Index].~T();
            }
        }

        m_Size = Size;
    }

    template<typename T>
    void Vector<T>::Reserve(size_t NewCapacity)
    {
        if (NewCapacity <= m_Capacity)
            return;

        auto* NewStorage = new T[NewCapacity];
        TypedTransfer<T>::Copy(NewStorage, m_Data, m_Size);

        delete[] m_Data;
        m_Data = NewStorage;
        m_Capacity = NewCapacity;
    }

    template<typename T>
    size_t Vector<T>::Size() const
    {
        return m_Size;
    }

    template<typename T>
    size_t Vector<T>::Capacity() const
    {
        return m_Capacity;
    }

    template<typename T>
    void Vector<T>::PushBack(const T& Element)
    {
        Reserve(m_Size + 1);
        m_Data[m_Size] = Element;
        m_Size++;
    }

    template<typename T>
    void Vector<T>::PushBack(T&& Element)
    {
        Reserve(m_Size + 1);
        m_Data[m_Size] = Move(Element);
        m_Size++;
    }
} // namespace Common
