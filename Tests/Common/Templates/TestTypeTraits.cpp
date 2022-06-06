#include <gtest/gtest.h>

#include <Templates/TypeTraits.h>

TEST(Templates, IsIntegral)
{
    EXPECT_TRUE(Common::IsIntegral<int>);
    EXPECT_TRUE(Common::IsIntegral<uint64_t>);
    EXPECT_TRUE(Common::IsIntegral<const volatile short>);
    EXPECT_TRUE(Common::IsIntegral<bool>);

    EXPECT_FALSE(Common::IsIntegral<const char*>);
    EXPECT_FALSE(Common::IsIntegral<float>);
}
