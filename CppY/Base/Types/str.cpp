#include "str.h"
#include "tuple.h"
#include "dict.h"
#include "FunDefs.h"
#include "Comparisons.h"
#include "Int.h"
#include <locale>
#include <codecvt>
#include <algorithm>
namespace py
{

	namespace py_str
	{
		//tested
		FUN_DEF(capitalize);
		PARAM(self, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
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
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		return meAsStr->lower();
		END_FUN(casefold);

		//tested
		//TODO: make it work if the fill character is a char (currently only works if it is a string)
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
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
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

		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
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

		FUN_DEF(encode);
		PARAM(self, );
		PARAM(encoding, "utf-8");
		PARAM(errors, "strict");
		END_FUN(encode);

		
		FUN_DEF(endswith);
		PARAM(self, );
		PARAM(ending, );
		PARAM(start, 0);
		PARAM(end, None);
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;

		if (is_ofType(end, None))
			end = data.length() - 1;
		if (!is_ofType(ending, tuple()))
			tuple ending = tuple({ ending });
			
			
		//if ((not is_ofType(ending, str("s"))) && not is_ofType(ending, tuple({})))
		//	THROW("TypeError: endswith first arg must be str or a tuple of str");
		int endPos = (int)end;
		int startPos = (int)start;
		auto endingAsTuple = reinterpret_cast<pyTuple*>(ending._ptr.get());

		std::string dataSub = data.substr(startPos, (endPos - startPos) + 1);
		//_imple.size() = 1095?
		for (int i = 0; i < endingAsTuple->_impl.size(); i++)
		{
			//this throws
			//std::string endingStr = (std::string)ending[i];
			std::string endingStr = (std::string)endingAsTuple[i];
			std::size_t foundPos= dataSub.rfind(endingStr);
			
			//if (dataSub.rfind(endingStr) == (dataSub.length() - endingStr.length()))
			if (foundPos == (dataSub.length() - endingStr.length()))
				return true;
		}
		return false;
		END_FUN(endswith);

		//https://www.programiz.com/python-programming/methods/string/expandtabs
		//redo based on above understanding
		FUN_DEF(expandtabs);
		PARAM(self, );
		PARAM(tabSize, 8);
		//if (not is_ofType(tabSize, Int(tabSize)))
		//	THROW("integer value expected");
		int tabNum = (int)tabSize;
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		std::string tabStr = "";
		for (int i = 0; i < tabNum; i++)
			tabStr += " ";

		int pos = data.find('\t');

		// Repeat until end is reached
		while (pos != std::string::npos)
		{
			// Replace this occurrence of Sub String
			data.replace(pos, 1, tabStr);
			// Get the next occurrence from the current position
			pos = data.find(('\t'), pos + tabStr.size());
		}
		return data;
		END_FUN(expandtabs);


		FUN_DEF(find);
		PARAM(self, );
		END_FUN(find);

		FUN_DEF(format);
		PARAM(self, );
		END_FUN(format);

		FUN_DEF(format_map);
		PARAM(self, );
		END_FUN(format_map);

		FUN_DEF(index);
		PARAM(self, );
		END_FUN(index);

		//tested
		FUN_DEF(isalnum);
		PARAM(self, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		return find_if(data.begin(), data.end(), [](char c) { return !(std::isalnum(c)); }) == data.end() && data.length() > 0;
		END_FUN(isalnum);

		//tested
		FUN_DEF(isalpha);
		PARAM(self, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		return find_if(data.begin(), data.end(), [](char c) { return !(std::isalpha(c)); }) == data.end() && data.length() > 0;
		END_FUN(isalpha);

		//tested
		//TODO: test with a non-ascii string
		FUN_DEF(Isascii);
		PARAM(self, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
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
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		bool retVal = find_if(data.begin(), data.end(), [](char c) { return !(std::isdigit(c)); }) == data.end() && data.length() > 0;
		return retVal;
		END_FUN(isdecimal);

		//TODO
		//allows exponents- just has to be a digit, not necessarily decimal
		//https://www.includehelp.com/python/difference-between-string-isdecimal-isdigit-isnumeric-and-methods.aspx
		FUN_DEF(isdigit);
		PARAM(self, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		return find_if(data.begin(), data.end(), [](char c) { return !(std::isdigit(c)); }) == data.end() && data.length() > 0;
		END_FUN(isdigit);

		//tested
		FUN_DEF(isidentifier);
		PARAM(self, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
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
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		bool retVal;
		return (find_if(data.begin(), data.end(), [](char c) { return (std::isalpha(c)) && !(std::islower(c)); }) == data.end()) &&
			std::any_of(data.begin(), data.end(), [](char c) { return (std::isalpha(c)); });
		END_FUN(islower);

		//TODO
		//allows fractions, etc.- just has to be numeric, not necessarily decimal
		//https://www.includehelp.com/python/difference-between-string-isdecimal-isdigit-isnumeric-and-methods.aspx
		FUN_DEF(isnumeric);
		PARAM(self, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		return find_if(data.begin(), data.end(), [](char c) { return !(std::isdigit(c)); }) == data.end() && data.length() > 0;
		END_FUN(isnumeric);

		//tested
		FUN_DEF(isprintable);
		PARAM(self, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		return (find_if(data.begin(), data.end(), [](char c) { return !(std::isprint(c)); }) == data.end()) || (data.length() == 0);
		END_FUN(isprintable);

		//tested
		FUN_DEF(isspace);
		PARAM(self, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		return find_if(data.begin(), data.end(), [](char c) { return !(std::isspace(c)); }) == data.end() && data.length() > 0;
		END_FUN(isspace);

		FUN_DEF(istitle);
		PARAM(self, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;

		END_FUN(istitle);

		//tested
		FUN_DEF(isupper);
		PARAM(self, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		return (find_if(data.begin(), data.end(), [](char c) { return (std::isalpha(c)) && !(std::isupper(c)); }) == data.end()) &&
			std::any_of(data.begin(), data.end(), [](char c) { return (std::isalpha(c)); });
		END_FUN(isupper);

		//how to convert from iterable wrapped in object to regular iterable
		FUN_DEF(join);
		PARAM(self, );
		PARAM(iterable, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		return meAsStr->join(iterable);
		END_FUN(join);

		//tested
		FUN_DEF(ljust);
		PARAM(self, );
		PARAM(width, );
		PARAM(fillchar, " ");
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
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
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		return meAsStr->lower();
		END_FUN(lower);

		//tested
		FUN_DEF(lstrip);
		PARAM(self, );
		PARAM(chars, None)
			auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		//if (!is_ofType(chars, str("sss")) && !is_ofType(chars, None))
		//	THROW("TypeError: lstrip arg must be None or str");
		if (chars == None)
			chars = " ";
		std::string charsStr = (std::string)chars;
		return(data.substr(data.find_first_not_of(chars)));
		END_FUN(lstrip);

		//in process-uses some encoding
		FUN_DEF(maketrans);
		PARAM(self, );
		PARAM(x, );
		PARAM(y, None);
		PARAM(z, None);
		dict retDict;
		if (is_ofType(z, None))
		{
			std::string xstr = x;
			int xLen = xstr.length();
			std::string ystr = y;
			int yLen = ystr.length();
			if (xLen != yLen)
				THROW("ValueError: the first two maketrans arguments must have equal length");

			for (int i = 0; i < xLen; i++)
			{
				char x = xstr[i];
				char y = ystr[i];

			}

		}
		return retDict;
		END_FUN(maketrans);

		
		FUN_DEF(partition);
		PARAM(self, );
		PARAM(sep, );
		//if (not is_ofType(sep, str("sss")))
		//	THROW("TypeError: must be str");
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
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
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
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
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
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
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
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
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
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

		
		FUN_DEF(rpartition);
		PARAM(self, );
		PARAM(sep, );
		//if (not is_ofType(sep, str("sss")))
		//	THROW("TypeError: must be str");
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		std::string sepStr = (std::string)sep;
		int sepPos = data.find_last_of(sepStr);
		return sepPos == std::string::npos ? tuple({ data, "", "" }) :
			tuple({ data.substr(0, sepPos), sep, data.substr(sepPos + 1) });
		END_FUN(rpartition);

		FUN_DEF(rsplit);
		PARAM(self, );
		END_FUN(rsplit);

		//tested
		FUN_DEF(rstrip);
		PARAM(self, );
		PARAM(chars, None)
			auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		if (!is_ofType(chars, str("sss")) && !is_ofType(chars, None))
		    THROW("TypeError: rstrip arg must be None or str");
		if (chars == None)
			chars = " ";
		std::string charsStr = (std::string)chars;
		return(data.substr(0, data.find_last_not_of(chars) + 1));
		END_FUN(rstrip);

		FUN_DEF(split);
		PARAM(self, );
		END_FUN(split);

		FUN_DEF(splitlines);
		PARAM(self, );
		END_FUN(splitlines);

		FUN_DEF(startswith);
		PARAM(self, );
		END_FUN(startswith);

		FUN_DEF(strip);
		PARAM(self, );
		END_FUN(strip);

		FUN_DEF(swapcase);
		PARAM(self, );
		END_FUN(swapcase);

		FUN_DEF(title);
		PARAM(self, );
		END_FUN(title);

		FUN_DEF(translate);
		PARAM(self, );
		END_FUN(translate);

		//tested
		FUN_DEF(upper);
		PARAM(self, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
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
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		std::string data = meAsStr->_impl;
		if (finalWidth > data.length())
			data.insert(0, finalWidth - data.length(), '0');
		return data;
		END_FUN(zfill);



		//	FUN_DEF(format);
		//	PARAM(self, );
		//	END_FUN(format);

		//	FUN_DEF(strip);
		//	PARAM(self, );

		//	END_FUN(strip);

		//	FUN_DEF(split);
		//	PARAM(self, );

		//	END_FUN(split);

		//	FUN_DEF(replace);
		//	PARAM(self, );

		//	END_FUN(replace);
		//	

		//	FUN_DEF();
		//	PARAM(self, );

		//	END_FUN();
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
		//(*this).attr(encode) = py_str::encode;
		(*this).attr(endswith) = py_str::endswith;
		(*this).attr(expandtabs) = py_str::expandtabs;
		(*this).attr(find) = py_str::find;
		(*this).attr(format) = py_str::format;
		(*this).attr(format_map) = py_str::format_map;
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
		(*this).attr(maketrans) = py_str::maketrans;
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
		(*this).attr(translate) = py_str::translate;
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