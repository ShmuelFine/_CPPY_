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

TEST(Interperter, GetVariables_RetrievesVariablesDefinedInLine)
{
    // Arrange
    PyParser parser;
    // Act
    auto command = parser.Parse(R"(x = [5, 'asdf', "asdf", f(78, x, "asdf", y), 6+7, z] )");

    // Assert
    auto vars = command->GetVariables();
    ASSERT_TRUE(vars.count("x"));
    ASSERT_FALSE(vars.count("y"));// Used, but not defined
    ASSERT_FALSE(vars.count("z"));// Used, but not defined
    ASSERT_FALSE(vars.count("w"));
}

TEST(Interperter, GetVariables_Retrieves_Multiple_VariablesDefinedInLine)
{
    // Arrange
    PyParser parser;
    // Act
    auto command = parser.Parse(R"(x,y,z = [5, 'asdf', "asdf", f(78, x0, "asdf", y0), 6+7, w] )");

    // Assert
    auto vars = command->GetVariables();
    ASSERT_TRUE(vars.count("x"));
    ASSERT_TRUE(vars.count("y"));// Used, but not defined
    ASSERT_TRUE(vars.count("z"));// Used, but not defined
    ASSERT_FALSE(vars.count("w"));
}

TEST(Interperter, GetVariables_Retrieves_Multiple_VariablesDefinedIn_ForStatement)
{
    // Arrange
    PyParser parser;
    // Act
    auto command = parser.Parse(R"(for x,y,z in os.walk(some_folder):)");

    // Assert
    auto vars = command->GetVariables();
    ASSERT_TRUE(vars.count("x"));
    ASSERT_TRUE(vars.count("y"));// Used, but not defined
    ASSERT_TRUE(vars.count("z"));// Used, but not defined
    ASSERT_FALSE(vars.count("w"));
}