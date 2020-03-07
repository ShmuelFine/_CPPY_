#include "pch.h"
#include "../Base/Types/range.h"
#include "../Base/Types/FunDefs.h"
#include <iostream>
using namespace py;

TEST(range, initialization_3_params_Sanity_Test)
{
	//Arrange
	auto r = range(0, 6, 2);
	int compArr[] = { 0, 2, 4 };
	int resArr[3];

	//Act
	int t = 0;
	for (auto i : r)
	{
		std::cout << (std::string)i;
	}
	for (int i = r.attr(start); i < int(r.attr(stop)); i += int(r.attr(step)), t++)
		resArr[t] = i;

	//Assert
	ASSERT_TRUE(r.attr(start) == 0);
	ASSERT_TRUE(r.attr(stop) == 6);
	ASSERT_TRUE(r.attr(step) == 2);

	ASSERT_EQ(compArr[0], resArr[0]);
	ASSERT_EQ(compArr[1], resArr[1]);
	ASSERT_EQ(compArr[2], resArr[2]);

}

TEST(range, initialization_2_params_Sanity_Test)
{
	//Arrange
	auto r = range(1, 6);
	int compArr[] = { 1, 2, 3, 4, 5 };
	int resArr[5];

	//Act
	int t = 0;

	for (int i = r.attr(start); i < int(r.attr(stop)); i += int(r.attr(step)), t++)
		resArr[t] = i;

	//Assert
	ASSERT_TRUE(r.attr(start) == 1);
	ASSERT_TRUE(r.attr(stop) == 6);
	ASSERT_TRUE(r.attr(step) == 1);

	ASSERT_EQ(compArr[0], resArr[0]);
	ASSERT_EQ(compArr[1], resArr[1]);
	ASSERT_EQ(compArr[2], resArr[2]);
	ASSERT_EQ(compArr[3], resArr[3]);
	ASSERT_EQ(compArr[4], resArr[4]);

}

TEST(range, initialization_single_param_sanity_test)
{
	//Arrange
	auto r = range(6);
	int compArr[] = { 0, 1, 2, 3, 4, 5 };
	int resArr[6];

	//Act
	int t = 0;

	for (int i = r.attr(start); i < int(r.attr(stop)); i += int(r.attr(step)), t++)
		resArr[t] = i;

	//Assert
	ASSERT_TRUE(r.attr(start) == 0);
	ASSERT_TRUE(r.attr(stop) == 6);
	ASSERT_TRUE(r.attr(step) == 1);

	ASSERT_EQ(compArr[0], resArr[0]);
	ASSERT_EQ(compArr[1], resArr[1]);
	ASSERT_EQ(compArr[2], resArr[2]);
	ASSERT_EQ(compArr[3], resArr[3]);
	ASSERT_EQ(compArr[4], resArr[4]);
	ASSERT_EQ(compArr[5], resArr[5]);

}

TEST(range, initialization_step_throws_when_passed_zero)
{
	//Assert
	ASSERT_THROW(range(0, 10, 0), PyBaseException);

}

TEST(range, Initialization_AddsMemberFunctionsAndMembersAsAttributes)
{
	// Arrange
	auto r = range(1, 6);

	// Act, Assert
	r.hasattr("count");
	r.hasattr("index");
	r.hasattr("start");
	r.hasattr("step");
	r.hasattr("stop");
}


TEST(range, startAttributeIsReadableYetUnchangable)
{
	// Arrange
	range r(4, 10, 2);

	// Act
	int startVal = r.attr(start);
	// Assert
	ASSERT_EQ(startVal, 4);
	ASSERT_THROW(r.attr(start) = 5, PyBaseException);

}




TEST(range, count_returns_1)
{
	//arrange
	auto r = range(1, 6);
	auto r2 = range(0, -10, -1);
	//act
	int actual = MEM_FUN(r, count).A(1));
	int actual2 = MEM_FUN(r, count).A(5));
	int actual3 = MEM_FUN(r, count).A(-3));
	int expected = 1;


	//assert
	ASSERT_EQ(actual, expected);
	ASSERT_EQ(actual2, expected);
	ASSERT_EQ(actual2, expected);
}

TEST(range, count_returns_0)
{
	//arrange
	auto r = range(1, 6);
	//act
	int actual = MEM_FUN(r, count).A(7));
	int expected = 0;


	//assert
	ASSERT_EQ(actual, expected);
}

TEST(range, when_passed_str_count_returns_0)
{
	//arrange
	auto r = range(1, 6);
	//act
	int actual = MEM_FUN(r, count).A("ttt"));
	int expected = 0;


	//assert
	ASSERT_EQ(actual, expected);
}

TEST(range, index_returnsIndexWhenAppropriate)
{
	//arrange
	auto r = range(1, 6);
	//act
	int actual = MEM_FUN(r, index).A(2));
	int expected = 1;

	int actual2 = MEM_FUN(r, index).A(4));
	int expected2 = 3;

	//assert
	ASSERT_EQ(actual, expected);
	ASSERT_EQ(actual2, expected2);
}

TEST(range, index_whenStepIsNegative_returnsIndexWhenAppropriate)
{
	//arrange
	auto r = range(10, 1, -1);
	auto r2 = range(0, -10, -1);
	//act
	int actual = MEM_FUN(r, index).A(5));
	int expected = 5;

	int actual2 = MEM_FUN(r2, index).A(-2));
	int expected2 = 2;

	//assert
	ASSERT_EQ(actual, expected);
	ASSERT_EQ(actual2, expected2);
}



TEST(range, index_ifValueDoesntExistThenThrows)
{
	//arrange
	auto r = range(1, 6);

	//#define MEM_FUN(obj, fun) obj.attr(fun)(ARGS(obj)
		//r.attr(index)(ARGS(r).A(9));
		//assert
	ASSERT_THROW(r.attr(index)(ARGS(r).A(9)), PyBaseException);
	ASSERT_THROW(r.attr(index)(ARGS(r).A("str")), PyBaseException);

}
/*
TEST(range, n)
{
	//arrange

	//act

	//assert
}*/
