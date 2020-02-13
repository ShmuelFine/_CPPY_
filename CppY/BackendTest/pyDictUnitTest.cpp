#include "pch.h"
#include "..\Base\Types\object.h"
#include "..\Base\Types\pyDict.h"
#include <iostream>
using namespace py;

TEST(dictTest, InitializationAndIteration_Sanity)
{
	// Arrange
	std::map<object, object> initVec_objs = { {"hello","2"}, {"World","4"} };
	std::map<pyObjPtr, pyObjPtr> expected(initVec_objs.begin(), initVec_objs.end());
    pyDict d(expected);

	// Act, Assert
	ASSERT_TRUE(*d[pyStr("hello").Clone()] == pyStr("2"));
	ASSERT_TRUE(*d[pyStr("World").Clone()] == pyStr("4"));

	for (auto& kvp : d)
	{
		if (*(kvp->FetchByIdx(0)) == pyStr("hello"))
			ASSERT_TRUE(*(kvp->FetchByIdx(1)) == pyStr("2"));

		else if (*(kvp->FetchByIdx(0)) == pyStr("World"))
			ASSERT_TRUE(*(kvp->FetchByIdx(1)) == pyStr("4"));

		else
			FAIL() << " unknown key !";
	}
}

/*
TEST(defaultDictTest, InitializationAndIteration_Sanity)
{
	// Arrange
	//std::vector<std::pair<object, int> > vec1 = { {"hello",2}, {"World",4} };


	object l = pyDefaultDict<pyInt>({ {"hello",2}, {"World",4} });
	std::map<object, int> expected = { {"hello",2}, {"World",4} };

	auto it = l.begin();
	auto val = *it;

	// Act, Assert
	int i = 0;
	for (auto o : l)
	{
		EXPECT_TRUE(o == expected[i++]);
	}

}
*/

