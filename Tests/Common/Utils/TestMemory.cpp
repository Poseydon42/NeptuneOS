#include <gtest/gtest.h>
#include <Utils/Memory.h>

TEST(Memory, AlignToNearestHigher)
{
	const uint8_t* Ptr = (const uint8_t*)0x42;
	EXPECT_EQ(Common::Memory::AlignToNearestHigher(Ptr, 4), (const uint8_t*)0x44);
	EXPECT_EQ(Common::Memory::AlignToNearestHigher(Ptr, 8), (const uint8_t*)0x48);
	EXPECT_EQ(Common::Memory::AlignToNearestHigher(Ptr, 0), Ptr);
	EXPECT_EQ(Common::Memory::AlignToNearestHigher(Ptr, 2), Ptr);
}

