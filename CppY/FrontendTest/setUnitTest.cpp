#include "pch.h"
#include "..\Base\Types\set.h"
#include "..\Base\Types\list.h"
#include "..\Base\Types\TypeSpecificConstruction.h"
#include "..\Base\Types\Built_In_Functions.h"

using namespace py;

TEST(set, difference_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });
	set other = set({ 1,3 });
	set other2 = set({ 2 });
	set expected = set({ 4 });
	list other_lst = list({ other,other2 });

	// Act
	auto actual = MEM_FUN(s, difference).A(other_lst));

	//pyObj* o;
	//auto s = (std::string) (*o);
	//auto s2 = (std::string)expected;
	//auto s1 = (std::string)actual;

	//// Assert
	//printf("%s", s2.c_str());
	//printf("%s", s1.c_str());
	ASSERT_TRUE(expected == actual);
}

TEST(set, difference_update_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });
	set other = set({ 3 });
	set other1 = set({ 2 });
	set expected = set({ 1,4 });

	// Act
	MEM_FUN(s, difference_update).A(list({ other,other1 })));

	//pyObj* o;
	//auto s = (std::string) (*o);
	//auto s2 = (std::string)expected;
	//auto s1 = (std::string)s;

	//// Assert
	//printf("%s", s2.c_str());
	//printf("%s", s1.c_str());

	ASSERT_TRUE(expected == s);
}

TEST(set, discard_Sanity)
{
	// Arrange
	set s = set({ 1,"dd",6,4 });
	set expected = set({ 1,6,4 });

	// Act
	MEM_FUN(s, discard).A("dd"));

	//pyObj* o;
	//auto s = (std::string) (*o);
	//auto s2 = (std::string)expected;
	//auto s1 = (std::string)s;

	//// Assert
	//printf("%s", s2.c_str());
	//printf("%s", s1.c_str());

	ASSERT_TRUE(expected == s);
}

TEST(set, intersection_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });
	set other = set({ 3,2,7 });
	set other1 = set({ 2,4 });
	set expected = set({ 2 });
	list other_lst({ other,other1 });
	// Act
	auto actual = MEM_FUN(s, intersection).A(other_lst));

	//pyObj* o;
	//auto s = (std::string) (*o);
	//auto s2 = (std::string)expected;
	//auto s1 = (std::string)actual;

	//// Assert
	//printf("%s", s2.c_str());
	//printf("%s", s1.c_str());

	ASSERT_TRUE(expected == actual);
}

TEST(set, intersection_update_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });
	set other = set({ 3,2,7 });
	set other1 = set({ 2,4,6 });
	set expected = set({ 2 });
	list other_lst = list({ other,other1 });

	// Act
	MEM_FUN(s, intersection_update).A(other_lst));

	//pyObj* o;
	//auto s = (std::string) (*o);
	//auto s2 = (std::string)expected;
	//auto s1 = (std::string)s;

	//// Assert
	//printf("%s", s2.c_str());
	//printf("%s", s1.c_str());

	ASSERT_TRUE(expected == s);
}

TEST(set, issuperset_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });
	set other = set({ 3,2 });

	// Act
	bool actual = MEM_FUN(s, issuperset).A(other));

	// Assert

	ASSERT_TRUE(true == actual);
}

TEST(set, add_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });
	set expected = set({ 1,2,3,4,5 });

	// Act
	MEM_FUN(s, add).A(5));

	//auto s2 = (std::string)expected;
	//auto s1 = (std::string)s;

	//// Assert
	//printf("%s", s2.c_str());
	//printf("%s", s1.c_str());

	ASSERT_TRUE(expected == s);
}

TEST(set, clear_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });
	set expected = set();

	// Act
	MEM_FUN(s, clear));

	// Assert

	ASSERT_TRUE(s==expected);
}

TEST(set, pop_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });
	object expected = 1;
	set expecteds = set({ 2,3,4 });

	// Act
	object actual = MEM_FUN(s, pop));

	//auto s1 = (std::string)s;

	//// Assert
	//printf("%s", s1.c_str());

	ASSERT_TRUE(expected == actual);
	ASSERT_TRUE(expecteds == s);
}


TEST(set, isdisjoint_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });
	set other1 = set({ 3,7 });
	set other2 = set({ 8,7 });

	// Act
	bool actual1 = MEM_FUN(s, isdisjoint).A(other1));
	bool actual2 = MEM_FUN(s, isdisjoint).A(other2));

	// Assert

	ASSERT_TRUE(false == actual1);
	ASSERT_TRUE(true == actual2);
}



TEST(set, issubset_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });
	set other = set({ 3,2 });

	// Act
	bool actual = MEM_FUN(other, issubset).A(s));

	// Assert

	ASSERT_TRUE(true == actual);
}

TEST(set, union_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });
	set other = set({ 5,9 });
	set other1 = set({ 10,15 });
	set expected = set({ 1,2,3,4,5,9,10,15 });

	// Act
	object actual = MEM_FUN(s, union).A(list({ other ,other1})));

	//auto s2 = (std::string)expected;
	//auto s1 = (std::string)actual;

	//// Assert
	//printf("%s", s2.c_str());
	//printf("%s", s1.c_str());

	ASSERT_TRUE(expected == actual);
}
TEST(set, symmetric_difference_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });
	set other = set({ 5,1,4,6 });
	set expected = set({ 2,3,5,6 });

	// Act
	object actual = MEM_FUN(s, symmetric_difference).A(other));

	//auto s1 = (std::string)actual;

	//// Assert
	//printf("%s", s1.c_str());

	ASSERT_TRUE(expected == actual);
}

TEST(set, symmetric_difference_update_Sanity)
{
	set s = set({ 1,2,3,4 });
	set other = set({ 5,1,4,6 });
	set expected = set({ 2,3,5,6 });

	// Act
	MEM_FUN(s, symmetric_difference_update).A(other));

	//auto s1 = (std::string)s;

	//// Assert
	//printf("%s", s1.c_str());

	ASSERT_TRUE(expected == s);
}


TEST(set, remove_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });

	// Act

	// Assert
	//MEM_FUN(s, remove).A(0))
	ASSERT_THROW(
		s.attr(remove)(ARGS(s).A(0)),  PyBaseException);

}

TEST(set, update_Sanity)
{
	// Arrange
	set s = set({ 1,2,3,4 });
	set other = set({ 5,9 });
	set other1 = set({ 10,15 });
	set expected = set({ 1,2,3,4,5,9,10,15 });

	// Act
	MEM_FUN(s, update).A(list({ other ,other1 })));

	ASSERT_TRUE(expected == s);

}