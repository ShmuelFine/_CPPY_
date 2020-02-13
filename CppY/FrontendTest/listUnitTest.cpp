#include "pch.h"
#include "..\Base\Types\TypeSpecificConstruction.h"
#include "..\Base\Types\list.h"
#include "..\Base\Types\Built_In_Functions.h"


using namespace py;

TEST(list, Initialization_SanityTest)
{
	// Arrange, Act
	auto l = list({1, 2., "hello"});

	// Assert
	ASSERT_TRUE(l[0] == 1);
	ASSERT_TRUE(l[1] == 2.);
	ASSERT_TRUE(l[2] == "hello");
}

TEST(list, Initialization_AddsMemberFunctionsAsAttributes)
{
	// Arrange
	auto l = list({ 1, 2., "hello", 1 });

	// Act, Assert
	l.hasattr("count");
	l.hasattr("extend");
}

TEST(list, count_whenElementPresent_ThenReturnsAppearancesAmount)
{
	// Arrange
	auto l = list({ 1, 2., "hello", 1 });

	// Act
	int count_1 = MEM_FUN(l, count).A(1));
	int count_hello = MEM_FUN(l, count).A("hello"));
	
	// Assert
	ASSERT_EQ(count_1, 2);
	ASSERT_EQ(count_hello, 1);
}

TEST(list, count_whenElement_NOT_Present_ThenReturnsZero)
{
	// Arrange
	auto l = list({ 1, 2., "hello", 1 });

	// Act
	int count_3 = MEM_FUN(l, count).A(3));

	// Assert
	ASSERT_EQ(count_3, 0);
}


TEST(list, extend_WhenByNonEmptyList_Sanity)
{
	// Arrange
	auto l = list({ 1, 2., "hello", 1 });
	auto toAdd = list({ "A","B","C" });
	
	// Act
	MEM_FUN(l, extend).A(toAdd));

	int count_A = MEM_FUN(l, count).A("A"));
	int count_B = MEM_FUN(l, count).A("B"));
	int count_C = MEM_FUN(l, count).A("C"));
	int outSize = 0;
	
	for (auto elem : l)
		outSize++;
	
	// Assert
	ASSERT_EQ(outSize, 7);
	ASSERT_EQ(count_A, 1);
	ASSERT_EQ(count_B, 1);
	ASSERT_EQ(count_C, 1);
}

TEST(list, append_Sanity)
{
	// Arrange
	auto l = list({ 1, 2., "hello", 1 });
	object toAdd = "A";

	// Act
	MEM_FUN(l, append).A(toAdd));

	int count_A = MEM_FUN(l, count).A("A"));
	int outSize = 0;

	for (auto elem : l)
		outSize++;

	// Assert
	ASSERT_EQ(outSize, 5);
	ASSERT_EQ(count_A, 1);
}

TEST(list, clear_Sanity)
{
	// Arrange
	auto l = list({ 1, 2., "hello", 1 });

	// Act
	MEM_FUN(l, clear));

	int outSize = 0;

	for (auto elem : l)
		outSize++;

	// Assert
	ASSERT_EQ(outSize, 0);
}

TEST(list, sort_Sanity)
{
	// Arrange
	auto l = list({ 10, 2, 5, 8, 1, 10 });
	auto expected = list({ 1, 2, 5, 8, 10, 10 });
	// Act
	MEM_FUN(l, sort));

	for (int i = 0; i < 6; i++)
		ASSERT_EQ(l[i], expected[i]);
}

TEST(list, pop_Sanity)
{
	// Arrange
	auto l = list({ 10, 2, 5, 8, 1, 10 });
	auto expectedList = list({ 10, 5, 8, 1, 10 });
	auto expected = 2;

	// Act
	auto actual = MEM_FUN(l, pop).A(1));
	
	// Assert
	for (int i = 0; i < 5; i++)
		ASSERT_EQ(l[i], expectedList[i]);
	ASSERT_EQ(actual, expected);
}

TEST(list, insert_Sanity)
{
	// Arrange
	auto l = list({ 10, 2, 5, 8, 1, 10 });
	auto expected = list({ 10, 2, 5, 8, 9, 1, 10 });

	// Act
	MEM_FUN(l, insert).A(4).A(9));

	for (int i = 0; i < 7; i++)
		ASSERT_EQ(l[i], expected[i]);
}

TEST(list, remove_Sanity)
{
	// Arrange
	auto l = list({ 10, 2, 5, 8, 1, 10 });
	auto expected = list({ 10, 2, 5, 8, 10 });

	// Act
	MEM_FUN(l, remove).A(1));

	for (int i = 0; i < 5; i++)
		ASSERT_EQ(l[i], expected[i]);
}

TEST(list, index_Sanity)
{
	// Arrange
	auto l = list({ 10, 2, 5, 8, 1, 10 });
	auto expected = 2;
	// Act
	auto actual = MEM_FUN(l, index).A(5));

	ASSERT_TRUE(actual == expected);
}

TEST(list, reverse_Sanity)
{
	// Arrange
	auto l = list({ 1,2,9,5,4 });
	auto expected = list({4,5,9,2,1 });

	// Act
	MEM_FUN(l, reverse));

	for (int i = 0; i < 5; i++)
		ASSERT_EQ(l[i], expected[i]);
}

TEST(list, copy_Sanity)
{
	// Arrange
	auto l = list({ 1, 2., "hello", 1 });
	auto expected = list({ 1, 2., "hello", 1 });

	// Act
	auto actual = MEM_FUN(l, copy));


	// Assert
	for (int i = 0; i < 4; i++)
		ASSERT_EQ(actual[i], expected[i]);
}

TEST(list, count_Sanity)
{
	// Arrange
	auto l = list({ 1, 2., "hello", 1 });

	// Act
	MEM_FUN(l, clear));

	int outSize = 0;

	for (auto elem : l)
		outSize++;

	// Assert
	ASSERT_EQ(outSize, 0);
}

TEST(list, operatorSquareBrackets_ModifiesInnerElements)
{
	// Arrange
	auto l = list({ 1, 2., "hello", 1 });

	// Act
	l[0] = "adsf";

	bool isEq = l[0] == "adsf";

	// Assert
	ASSERT_TRUE(l[0] == "adsf");
}


TEST(list, operatorSquareBrackets_SupportsNegativeIndexes)
{
	// Arrange
	auto l = list({ 1, 2., "hello", 1 });

	// Act, Assert
	ASSERT_TRUE(l[-1] == 1);
}

//
//TEST(dict, Initialization_SanityTest)
//{
//	// Arrange, Act
//	auto d = dict({ {1, 2.}, {"hello", 35} });
//
//	// Assert
//	ASSERT_TRUE(d[1] == 2.);
//	ASSERT_TRUE(d["hello"] == 35);
//}

