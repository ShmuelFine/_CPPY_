#include "pch.h"
#include "../Base/Types/tuple.h"
#include "../Base/Types/FunDefs.h"
using namespace py;

TEST(tuple, initialization_Sanity_Test)
{
	auto t = tuple({ 1, 2., "hello" });

	// Assert
	ASSERT_TRUE(t[0] == 1);
	ASSERT_TRUE(t[1] == 2.);
	ASSERT_TRUE(t[2] == "hello");
}

TEST(tuple, Initialization_AddsMemberFunctionsAsAttributes)
{
	// Arrange
	auto t = tuple({ 1, 2., "hello" });

	// Act, Assert
	t.hasattr("count");
	t.hasattr("index");
}

TEST(tuple, brackets_on_RHS_doesnt_throw)
{
	//Arrange
	auto t = tuple({ 1, 2., "hello" });
	int p;

	//Act
	int i = t[0];

	//Assert
	ASSERT_NO_THROW(p = t[0]);
	ASSERT_TRUE(i == 1);
}


TEST(tuple, brackets_on_LHS_does_throw)
{
	// Arrange
	auto t = tuple({ 1, 2., "hello" });

	// Act, Assert
	ASSERT_THROW((t[0] = 5), PyBaseException);
}


TEST(tuple, count_returns_proper_count_when_value_is_indeed_present)
{
	// Arrange
	auto t = tuple({ 1, "hello", "hello", 3.5 });
	int expected = 1;
	int expected2 = 2;

	//Act
	int actual = MEM_FUN(t, count).A(3.5));
	int actual2 = MEM_FUN(t, count).A("hello"));


	// Act, Assert
	ASSERT_EQ(expected, actual);
	ASSERT_EQ(expected2, actual2);
}


TEST(tuple, count_returns_proper_count_when_value_is_not_present)
{
	// Arrange
	auto t = tuple({ 1, "hello", "hello", 3.5 });
	int expected = 0;


	//Act
	int actual = MEM_FUN(t, count).A(3.2));
	int actual2 = MEM_FUN(t, count).A("he"));


	// Act, Assert
	ASSERT_EQ(expected, actual);
	ASSERT_EQ(expected, actual2);
}

TEST(tuple, index_returns_properly_when_value_is_indeed_present)
{
	// Arrange
	auto t = tuple({ 1, "hello", "hello", 3.5 });
	int expected = 0;
	int expected2 = 1;

	//Act
	int actual = MEM_FUN(t, index).A(1));
	int actual2 = MEM_FUN(t, index).A("hello"));


	// Act, Assert
	ASSERT_EQ(expected, actual);
	ASSERT_EQ(expected2, actual2);
}

TEST(tuple, index_throws_if_value_not_present)
{
	// Arrange
	auto t = tuple({ 1, "hello", "hello", 3.5 });



	// Act, Assert
	ASSERT_THROW(t.attr(index)(ARGS(t).A(9)), PyBaseException);
}
