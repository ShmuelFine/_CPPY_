#include "pch.h"
#include "..\Base\Types\Built_In_Functions.h"
#include "..\Base\Types\list.h"
#include "..\Base\Types\Int.h"
#include "..\Base\Types\_Exceptions.h"
#include "..\Base\Types\FunDefs.h"
using namespace py;

TEST(len, SanityTest)
{
	// Arrange
	auto emptyList = list();
	auto l = list({ 1,"hello",3.5 });
	object x = 5;
	
	// Act, Assert
	ASSERT_EQ(len(ARGS(emptyList)), 0);
	ASSERT_EQ(len(ARGS(l)), 3);
	ASSERT_THROW(len(ARGS(x)), py::PyBaseException);


}
//
//TEST(bin, SanityTest)
//{
//	Int x1 = Int(68);
//	std::string res = bin(ARGS(x1));
//	ASSERT_EQ(res, "0b0111");
//}