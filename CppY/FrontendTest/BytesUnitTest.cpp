#include "pch.h"
#include "..\Base\Types\bytes.h"
#include "..\Base\Types\FunDefs.h"
#include <cstddef>
using namespace py;

TEST(bytes, hex)
{
	// Arrange
	std::vector<unsigned char> vec = { 124 , 124};
	auto b = bytes(vec);
	// Act
	std::string hexstr = MEM_FUN(b, hex));
	// Assert
	ASSERT_EQ(hexstr, "7c7c");
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
//TEST(bytes, replace)
//{
//	// Arrange
//	bytes b(R"(\xb6\x01\xef\xb6)");
//
//	// Act
//	bytes rep1 = MEM_FUN(b, replace).A(182).A(181));
//	bytes expected(R"(\xb5\x01\xef\xb5)");
//
//	bytes rep2 = MEM_FUN(b, replace).A(bytes(R"(\xb5\x01)")).A(bytes(R"(\xb3\x51)")));
//	bytes expected2(R"(\xb3\x51\xef\xb6)");
//
//	// Assert
//	ASSERT_EQ(rep1, expected);
//	ASSERT_EQ(rep2, expected2);
//}
TEST(bytes, center)
{
	// Arrange
	auto b = bytes(R"(\xb1\x01\xb6)");

	// Act
	bytes actual = MEM_FUN(b, center).A(5));
	bytes expected(R"(\x20\x20\xb1\x01\xb6\x20\x20)");
	// Assert
	ASSERT_TRUE(expected == actual);
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

//TEST(bytes, rindex)
//{
//	// Arrange
//	auto b = bytes(R"(xb6\xb1\x01\xb6)");
//
//	// Act
//	int ind = MEM_FUN(b, rindex).A(182));
//	int ind2 = MEM_FUN(b, rindex).A(bytes(R"(\xb6)")));
//
//	// Assert
//	ASSERT_EQ(ind, 3);
//	ASSERT_EQ(ind2, 3);
//}
TEST(bytes, startswith)
{
	// Arrange
	auto b = bytes(R"(\xb1\x01\xb6)");
	// Act

	bool open1 = MEM_FUN(b, startswith).A(bytes(177)));
	bool open2 = MEM_FUN(b, startswith).A(bytes(182)));
	// Assert
	ASSERT_EQ(open1, true);
	ASSERT_EQ(open2, false);
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
