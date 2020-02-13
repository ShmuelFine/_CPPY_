#include "pch.h"
#include "..\Interpreter\Interpreter.h"

using namespace py;

TEST(Interperter, TestName) 
{
    // Arrange
    PyParser parser;
    // Act
    auto command = parser.Parse("x = 5 + f(78)");
    auto cppStr = command->Translate();

    // Assert
    EXPECT_EQ(cppStr, "object x = 5 + f(78)");
}