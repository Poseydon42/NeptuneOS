#include <gtest/gtest.h>

#include <Containers/Vector.h>

using namespace Common;

TEST(Vector, Basic)
{
    Vector<int> First;
    EXPECT_EQ(First.Size(), 0);
    EXPECT_EQ(First.Capacity(), 0);

    First.Resize(4);
    EXPECT_EQ(First.Size(), 4);
    First[0] = First[1] = 1;
    EXPECT_EQ(First[0], First[1]);
    EXPECT_EQ(First[1], 1);

    auto Second = First;
    ASSERT_EQ(First.Size(), Second.Size());
    for (int Index = 0; Index < Second.Size(); Index++)
    {
        EXPECT_EQ(First[Index], Second[Index]);
    }
    EXPECT_EQ(*First.Data(), 1);
    First.Data()[2] = 3;
    EXPECT_EQ(First[2], 3);
}
