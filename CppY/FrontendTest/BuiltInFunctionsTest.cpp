#include "pch.h"
#include "..\Base\Types\Built_In_Functions.h"
#include "..\Base\Types\list.h"
#include "..\Base\Types\_Exceptions.h"
#include "..\Base\Types\FunDefs.h"
using namespace py;

TEST(len, SanityTest)
{
	// Arrange
	list emptyList;
	list l({ 1,"hello",3.5 });
	object x = 5;
	
	// Act, Assert
	ASSERT_EQ(len(ARGS(emptyList)), 0);
	ASSERT_EQ(len(ARGS(l)), 3);
	ASSERT_THROW(len(ARGS(x)), py::PyBaseException);


}