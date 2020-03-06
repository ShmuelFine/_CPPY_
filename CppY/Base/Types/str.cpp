#include "str.h"
#include "tuple.h"
#include "dict.h"
#include "list.h"
#include "FunDefs.h"
#include "Comparisons.h"
#include "Int.h"
#include <locale>
#include <codecvt>
#include <algorithm>
#include <regex>
#include <string>
namespace py
{

	namespace py_str
	{
		//not implemented- format-map, maketrans, encode, translate
		//tested
		FUN_DEF(capitalize);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		std::transform(data.begin(), data.end(), data.begin(),
			[](unsigned char c) { return std::tolower(c); });
		if (!data.empty())
			data[0] = std::toupper(data[0]);
		return data;
		END_FUN(capitalize);

		//tested
		FUN_DEF(casefold);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		return meAsStr->lower();
		END_FUN(casefold);

		//tested
		FUN_DEF(center);
		PARAM(self, );
		PARAM(width, );
		PARAM(fillchar, " ");
		//if (not is_ofType(width, Int(1)))
		//	THROW("integer value expected");

		//TODO:test if fillchar is unicode character
		//if (not is_ofType(fillchar, str("s")))
		//	THROW("The fill character must be  a unicode character");

		int wid = width;
		auto fllChr = (std::string) fillchar;

		//if(fllChr.length() != 1)
		//	THROW("The fill character must be exactly one character long");
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;

		int fillspaces;

		for (fillspaces = wid - data.length(); fillspaces > 1; fillspaces -= 2)
			data = fllChr + data + fllChr;

		return (fillspaces == 1) ? (data += fillchar) : data;
		END_FUN(center);

		//tested
		FUN_DEF(count);
		PARAM(self, );
		PARAM(sub, );
		PARAM(start, 0);
		PARAM(end, None);
		auto subStr = (std::string) sub;

		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		if (is_ofType(end, None))
			end = data.length();
		//if (not is_ofType(sub, str("s")))
		//	THROW("Must be of type str");
		int startInt = (int)start;
		int pos = data.find(subStr, startInt);
		int counter = 0;
		int endPos = (int)end;
		int nextStartingPos;
		while (((pos + subStr.length() - 1) < endPos) && pos != -1)
		{
			counter++;
			nextStartingPos = pos + 1;
			pos = data.find(subStr, nextStartingPos);
		}
		return counter;
		END_FUN(count);

		//tested
		FUN_DEF(endswith);
		PARAM(self, );
		PARAM(ending, );
		PARAM(start, 0);
		PARAM(end, None);
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;


		if (is_ofType(end, None))
			end = data.length();
		if (!is_ofType(ending, tuple()))
			ending = tuple({ ending });
		
		int endPos = (int)end;
		int startPos = (int)start;
		pyStr dataSub = data.substr(startPos, (endPos - startPos));

		//if ((not is_ofType(ending, str("s"))) && not is_ofType(ending, tuple({})))
		//	THROW("TypeError: endswith first arg must be str or a tuple of str");


		for (auto elem : ending)
		{
			std::string elem_str = (std::string)elem;
			if (dataSub.endswith(elem_str))
				return True;
		}

		return false;
		END_FUN(endswith);

		//tested
		FUN_DEF(expandtabs);
		PARAM(self, );
		PARAM(tabSize, 8);
		//if (not is_ofType(tabSize, Int(tabSize)))
		//	THROW("integer value expected");
		int tabNum = (int)tabSize;
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		std::string spaceStr = "";

		int pos = data.find('\t');
		int prevPos = 0;
		int lastLineBreak = 0;
		int countFromPrevTab = 0;
		int nextTabPos = 0;
		int spaces = 0;
		while (pos != std::string::npos)
		{
			lastLineBreak = data.find_first_of('\n\r', prevPos);
			if (lastLineBreak < pos && lastLineBreak > -1)
				countFromPrevTab = 0;
			else
			    countFromPrevTab = (pos % tabNum);
			nextTabPos = (pos - countFromPrevTab) + tabNum;
			spaces = nextTabPos - pos;
			spaceStr = "";
			for (int i = 0; i < spaces; i++)
				spaceStr += " ";

			data.replace(pos, 1, spaceStr);
			prevPos = pos;
			pos = data.find('\t', prevPos);
		}

		return data;
		END_FUN(expandtabs);

		//tested
		FUN_DEF(find);
		PARAM(self, );
		PARAM(sub, );
		PARAM(start, 0);
		PARAM(end, None);
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;

		//if (not is_ofType(sub, str("s")))
		//	THROW("TypeError: sub must be of type string");

		if (is_ofType(end, None))
			end = data.length();

		int endPos = (int)end;
		int startPos = (int)start;
		std::string subStr = (std::string)sub;
		size_t findRes = data.substr(startPos, endPos - startPos).rfind(subStr, 0);
		return findRes == std::string::npos ? -1 : findRes;
		END_FUN(find);

		FUN_DEF(format);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;

		while (HAS_MORE_POS_PARAMS())
		{
			PARAM(next, );
			using namespace std;
			int placeHolders = MatchesAmount(data, "\\{.*?\\}");
			if (placeHolders > 0)
			{
				smatch currMatch;
				if (!regex_search(data, currMatch, regex("\\{(.*?)\\}")))
					throw "BUG";

				std::string nextToken = currMatch[1];
				std::string newS;
				//std::string valueStr = toString(object(next));
				std::string valueStr = (std::string)next;
				if (!nextToken.empty())
					//newS = regex_replace(data, regex("\\{" + nextToken + "\\}"), valueStr);
					data = regex_replace(data, regex("\\{" + nextToken + "\\}"), valueStr);
				else
					//newS = regex_replace(data, regex("\\{" + nextToken + "\\}"), valueStr, std::regex_constants::format_first_only);
					data = regex_replace(data, regex("\\{" + nextToken + "\\}"), valueStr, std::regex_constants::format_first_only);
			}
			else
				return data;
		}
		
		return data;

		END_FUN(format);

		//tested
		FUN_DEF(index);
		PARAM(self, );
		PARAM(sub, );
		PARAM(start, 0);
		PARAM(end, None);
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;

		//if (not is_ofType(sub, str("s")))
		//	THROW("TypeError: sub must be of type string");

		if (is_ofType(end, None))
			end = data.length();

		int endPos = (int)end;
		int startPos = (int)start;
		std::string subStr = (std::string)sub;
		size_t indexRes = data.substr(startPos, endPos - startPos).rfind(subStr, 0);
		if(indexRes == std::string::npos)
			THROW("ValueError: substring not found");
		return indexRes;
		END_FUN(index);

		//tested
		FUN_DEF(isalnum);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		return find_if(data.begin(), data.end(), [](char c) { return !(std::isalnum(c)); }) == data.end() && data.length() > 0;
		END_FUN(isalnum);

		//tested
		FUN_DEF(isalpha);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		return find_if(data.begin(), data.end(), [](char c) { return !(std::isalpha(c)); }) == data.end() && data.length() > 0;
		END_FUN(isalpha);

		//tested
		//TODO: test with a non-ascii string
		FUN_DEF(Isascii);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		for (int i = 0; i < data.length(); i++)
		{
			if (data[i] < 0 || data[i] >= 128)
				return false;
		}
		return true;
		END_FUN(Isascii);

		//tested
		FUN_DEF(isdecimal);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		bool retVal = find_if(data.begin(), data.end(), [](char c) { return !(std::isdigit(c)); }) == data.end() && data.length() > 0;
		return retVal;
		END_FUN(isdecimal);

		//TODO: for now just acts like isdecimal
		//https://www.includehelp.com/python/difference-between-string-isdecimal-isdigit-isnumeric-and-methods.aspx
		FUN_DEF(isdigit);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		bool retVal = find_if(data.begin(), data.end(), [](char c) { return !(std::isdigit(c)); }) == data.end() && data.length() > 0;
		return retVal;
		END_FUN(isdigit);

		//tested
		FUN_DEF(isidentifier);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;


		if (std::isdigit(data[0]))
			return false;

		for (int i = 0; i < data.length(); i++)
		{
			if (data[i] == ' ' || (!std::isalnum(data[i]) && data[i] != '_'))
				return false;
		}
		return true;
		END_FUN(isidentifier);

		//tested
		FUN_DEF(islower);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		bool retVal;
		return (find_if(data.begin(), data.end(), [](char c) { return (std::isalpha(c)) && !(std::islower(c)); }) == data.end()) &&
			std::any_of(data.begin(), data.end(), [](char c) { return (std::isalpha(c)); });
		END_FUN(islower);

		//TODO: for now just acts like isdecimal
		FUN_DEF(isnumeric);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		bool retVal = find_if(data.begin(), data.end(), [](char c) { return !(std::isdigit(c)); }) == data.end() && data.length() > 0;
		return retVal;
		END_FUN(isnumeric);

		//tested
		FUN_DEF(isprintable);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		return (find_if(data.begin(), data.end(), [](char c) { return !(std::isprint(c)); }) == data.end()) || (data.length() == 0);
		END_FUN(isprintable);

		//tested
		FUN_DEF(isspace);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		return find_if(data.begin(), data.end(), [](char c) { return !(std::isspace(c)); }) == data.end() && data.length() > 0;
		END_FUN(isspace);

		FUN_DEF(istitle);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;

		END_FUN(istitle);

		//tested
		FUN_DEF(isupper);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		return (find_if(data.begin(), data.end(), [](char c) { return (std::isalpha(c)) && !(std::isupper(c)); }) == data.end()) &&
			std::any_of(data.begin(), data.end(), [](char c) { return (std::isalpha(c)); });
		END_FUN(isupper);

		//tested
		FUN_DEF(join);
		PARAM(self, );
		PARAM(iterable, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		return meAsStr->join(iterable);
		END_FUN(join);

		//tested
		FUN_DEF(ljust);
		PARAM(self, );
		PARAM(width, );
		PARAM(fillchar, " ");
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		std::string addStr = "";
		//if (not is_ofType(width, Int(1)))
		//	THROW("integer value expected");
		int wid = (int)width;
		std::string fllChr = (std::string)fillchar;
		int fillWid = wid - data.length();
		for (int i = 0; i < fillWid; i++)
			addStr += fllChr;
		return data + addStr;
		END_FUN(ljust);

		//tested
		FUN_DEF(lower);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		return meAsStr->lower();
		END_FUN(lower);

		//tested
		FUN_DEF(lstrip);
		PARAM(self, );
		PARAM(chars, None)
			auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		//if (!is_ofType(chars, str("sss")) && !is_ofType(chars, None))
		//	THROW("TypeError: lstrip arg must be None or str");
		if (chars == None)
			chars = " ";
		std::string charsStr = (std::string)chars;
		return(data.substr(data.find_first_not_of(charsStr)));
		END_FUN(lstrip);

		
		//FUN_DEF(maketrans);
		//PARAM(self, );
		//PARAM(x, );
		//PARAM(y, None);
		//PARAM(z, None);
		//dict retDict = dict();
		////1 argument

		////2 arguments
		//if (is_ofType(z, None))
		//{
		//	std::string xstr = x;
		//	int xLen = xstr.length();
		//	std::string ystr = y;
		//	int yLen = ystr.length();
		//	if (xLen != yLen)
		//		THROW("ValueError: the first two maketrans arguments must have equal length");

		//	for (int i = 0; i < xLen; i++)
		//	{
		//		char x = xstr[i];
		//		char y = ystr[i];

		//	}

		//}

		////3 arguments
		//return retDict;
		//END_FUN(maketrans);

		//tested
		FUN_DEF(partition);
		PARAM(self, );
		PARAM(sep, );
		//if (not is_ofType(sep, str("sss")))
		//	THROW("TypeError: must be str");
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		std::string sepstr = (std::string)sep;
		int sepPos = data.find(sepstr);

		std::vector<std::string> v = { data, "", "" };
		return sepPos == std::string::npos ? tuple({ data, "", "" }) :
			tuple({ data.substr(0, sepPos), sepstr, data.substr(sepPos + 1) });
		END_FUN(partition);

		//tested
		FUN_DEF(replace);
		PARAM(self, );
		PARAM(old, );
		PARAM(New, );
		PARAM(count, None);
		//check parameters
		/*if (not is_ofType(old, str("sss")) || not is_ofType(New, str("sss")))
			THROW("TypeError: argument must be str");
		if (not is_ofType(count, Int(1)))
			THROW("third argument must be an integer");*/

		int cnt = (int)count;
		std::string what = (std::string)old;
		std::string withWhat = (std::string)New;
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;

		int occurencesToReplace = ((is_ofType(count, None)) ? data.length(): cnt);
		int oldPos;
		int pos = data.find(what);
		for (int i = 0; i < occurencesToReplace && pos != std::string::npos; i++)
		{
			data.replace(pos, what.length(), withWhat);
			oldPos = pos;
			pos = data.find(what, oldPos);
		}
		return data;
		END_FUN(replace);


		//tested
		FUN_DEF(rfind);
		PARAM(self, );
		PARAM(sub, );
		PARAM(start, 0);
		PARAM(end, None);
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;

		//if (not is_ofType(sub, str("s")))
		//	THROW("TypeError: sub must be of type string");

		if (is_ofType(end, None))
			end = data.length();

		int endPos = (int)end;
		int startPos = (int)start;
		std::string subStr = (std::string)sub;
		size_t rfindRes = data.substr(startPos, endPos - startPos).rfind(subStr, endPos);
		return rfindRes == std::string::npos ? -1 : rfindRes;
		END_FUN(rfind);

		//tested
		FUN_DEF(rindex);
		PARAM(self, );
		PARAM(sub, );
		PARAM(start, 0);
		PARAM(end, None);
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;

		//if (not is_ofType(sub, str("s")))
		//	THROW("TypeError: sub must be of type string");

		if (is_ofType(end, None))
			end = data.length();

		int endPos = (int)end;
		int startPos = (int)start;
		std::string subStr = (std::string)sub;
		size_t rindexRes = data.substr(startPos, endPos - startPos).rfind(subStr, endPos);
		if (rindexRes == std::string::npos)
			THROW("ValueError: substring not found");
		return rindexRes;
		END_FUN(rindex);

		//tested
		FUN_DEF(rjust);
		PARAM(self, );
		PARAM(width, );
		PARAM(fillchar, " ");
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		std::string addStr = "";
		//if (not is_ofType(width, Int(1)))
		//	THROW("integer value expected");
		int wid = (int)width;
		std::string fllChr = (std::string)fillchar;
		int fillWid = wid - data.length();
		for (int i = 0; i < fillWid; i++)
			addStr += fllChr;
		return addStr + data;
		END_FUN(rjust);

		//tested
		FUN_DEF(rpartition);
		PARAM(self, );
		PARAM(sep, );
		//if (not is_ofType(sep, str("sss")))
		//	THROW("TypeError: must be str");
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		std::string sepStr = (std::string)sep;
		int sepPos = data.rfind(sepStr);
		return sepPos == std::string::npos ? tuple({ data, "", "" }) :
			tuple({ data.substr(0, sepPos), sep, data.substr(sepPos + 1) });
		END_FUN(rpartition);

		FUN_DEF(rsplit);
		PARAM(self, );
		END_FUN(rsplit);

		//tested
		FUN_DEF(rstrip);
		PARAM(self, );
		PARAM(chars, None);
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		//if (!is_ofType(chars, str("sss")) && !is_ofType(chars, None))
		//    THROW("TypeError: rstrip arg must be None or str");
		if (chars == None)
			chars = " ";
		std::string charsStr = (std::string)chars;
		std::string retStr = data;
		int pos = retStr.rfind(charsStr);
		int diff = retStr.length() - charsStr.length();
		while (pos == (diff) && pos != std::string::npos)
		{
			retStr = retStr.substr(0, retStr.rfind(charsStr));
			pos = retStr.rfind(charsStr);
			diff = retStr.length() - charsStr.length();
		}
		     
		return retStr;
		END_FUN(rstrip);

		//tested
		FUN_DEF(split);
		PARAM(self, );
		PARAM(sep, " ");
		PARAM(maxsplit, -1);
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		std::string sepStr = (std::string)sep;
		int max = (int)maxsplit;
		if (max == -1)
			max = data.length();
		auto parts = meAsStr->split(sepStr, max);
		std::vector<object> result;
		for (auto& p : parts)
			result.push_back(p);
		return list(result);
		END_FUN(split);

		//tested
		FUN_DEF(splitlines);
		PARAM(self, );
		PARAM(keepends, False);
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		bool keependsBool = (bool)keepends;
		std::string whatToSearchFor = "\n\r";
		int prevPos = -1;
		int pos = data.find_first_of(whatToSearchFor);
		auto retList = list({});
		while (pos != std::string::npos)
		{
				if(keependsBool)
					MEM_FUN(retList, append).A(data.substr(prevPos + 1, pos - prevPos)));
				else
				{
					std::string str = data.substr(prevPos + 1, (pos - prevPos) - 1);
					MEM_FUN(retList, append).A(str));
				}
			 
			
			prevPos = pos;
			pos = data.find_first_of(whatToSearchFor, prevPos + 1);
		}

		if (!retList)
		{
			MEM_FUN(retList, append).A(data));
		}
		else
			MEM_FUN(retList, append).A(data.substr(prevPos + 1)));
		return retList;
		END_FUN(splitlines);

		//tested
		FUN_DEF(startswith);
		PARAM(self, );
		PARAM(ending, );
		PARAM(start, 0);
		PARAM(end, None);
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;


		if (is_ofType(end, None))
			end = data.length();
		if (!is_ofType(ending, tuple()))
			ending = tuple({ ending });

		int endPos = (int)end;
		int startPos = (int)start;
		std::string dataSub = data.substr(startPos, (endPos - startPos));

		//if ((not is_ofType(ending, str("s"))) && not is_ofType(ending, tuple({})))
		//	THROW("TypeError: endswith first arg must be str or a tuple of str");


		for (auto elem : ending)
		{
			std::string elem_str = (std::string)elem;
			if (dataSub.rfind(elem_str, 0) == 0)
				return True;
		}

		return false;
		END_FUN(startswith);

		//tested
		FUN_DEF(strip);
		PARAM(self, );
		PARAM(chars, None);
		if (is_ofType(chars, None))
			chars = " ";
		std::string charStr = (std::string)chars;
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		std::string retStr = MEM_FUN(self, lstrip).A(chars));
		return MEM_FUN(str(retStr), rstrip).A(chars));
		END_FUN(strip);

		//tested
		FUN_DEF(swapcase);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		std::string retStr = "";
		int len = data.length();

		for (int i = 0; i < len; i++)
		{
			if (std::isupper(data[i]))
				retStr += std::tolower(data[i]);
			else if (std::islower(data[i]))
				retStr += std::toupper(data[i]);
			else
				retStr += data[i];
		}
		return retStr;
		END_FUN(swapcase);

		//tested
		FUN_DEF(title);
		PARAM(self, );
		std::string retStr = "";
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		auto splitList = MEM_FUN(str(data), split));
		for (auto elem : splitList)
		{
			std::string elemStr = (std::string)elem;
			std::string innerStr;
			innerStr += std::toupper(elemStr[0]);
			for (int i = 1; i < elemStr.length(); i++)
			{
				innerStr += std::tolower(elemStr[i]);
			}
			retStr += innerStr + " ";
		}
 		std::string ret = retStr.substr(0, retStr.size() - 1);
		return ret;
		END_FUN(title);

		//tested
		FUN_DEF(upper);
		PARAM(self, );
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		std::transform(data.begin(), data.end(), data.begin(),
			[](unsigned char c) { return std::toupper(c); });
		return data;
		END_FUN(upper);

		//tested
		FUN_DEF(zfill);
		PARAM(self, );
		PARAM(width, 0);
		//if (not is_ofType(width, Int(3)))
		//	THROW("TypeError: must be an integer");
		int finalWidth = (int)width;
		auto meAsStr = dynamic_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		if (finalWidth > data.length())
			data.insert(0, finalWidth - data.length(), '0');
		return data;
		END_FUN(zfill);
	}
}

namespace py
{
	void str::AddAttributes()
	{
		(*this).attr(capitalize) = py_str::capitalize;
		(*this).attr(casefold) = py_str::casefold;
		(*this).attr(center) = py_str::center;
		(*this).attr(count) = py_str::count;
		(*this).attr(endswith) = py_str::endswith;
		(*this).attr(expandtabs) = py_str::expandtabs;
		(*this).attr(find) = py_str::find;
		(*this).attr(format) = py_str::format;
		(*this).attr(index) = py_str::index;
		(*this).attr(isalnum) = py_str::isalnum;
		(*this).attr(isalpha) = py_str::isalpha;
		(*this).attr(Isascii) = py_str::Isascii;
		(*this).attr(isdecimal) = py_str::isdecimal;
		(*this).attr(isdigit) = py_str::isdigit;
		(*this).attr(isidentifier) = py_str::isidentifier;
		(*this).attr(islower) = py_str::islower;
		(*this).attr(isnumeric) = py_str::isnumeric;
		(*this).attr(isprintable) = py_str::isprintable;
		(*this).attr(isspace) = py_str::isspace;
		(*this).attr(istitle) = py_str::istitle;
		(*this).attr(isupper) = py_str::isupper;
		(*this).attr(join) = py_str::join;
		(*this).attr(ljust) = py_str::ljust;
		(*this).attr(lower) = py_str::lower;
		(*this).attr(lstrip) = py_str::lstrip;
		(*this).attr(partition) = py_str::partition;
		(*this).attr(replace) = py_str::replace;
		(*this).attr(rfind) = py_str::rfind;
		(*this).attr(rindex) = py_str::rindex;
		(*this).attr(rjust) = py_str::rjust;
		(*this).attr(rpartition) = py_str::rpartition;
		(*this).attr(rsplit) = py_str::rsplit;
		(*this).attr(rstrip) = py_str::rstrip;
		(*this).attr(split) = py_str::split;
		(*this).attr(splitlines) = py_str::splitlines;
		(*this).attr(startswith) = py_str::startswith;
		(*this).attr(strip) = py_str::strip;
		(*this).attr(swapcase) = py_str::swapcase;
		(*this).attr(title) = py_str::title;
		(*this).attr(upper) = py_str::upper;
		(*this).attr(zfill) = py_str::zfill;

	}

	str::str() : object(pyStr())
	{
		AddAttributes();
	}

	str::str(const char* cstr) : object(pyStr(cstr))
	{
		AddAttributes();
	}


	str::str(std::string const& stdStr) : object(pyStr(stdStr))
	{
		AddAttributes();
	}
}