#include "pch.h"
#include "..\Base\Types\object.h"
#include "..\Base\Types\FunDefs.h"
using namespace py;
//
//void test(pyObj& paramsObj)
//{
//	pyDict * params_ptr = reinterpret_cast<pyDict *>(&paramsObj);
//	pyDict & params = *params_ptr;
//	int __param_counter__ = 0;
//
//	object _key__paramName("paramName");
//	bool isNamedArgExist__paramName = params._impl.count(_key__paramName);
//	bool isUnNamedArgExist__paramName = params._impl.count(object(__param_counter__));
//	THROW_UNLESS(isNamedArgExist__paramName || isUnNamedArgExist__paramName, "Not enough parameters given - can't fetch param " "paramName");
//	object paramName = isNamedArgExist__paramName ? params[_key__paramName] : params[object(__param_counter__++)];
//
//
//}

FUN_DEF(multTwoNums)
PARAM(a, );
PARAM(b, );
return a + b;
END_FUN(multTwoNums)

TEST(pyFunctions, WhenAllArgsAreNamed)
{
	// Arrange
	int a = 10;
	int b = 5;
	
	// Act
	object c = multTwoNums(ARGS("a", a).A("b", b));
	
	// Assert
	ASSERT_TRUE(c == 15);
}

TEST(pyFunctions, WhenAllArgsAre_NOT_Named)
{
	// Arrange
	int a = 10;
	int b = 5;
	
	// Act
	object c = multTwoNums(ARGS(a).A(b));

	// Assert
	ASSERT_TRUE(c == 15);
}

TEST(pyFunctions, WhenOnlySuffixOfAllArgsISNamed)
{
	// Arrange
	int a = 10;
	int b = 5;

	// Act
	object c = multTwoNums(ARGS(a).A("b", b));

	// Assert
	ASSERT_TRUE(c == 15);
}

TEST(pyFunctions, When_UnnamedArgs_PlacedAfter_NamedArgs_ThenThrows)
{
	// Arrange
	int a = 10;
	int b = 5;

	// Act
	// Assert
	ASSERT_THROW(
	object c = multTwoNums(ARGS("a", a).A(b)), std::exception);
}

TEST(pyFunctions, When_UNNamedArgs_AreMissing_ThenThrows)
{
	// Arrange
	int a = 10;
	int b = 5;

	// Act
	// Assert
	ASSERT_THROW(
		object c = multTwoNums(ARGS(a)), std::exception);

	// wrong named arg name:
	ASSERT_THROW(
		object c = multTwoNums(ARGS(a).A("c", b)), std::exception);
}