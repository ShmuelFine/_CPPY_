#include "pch.h"

#include "..\Base\Types\object.h"
using namespace py;

TEST(NumericObjectTest, AdditionWithPOD_Sanity)
{
	// Arrange
	object I = 5;
	
	// Act
	object J = I + 8;

	// Assert
	ASSERT_TRUE(J == 5 + 8);
}


TEST(NumericObjectTest, AdditionWithObj_Sanity)
{
	// Arrange
	object I = 5;
	object K = 8;

	// Act
	object J = I + K;

	// Assert
	ASSERT_TRUE(J == 5 + 8);
}



TEST(NumericObjectTest, PostAndPreIncrement_Sanity)
{
	// Arrange
	object I = 5;

	// Act
	object post_inc = I++;
	I = 5;
	object pre_inc = ++I;

	// Assert
	ASSERT_TRUE(post_inc == 5);
	ASSERT_TRUE(pre_inc == 6);
}