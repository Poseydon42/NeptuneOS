#include <gtest/gtest.h>

#include <Math/Bits.h>

TEST(Bits, FindMostSignificantSetBit)
{
    EXPECT_EQ(Common::FindMostSignificantSetBit(1), 0);
    EXPECT_EQ(Common::FindMostSignificantSetBit(3), 1);
    EXPECT_EQ(Common::FindMostSignificantSetBit(0x80000000), 31);
    EXPECT_EQ(Common::FindMostSignificantSetBit(0x40000001), 30);
}

TEST(Bits, FindMostSignificantClearBit)
{
    EXPECT_EQ(Common::FindMostSignificantClearBit(0xFFFFFFFE), 0);
    EXPECT_EQ(Common::FindMostSignificantClearBit(0xFFFFFFFD), 1);
    EXPECT_EQ(Common::FindMostSignificantClearBit(0xFFFFFFFC), 1);
    EXPECT_EQ(Common::FindMostSignificantClearBit(0x80000000), 30);
    EXPECT_EQ(Common::FindMostSignificantClearBit(0x4FFFFFFF), 31);
}

