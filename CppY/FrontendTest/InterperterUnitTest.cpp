#include "pch.h"
#include "..\Interpreter\Interpreter.h"

using namespace py;

TEST(Interperter, var_equal_cons_plus_func__Fixture)
{
    // Arrange
    PyParser parser;
    // Act
    auto command = parser.Parse("x = 5 + f(78)");
    auto cppStr = command->Translate();

    // Assert
    EXPECT_EQ(cppStr, "x = 5 + f(78)");
}

TEST(Interperter, var_plus_func_equal_cons_plus_func__Fixture)
{
    // Arrange
    PyParser parser;
    // Act
    auto command = parser.Parse("x + sin(x) = 5 + f(78)");
    auto cppStr = command->Translate();

    // Assert
    EXPECT_EQ(cppStr, "x + sin(x) = 5 + f(78)");
}

TEST(Interperter, list_nesting_Fixture)
{
    // Arrange
    PyParser parser;
    // Act
    auto command = parser.Parse(R"(x = [5, 'asdf', "asdf", f(78, "asdf", y), 6+7] )");
    auto cppStr = command->Translate();

    // Assert
    EXPECT_EQ(cppStr, R"(x = list({5, "asdf", "asdf", f(78, "asdf", y), 6+7}) )");
}