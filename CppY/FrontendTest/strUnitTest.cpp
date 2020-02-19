#include "pch.h"
#include "../Base/Types/str.h"
#include "../Base/Types/FunDefs.h"
using namespace py;

TEST(str, capitalize_Sanity_Test)
{
	//Arrange
	auto s1 = str("hello");
	auto originalS1 = str("hello");;
	auto s2 = str("hELLO");
	auto expected = str("Hello");

	//Act
	auto actual1 = MEM_FUN(s1, capitalize));
	auto actual2 = MEM_FUN(s2, capitalize));

	//ASSERT
	ASSERT_EQ(actual1, expected);
	ASSERT_EQ(actual2, expected);	
}

TEST(str, capitalize_does_not_modify_original_string)
{
	//Arrange
	auto s1 = str("hello");
	auto originalS1 = str("hello");

	//Act
	auto actual1 = MEM_FUN(s1, capitalize));


	//ASSERT
	ASSERT_EQ(s1, originalS1);
}

TEST(str, casefold_sanity)
{
	//Arrange
	auto s1 = str("hELLO");
	auto expected = str("hello");

	//Act
	auto actual1 = MEM_FUN(s1, casefold));


	//ASSERT
	ASSERT_EQ(actual1, expected);
}

TEST(str, center_sanity)
{
	//Arrange
	auto s1 = str("sss");
	auto expected1 = str("  sss   ");
	auto expected2 = str("rrsssrrr");

	//Act
	auto actual1 = MEM_FUN(s1, center).A(7));
	auto actual2 = MEM_FUN(s1, center).A(8).A("r"));


	//ASSERT
	//ASSERT_EQ(actual1, expected1);
	ASSERT_EQ(actual2, expected2);
}

TEST(str, count_sanity)
{
	//Arrange
	auto s1 = str("asdasd");
	int expected = 2;

	//Act
	int actual = MEM_FUN(s1, count).A("s"));
	int actual2 = MEM_FUN(s1, count).A("asd"));
	int actual3 = MEM_FUN(s1, count).A("asd").A(2));
	int actual4 = MEM_FUN(s1, count).A("asd").A(0).A(2));
	int actual5 = MEM_FUN(s1, count).A("asd").A(0).A(3));

	//ASSERT
	ASSERT_EQ(actual, expected);
	ASSERT_EQ(actual2, expected);
	ASSERT_TRUE(actual3 == 1);
	ASSERT_TRUE(actual4 == 0);
	ASSERT_TRUE(actual5 == 1);
}

//TEST(str, endswith_sanity)
//{
//	//Arrange
//	auto s1 = str("asdasd");
//
//	//Act
//	bool actual = MEM_FUN(s1, endswith).A("s"));
//	bool actual2 = MEM_FUN(s1, endswith).A("d"));
//	bool actual3 = MEM_FUN(s1, endswith).A("s").A(0).A(4));
//	bool actual4 = MEM_FUN(s1, endswith).A("s").A(0).A(5));
//
//	ASSERT_FALSE(actual);
//	ASSERT_TRUE(actual2);
//	ASSERT_FALSE(actual3);
//	ASSERT_TRUE(actual4);
//}

//TEST(str, expandtabs_sanity)
//{
	//Arrange
	//auto s1 = str("\td");
	//auto expected1 = str("        d");
	//auto expected2 = str("    d");
	//auto expected3 = str(" d");
	//auto s2 = str("d\t");
	//auto expected4 = str("d       ");

	//Act
	//auto actual1 = MEM_FUN(s1, expandtabs));
	//auto actual2 = MEM_FUN(s1, expandtabs).A(4));
	//auto actual3 = MEM_FUN(s1, expandtabs).A(1));
	//auto actual4 = MEM_FUN(s2, expandtabs));

	//Assert
//    ASSERT_EQ(expected1, actual1);
//	ASSERT_EQ(expected2, actual2);
//	ASSERT_EQ(expected3, actual3);
//	ASSERT_EQ(expected4, actual4);
//}

TEST(str, isalnum_sanity)
{
	//Arrange
	auto s1 = str("1w23fd");
	auto s2 = str("");
	auto s3 = str("ewew3#");

	//ACT
	bool b1 = MEM_FUN(s1, isalnum));
	bool b2 = MEM_FUN(s2, isalnum));
	bool b3 = MEM_FUN(s3, isalnum));

	ASSERT_TRUE(b1);
	ASSERT_FALSE(b2);
	ASSERT_FALSE(b3);
}

TEST(str, isalpha_sanity)
{
	//Arrange
	auto s1 = str("1w23fd");
	auto s2 = str("");
	auto s3 = str("ewew3#");
	auto s4 = str("ewewRED");

	//ACT
	bool b1 = MEM_FUN(s1, isalpha));
	bool b2 = MEM_FUN(s2, isalpha));
	bool b3 = MEM_FUN(s3, isalpha));
	bool b4 = MEM_FUN(s4, isalpha));

	//Assert
	ASSERT_FALSE(b1);
	ASSERT_FALSE(b2);
	ASSERT_FALSE(b3);
	ASSERT_TRUE(b4);
}

TEST(str, isdecimal_sanity)
{
	//Arrange
	auto s1 = str("1w23fd");
	auto s2 = str("");
	auto s3 = str("635345");
	auto s4 = str("63&5345");

	//ACT
	bool b1 = MEM_FUN(s1, isdecimal));
	bool b2 = MEM_FUN(s2, isdecimal));
	bool b3 = MEM_FUN(s3, isdecimal));
	bool b4 = MEM_FUN(s4, isdecimal));

	//Assert
	ASSERT_FALSE(b1);
	ASSERT_FALSE(b2);
	ASSERT_TRUE(b3);
	ASSERT_FALSE(b4);
}

TEST(str, isidentifier_sanity)
{
	//Arrange
	auto s1 = str("fd434d");
	auto s2 = str("fd_434d");
	auto s3 = str("3fd_434d");
	auto s4 = str(" fd");

	//ACT
	bool b1 = MEM_FUN(s1, isidentifier));
	bool b2 = MEM_FUN(s2, isidentifier));
	bool b3 = MEM_FUN(s3, isidentifier));
	bool b4 = MEM_FUN(s4, isidentifier));

	//Assert
	ASSERT_TRUE(b1);
	ASSERT_TRUE(b2);
	ASSERT_FALSE(b3);
	ASSERT_FALSE(b4);
}

TEST(str, islower_sanity)
{
	//Arrange
	auto s = str("ew4343sd");
	auto s1 = str("ewewewsd");
	auto s2 = str("ewDSDSDwsd");

	//Act
	bool b = MEM_FUN(s, islower));
	bool b1 = MEM_FUN(s1, islower));
	bool b2 = MEM_FUN(s2, islower));

	//ASSERT
	ASSERT_TRUE(b);
	ASSERT_TRUE(b1);
	ASSERT_FALSE(b2);
	
}


TEST(str, isprintable_sanity)
{
	//Arrange
	auto s = str("ew4343sd");
	auto s1 = str("%&*ws d");
	auto s2 = str("My name is \n Ayush");

	//Act
	bool b = MEM_FUN(s, isprintable));
	bool b1 = MEM_FUN(s1, isprintable));
	bool b2 = MEM_FUN(s2, isprintable));

	//ASSERT
	ASSERT_TRUE(b);
	ASSERT_TRUE(b1);
	ASSERT_FALSE(b2);

}

TEST(str, isspace_sanity)
{
	//Arrange
	auto s = str("  ");
	auto s1 = str("");
	auto s2 = str(" fd ");

	//Act
	bool b = MEM_FUN(s, isspace));
	bool b1 = MEM_FUN(s1, isspace));
	bool b2 = MEM_FUN(s2, isspace));

	//ASSERT
	ASSERT_TRUE(b);
	ASSERT_FALSE(b1);
	ASSERT_FALSE(b2);

}

TEST(str, isupper_sanity)
{
	//Arrange
	auto s = str("RHDJDK6");
	auto s1 = str("");
	auto s2 = str("3");
	auto s3 = str("Gdh");

	//Act
	bool b = MEM_FUN(s, isupper));
	bool b1 = MEM_FUN(s1, isupper));
	bool b2 = MEM_FUN(s2, isupper));
	bool b3 = MEM_FUN(s3, isupper));

	//ASSERT
	ASSERT_TRUE(b);
	ASSERT_FALSE(b1);
	ASSERT_FALSE(b2);
	ASSERT_FALSE(b3);

}

