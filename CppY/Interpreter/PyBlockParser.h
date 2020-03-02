#pragma once

#include "Interpreter.h"

namespace py
{

	struct ScopeMetaData
	{
	public:
		size_t SpacesAmount;
		ICommandPtr ScopeStarter;
		ICommandPostProcessorPtr ScopePostProcessor;
		ScopeMetaData(size_t size, ICommandPtr cmd)//, ICommandPostProcessorPtr postProc = std::make_shared<DefaultPostProcessor>()) 
			: SpacesAmount(size), ScopeStarter(cmd), ScopePostProcessor(ScopeStarter->ScopeInnerHook())
		{}

	};

	class PyBlockParser
	{
		std::string ENDL;
	public:
		PyBlockParser(std::string const & newLine = "\n");
		std::string ParseBlock(std::vector<std::string> const lines);
	public:
		PyLineParserPtr LineParser;

		std::vector<ScopeMetaData> ScopesStack;
	};

}