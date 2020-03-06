#include "PyBlockParser.h"
#include "pyStr.h"

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
		ScopesStack.push_back(ScopeMetaData(0, GetEmptyCommand()));

		string str = "";
		for (int cmdIdx = 0; cmdIdx < commands.size(); cmdIdx++)
		{
			// If command is empty don't bother:
			if (commands[cmdIdx]->IsEmpty())
			{
				str += commands[cmdIdx]->Translate();
				continue;
			}

			if (cmdIdx == 21)
				int kalsd = 0;
			// Handle scopes:
			auto prevIndentation = ScopesStack.back();
			size_t currIndentation = commands[cmdIdx]->Indentation.length();
			if (currIndentation > prevIndentation.SpacesAmount)
			{
				str += string(prevIndentation.SpacesAmount, ' ') + "{" + ENDL;
				ScopesStack.push_back(ScopeMetaData(currIndentation, commands[cmdIdx-1]));
			}
			else if (currIndentation < prevIndentation.SpacesAmount)
			{
				str += string(prevIndentation.SpacesAmount, ' ') + "}" + ENDL;
				str += prevIndentation.ScopeStarter->ScopeEndHook();
				ScopesStack.pop_back();
				//THROW_UNLESS(currIndentation == ScopesStack.back().SpacesAmount, "Not allwoed indentation");
			}
			
			// PostProcess command:
			auto actualIndentation = ScopesStack.back();
			auto actualCommand = actualIndentation.ScopePostProcessor->Process(commands[cmdIdx]);

			// Predefine variables:
			auto vars = actualCommand->GetVariables();
			if (!vars.empty())
			{
				str += string(currIndentation, ' ') + "object " + *vars.begin();
				for (auto it = ++vars.begin(); it != vars.end(); it++)
					str += ", " + *it;
				str += ";" + ENDL;
			}
			
			// Go:
			str += actualCommand->Translate();
			if (actualCommand->IsToSemicolon())
				str += ";";

			str += ENDL;

		}

		for (int i = ScopesStack.size() - 1; i >= 1; i--)
		{
			str += string(ScopesStack[i - 1].SpacesAmount, ' ') + "}" + ENDL;
			str += string(ScopesStack[i - 1].SpacesAmount, ' ') + ScopesStack.back().ScopeStarter->ScopeEndHook();
			ScopesStack.pop_back();

		}
		return str;
	}

}