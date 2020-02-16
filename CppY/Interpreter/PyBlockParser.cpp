#include "PyBlockParser.h"
using namespace std;

namespace py
{
	
	PyBlockParser::PyBlockParser(std::string const& newLine)
		:LineParser(new PyLineParser()), ENDL(newLine)
	{
		ScopeMDs_Stack.push_back(VariablesMetaDataPtr(new VariablesMetaData()));
		Indintations.push_back(0);
	}
	
	string PyBlockParser::ParseBlock(vector<string> const lines)
	{
		vector<ICommandPtr> commands;
		for (auto l : lines)
			commands.push_back(LineParser->Parse(l));
		
		if (commands.empty())
			return string();

		THROW_UNLESS(commands[0]->Indentation.size() == 0, "First command should start from zero indentation.");
		
		string str = "";
		for (int cmdIdx = 0; cmdIdx < commands.size(); cmdIdx++)
		{
			// Handle scopes:
			int prevIndentation = Indintations.back();
			int currIndentation = commands[cmdIdx]->Indentation.length();
			if (currIndentation > prevIndentation)
			{
				str += "{" + ENDL;
				Indintations.push_back(currIndentation);
			}
			else if (currIndentation < prevIndentation)
			{
				str += string(currIndentation, ' ') + "}" + ENDL;
				Indintations.pop_back();
				THROW_UNLESS(currIndentation == Indintations.back(), "Not allwoed indentation");
			}
			
			// Predefine variables:
			auto vars = commands[cmdIdx]->GetVariables();
			if (!vars.empty())
			{
				str += string(currIndentation, ' ') + "object " + *vars.begin();
				for (auto it = ++vars.begin(); it != vars.end(); it++)
					str += ", " + *it;
				str += ";" + ENDL;
			}
			// Go:
			str += commands[cmdIdx]->Translate();
			if (commands[cmdIdx]->IsToSemicolon())
				str += ";";

			str += ENDL;

		}

		for (int i = Indintations.size() - 2; i >= 0; i--)
		{
			str += string(Indintations[i], ' ') + "}" + ENDL;
		}
		return str;
	}

}