#pragma once

#include "Interpreter.h"

namespace py
{
	class PyBlockParser
	{
		std::string ENDL;
	public:
		PyBlockParser(std::string const & newLine = "\r\n");
		std::string ParseBlock(std::vector<std::string> const lines);
	public:
		PyParserPtr LineParser;

		std::vector<VariablesMetaDataPtr> ScopeMDs_Stack;
		std::vector<int> Indintations;
	};

}