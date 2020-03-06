#include "PyBlockParser.h"
#include "PyLineSplitter.h"
using namespace std;

namespace py
{
	int countSubstring(const std::string& str, const std::string& sub)
	{
		if (sub.length() == 0) return 0;
		int count = 0;
		for (size_t offset = str.find(sub); offset != std::string::npos;
			offset = str.find(sub, offset + sub.length()))
		{
			++count;
		}
		return count;
	}


	PyLineAggregator::PyLineAggregator() : Line("")
	{
		bracketsBalance["("] = 0;
		bracketsBalance["["] = 0;
		bracketsBalance["{"] = 0;
		bracketsBalance["\"\"\""] = 0;
		bracketsBalance["\""] = 0;
		bracketsBalance["'''"] = 0;
		bracketsBalance["'"] = 0;

		bracketsClosures["("]		= ")";
		bracketsClosures["["]		= "]";
		bracketsClosures["{"]		= "}";
		bracketsClosures["\"\"\""]	= "\"\"\"";
		bracketsClosures["\""]		= "\"";
		bracketsClosures["'''"]		= "'''";
		bracketsClosures["'"]		= "'";
		
	}

	void PyLineAggregator::Append(std::string const& str)
	{
		for (auto& kvp : bracketsBalance)
		{
			bracketsBalance[kvp.first] += countSubstring(kvp.first, str) - countSubstring(bracketsClosures[kvp.first], str);
		}
		Line += str;

	}
	bool PyLineAggregator::IsComplete()
	{
		bool isOK = true;
		for (auto& kvp : bracketsBalance)
			isOK &= (kvp.second == 0);
		
		return isOK;
	}
}