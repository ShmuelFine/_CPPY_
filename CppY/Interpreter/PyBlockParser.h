#pragma once

#include "Interpreter.h"

namespace py
{
	struct Indentation
	{
	public:
		int SpacesAmount;
		ICommandPtr ScopeStarter;
		Indentation(int size, ICommandPtr cmd) : SpacesAmount(size), ScopeStarter(cmd) {}
	};

	class PyBlockParser
	{
		std::string ENDL;
	public:
		PyBlockParser(std::string const & newLine = "\r\n");
		std::string ParseBlock(std::vector<std::string> const lines);
	public:
		PyParserPtr LineParser;

		std::vector<Indentation> Indintations;
	};

}