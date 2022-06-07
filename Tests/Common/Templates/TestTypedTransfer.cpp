#include <gtest/gtest.h>

#include <Templates/TypedTransfer.h>

using namespace Common;

TEST(TypedTransfer, NonOverlapping)
{
    const int Source[5] = { 1, 3, 9, 7, 42 };
    int Dest[5];

    TypedTransfer<int>::Copy(Dest, Source, 5);
    for (size_t Index = 0; Index < 5; Index++)
        EXPECT_EQ(Source[Index], Dest[Index]);

    memset(Dest, 0, sizeof(Dest));

    TypedTransfer<int>::Move(Dest, Source, 5);
    for (size_t Index = 0; Index < 5; Index++)
        EXPECT_EQ(Source[Index], Dest[Index]);
}

TEST(TypedTransfer, Overlapping)
{
    int Source[5] = { 1, 3, 9, 7, 42 };
    const int Expected[5] = { 1, 3, 1, 3, 42 };

    TypedTransfer<int>::Move(Source + 2, Source, 2);
    for (size_t Index = 0; Index < 5; Index++)
        EXPECT_EQ(Source[Index], Expected[Index]);
}
