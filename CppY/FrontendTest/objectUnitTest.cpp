#include "pch.h"

#include "..\Base\Types\object.h"
using namespace py;

TEST(objectTest, IntializationAndComparison_ValidCase)
{
    // Arrange
    std::vector<object> objs = { 3, 3.5f, 4., false, "hello" };

    // Act, Assert
    ASSERT_TRUE(objs[0] == 3);
    ASSERT_TRUE(objs[1] == 3.5f);
    ASSERT_TRUE(objs[2] == 4.);
    ASSERT_TRUE(objs[3] == false);
    ASSERT_TRUE(objs[4] == "hello");
}

TEST(objectTest, NoneIsNotEqualToAnythingButItself)
{
    // Arrange
    std::vector<object> objs = { 3, 3.5f, 4., false, "hello" };
    
    // Act, Assert
    for (auto & obj : objs)
        ASSERT_TRUE(None != obj && obj != None);

    ASSERT_FALSE(None != None);
    ASSERT_TRUE(None == None);
}

TEST(objectTest, DefineAndFetchAttributesByName_WhenAttributesAreDataTypes_SanityTest)
{
    // Arrange
    object tested;
    
    // Act
    tested.attr(3) = 3;
    tested.attr(b) = false;
    tested.attr(some_double) = 3.;
    tested.attr(other_float) = 3.5f;
    tested.attr(myName) = "hello";

    // Assert
    ASSERT_TRUE(tested.attr(3) == 3);
    ASSERT_TRUE(tested.attr(b) == false);
    ASSERT_TRUE(tested.attr(some_double) == 3.);
    ASSERT_TRUE(tested.attr(other_float) == 3.5f);
    ASSERT_TRUE(tested.attr(myName) == "hello");

    ASSERT_TRUE(tested.getattr("3") == 3);
    ASSERT_TRUE(tested.getattr("b") == false);
    ASSERT_TRUE(tested.getattr("some_double") == 3.);
    ASSERT_TRUE(tested.getattr("other_float") == 3.5f);
    ASSERT_TRUE(tested.getattr("myName") == "hello");
}