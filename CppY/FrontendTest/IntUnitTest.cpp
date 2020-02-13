#include "pch.h"
#include "..\Base\Types\Int.h"
#include "..\Base\Types\FunDefs.h"

using namespace py;

TEST(Int, bit_length_Sanity)
{
	// Arrange
	std::vector<Int> x			= { -37, 0, 2, 5 };
	std::vector<int> expected	= { 6, 0, 2, 3 };
	
	std::vector<object> bin_lengths;

	// Act
	for (int i = 0; i < x.size();i++)
		bin_lengths.push_back(MEM_FUN(x[i], bit_length)));

	// Assert
	for (int i = 0; i < x.size(); i++)
		ASSERT_TRUE(bin_lengths[i] == expected[i]);
}

TEST(Int, to_bytes_CreatesNonModifyableCollection)
{
	// Arrange
	Int x = -37;

	auto bytes = MEM_FUN(x, to_bytes).A(5));

	// Act, Assert
	EXPECT_THROW(bytes[0] = 23, std::exception);
}