#include "pch.h"

#include "..\Base\Types\object.h"
#include "..\Base\Types\pyLambda.h"
#include "..\Base\Types\pyList.h"
using namespace py;

TEST(listTest, InitializationAndIteration_Sanity)
{
	// Arrange
	std::vector<object> expected = { 1, 2.5f, 3., "hello" };
	pyList l(expected);
	
	auto it = l.begin();
	auto val = *it;

	// Act, Assert
	int i = 0;
	for (auto o : l)
	{
		EXPECT_TRUE(*o == *expected[i++]);
	}
}


TEST(listTest, LambdaInitialization_Sanity)
{
	// Arrange
	pyList l({ pyInt(1).Clone(), Float(2.5f).Clone(), Double(3.).Clone(), pyStr("hello").Clone() });
	object l2 = LAMBDA_BASE(pyStr(*x).Clone(), x, l);
	
	// TODO: are those strings platform dependant?
	std::vector<object> expected = {"1","2.500000", "3.000000", "hello" };

	// Act, Assert
	for (int i = 0; i < expected.size(); i++)
	{
		EXPECT_TRUE(l2[i] == expected[i]);
	}
}
