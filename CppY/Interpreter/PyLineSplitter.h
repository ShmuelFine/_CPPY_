#pragma once

#include "Interpreter.h"

namespace py
{
	class PyLineAggregator
	{
	public:
		std::string Line;
		std::map<std::string, int> bracketsBalance;
		std::map<std::string, std::string> bracketsClosures;

	public:
		PyLineAggregator();
		void Append(std::string const& str);
		bool IsComplete();
		
	};
}