#include "pch.h"
#include "..\Interpreter\Interpreter.h"
#include "..\Interpreter\PyBlockParser.h"
#include "..\Interpreter\PyLineSplitter.h"
#include "..\Base\Types\pyStr.h"
#include <fstream>

using namespace py;

TEST(Interperter, funcParsing_WrapsArgsWithMacro)
{
    // Arrange
    PyLineParser parser;
    // Act
    auto command = parser.Parse(R"(print(a,b,c))");
    auto cppStr = command->Translate();

    // Assert
    EXPECT_EQ(cppStr, "print(ARGS(a).A(b).A(c))");
}

TEST(Interperter, var_equal_cons_plus_func__Fixture)
{
    // Arrange
    PyLineParser parser;
    // Act
    auto command = parser.Parse("x = 5 + f(78)");
    auto cppStr = command->Translate();

    // Assert
    EXPECT_EQ(cppStr, "x = 5 + f(ARGS(78))");
}

TEST(Interperter, var_plus_func_equal_cons_plus_func__Fixture)
{
    // Arrange
    PyLineParser parser;
    // Act
    auto command = parser.Parse("x + sin(x) = 5 + f(78)");
    auto cppStr = command->Translate();

    // Assert
    EXPECT_EQ(cppStr, "x + sin(ARGS(x)) = 5 + f(ARGS(78))");
}

TEST(Interperter, list_nesting_Fixture)
{
    // Arrange
    PyLineParser parser;
    // Act
    auto command = parser.Parse(R"(x = [5, 'asdf', "asdf", f(78, "asdf", y), 6+7] )");
    auto cppStr = command->Translate();

    // Assert
    EXPECT_EQ(cppStr, R"(x = list({5, "asdf", "asdf", f(ARGS(78).A("asdf").A(y)), 6+7}) )");
}

TEST(Interperter, GetVariables_RetrievesVariablesDefinedInLine)
{
    // Arrange
    PyLineParser parser;
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
    PyLineParser parser;
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
    PyLineParser parser;
    // Act
    auto command = parser.Parse(R"(for x,y,z in os.walk(some_folder):)");

    // Assert
    auto vars = command->GetVariables();
    ASSERT_TRUE(vars.count("x"));
    ASSERT_TRUE(vars.count("y"));// Used, but not defined
    ASSERT_TRUE(vars.count("z"));// Used, but not defined
    ASSERT_FALSE(vars.count("w"));
}

TEST(BlockParser, scopes_and_variable_Defs)
{
    // Arrange
    PyBlockParser blockparser("\n");
    std::vector<std::string> lines = {
    "x = 5",
    "if x > 8:",
    "   print(\"hello\")",
    "else:",
    "   y = 8",
    "   print(x + y)",
    };
    // Act
    auto cppBlock = blockparser.ParseBlock(lines);
    // Assert
    std::string expected =
        R"(object x;
x = 5;
if (x > 8)
{
   print(ARGS("hello"));
}
else
{
   object y;
   y = 8;
   print(ARGS(x + y));
}
)";
    EXPECT_EQ(cppBlock, expected);

}

TEST(BlockParser, funDefWithDocString)
{
    // Arrange
    PyBlockParser blockparser("\n");
    std::vector<std::string> lines = {
R"(def __init__(self, A, B):)",
R"(     """ this is doc string """ )",
R"(     pass)" };

    // Act
    auto cppBlock = blockparser.ParseBlock(lines);
    // Assert
    std::string expected =
        R"(FUN_DEF(__init__);
PARAM(self,);
PARAM(A,);
PARAM(B,);
{

     pass;
}
END_FUN_WITH_DOC_STR(__init__,      """ this is doc string """ );)";
    EXPECT_EQ(cppBlock, expected);

}

TEST(BlockParser, funDefWith_NO_DocString)
{
    // Arrange
    PyBlockParser blockparser("\n");
    std::vector<std::string> lines = {
R"(def __init__(self, A, B):)",
R"(     pass)" };

    // Act
    auto cppBlock = blockparser.ParseBlock(lines);
    // Assert
    std::string expected =
        R"(FUN_DEF(__init__);
PARAM(self,);
PARAM(A,);
PARAM(B,);
{
     pass;
}
END_FUN_WITH_DOC_STR(__init__, "");)";
    EXPECT_EQ(cppBlock, expected);

}

TEST(BlockParser, class_DefWith_DocString)
{
    // Arrange
    PyBlockParser blockparser("\n");
    std::vector<std::string> lines = {
R"(class Person:)"                     ,
R"(    """THIS IS A PERSON""")"           ,
R"(    def __init__(self, name, age):)",
R"(        self.name = name)"          ,
R"(        self.age = age)"            ,
};

    // Act
    auto cppBlock = blockparser.ParseBlock(lines);
    // Assert
    std::string expected =
        R"(FUN_DEF(__init__);
PARAM(self,);
PARAM(A,);
PARAM(B,);
{
     pass;
}
END_FUN_WITH_DOC_STR(__init__, "");)";
    EXPECT_EQ(cppBlock, expected);

}


TEST(BlockParser, ACTUAL_WORK)
{
    using namespace std;

    // Arrange
    PyBlockParser blockparser("\n");
    PyLineAggregator lineAgg;


    auto fileName = R"(C:\Dev\Stabilization\MeshFlow4Android\Orig\RT_Stab.py)";
    ifstream ifs(fileName);

    string line;
    vector<string> pyLines;
    while (getline(ifs, line))
    {
        lineAgg.Append(line);
        if (lineAgg.IsComplete())
        {
            pyLines.push_back(lineAgg.Line);
            lineAgg = PyLineAggregator();
        }
    }
    ifs.close();

    auto cppBlock = blockparser.ParseBlock(pyLines);

    ofstream ofs(R"(C:\Dev\Stabilization\MeshFlow4Android\Cpp\Src\RT_Stab.cpp)");
    ofs << cppBlock;
    ofs.close();
}