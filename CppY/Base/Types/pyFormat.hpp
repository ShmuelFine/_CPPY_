#pragma once
//
//#include <iostream>
//#include <stdarg.h>
//#include <regex>
//#include "PyObj.h"
//
//int MatchesAmount(std::string const& s, std::string const& re);
//
//template<typename T>
//std::string toString(const T& t) { return std::to_string(t); }
//std::string toString(const char* t);
//std::string toString(const char& t);
//std::string toString(const std::string& t);
//
//std::string pyFormat(std::string s);
//
//template<typename T, typename... Args>
//std::string pyFormat(std::string s, T value, Args... args)
//{
//	using namespace std;
//	int placeHolders = MatchesAmount(s, "\\{.*?\\}");
//	if (placeHolders > 0)
//	{
//		smatch currMatch;
//		if (!regex_search(s, currMatch, regex("\\{(.*?)\\}")))
//			throw "BUG";
//
//		std::string nextToken = currMatch[1];
//		std::string newS;
//		if (!nextToken.empty())
//			newS = regex_replace(s, regex("\\{" + nextToken + "\\}"), toString(value));
//		else
//			newS = regex_replace(s, regex("\\{" + nextToken + "\\}"), toString(value), std::regex_constants::format_first_only);
//
//		return pyFormat(newS, args...);
//	}
//	else
//		return s;
//}
