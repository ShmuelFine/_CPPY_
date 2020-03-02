#include "PyBlockParser.h"
using namespace std;

namespace py
{
	
	PyBlockParser::PyBlockParser(std::string const& newLine)
		:LineParser(new PyLineParser()), ENDL(newLine)
	{
	}
	
	string PyBlockParser::ParseBlock(vector<string> const lines)
	{
		vector<ICommandPtr> commands;
		for (auto l : lines)
			commands.push_back(LineParser->Parse(l));
		
		if (commands.empty())
			return string();

		THROW_UNLESS(commands[0]->Indentation.size() == 0, "First command should start from zero indentation.");
		Indintations.push_back(Indentation(0, commands[0]));

		string str = "";
		for (int cmdIdx = 0; cmdIdx < commands.size(); cmdIdx++)
		{
			// Handle scopes:
			auto prevIndentation = Indintations.back();
			int currIndentation = commands[cmdIdx]->Indentation.length();
			if (currIndentation > prevIndentation.SpacesAmount)
			{
				str += "{" + ENDL;
				Indintations.push_back(Indentation(currIndentation, commands[cmdIdx]));
			}
			else if (currIndentation < prevIndentation.SpacesAmount)
			{
				str += string(currIndentation, ' ') + "}" + ENDL;
				str += prevIndentation.ScopeStarter->ScopeEndHook();
				Indintations.pop_back();
				THROW_UNLESS(currIndentation == Indintations.back().SpacesAmount, "Not allwoed indentation");
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
			str += string(Indintations[i].SpacesAmount, ' ') + "}" + ENDL;
		}
		return str;
	}

}