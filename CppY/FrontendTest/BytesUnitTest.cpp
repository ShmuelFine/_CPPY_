#include "pch.h"
#include "..\Base\Types\bytes.h"
#include "..\Base\Types\FunDefs.h"
#include <cstddef>
using namespace py;

TEST(bytes, hex)
{
	// Arrange
	std::vector<unsigned char> vec = { 124 };
	auto b = bytes(vec);
	// Act
	std::string hexstr = MEM_FUN(b, hex));
	// Assert
	ASSERT_TRUE(hexstr.compare("7c") == 0);
}
TEST(bytes, count)
{
	// Arrange
	bytes b(R"(\xb6\x01\xef)");

	// Act
	int counter = MEM_FUN(b, count).A(182));
	int counter1 = MEM_FUN(b, count).A(99));

	// Assert
	ASSERT_EQ(counter, 1);
	ASSERT_EQ(counter1, 0);
}
TEST(bytes, center)
{
	// Arrange
	auto b = bytes(R"(\xb1\x01\xb6)");

	// Act
	bytes s = MEM_FUN(b, center).A(5));
	bytes res(R"(\x20\x20\xb1\x01\xb6\x20\x20)");
	// Assert
	ASSERT_TRUE(s==res);
}
TEST(bytes, endswith)
{
	// Arrange
	auto b = bytes(R"(\xb1\x01\xb6)");
	auto b1 = bytes(R"(\xb1\x01\xb1\xb6)");

	// Act
	auto ending1 = bytes(R"(\xb6)");
	bool end1 = MEM_FUN(b, endswith).A(ending1));
	auto ending2 = bytes(R"(\xb5)");
	bool end2 = MEM_FUN(b, endswith).A(ending2));

	auto ending3 = bytes(R"(\xb1\xb6)");
	bool end3= MEM_FUN(b1, endswith).A(ending3));

	// Assert
	ASSERT_EQ(end1, true);
	ASSERT_EQ(end2, false);
	ASSERT_EQ(end3, true);
}

TEST(bytes, index)
{
	// Arrange
	auto b = bytes(R"(\xb1\x01\xb6)");

	// Act
	int ind = MEM_FUN(b, index).A(182));

	// Assert
	ASSERT_EQ(ind, 2);
}
TEST(bytes, startswith)
{
	// Arrange
	auto b = bytes(R"(\xb1\x01\xb6)");
	// Act

	bool open1 = MEM_FUN(b, startswith).A(117));
	bool open2 = MEM_FUN(b, startswith).A(182));
	// Assert
	ASSERT_EQ(open1, true);
	ASSERT_EQ(open2, false);
}

TEST(bytes, replace)
{
	// Arrange
	auto b = bytes(R"(\xb1\x01\xb6)");
	// Act
	bytes newb = MEM_FUN(b, replace).A(R"(\xb6)").A(R"(\xb5)"));
	bytes newb2 = MEM_FUN(b, replace).A(R"(\x01\xb6)").A(R"(\xb5)"));

	// Assert
	ASSERT_EQ(newb, bytes(R"(\xb1\x01\xb5)"));
	ASSERT_EQ(newb2, bytes(R"(\xb1\xb5)"));
}

TEST(bytes, stringOperator)
{
	// Arrange
	std::string origStr = R"(\xb1\x01\xb6)";
	auto b = bytes(origStr);
	
	// Act
	std::string b_str = (std::string)b;
	// Assert
	ASSERT_EQ(b_str, origStr);
}
