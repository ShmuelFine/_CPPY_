#pragma once

#include "PyObj.h"
#include "pyExceptions.h"
#include "pyFormat.hpp"
#include <string>
#include <regex>

namespace py
{
	class pyStr : public pyObj
	{
	public:
		std::string _impl;

	public:
		pyStr(const char* cstr);
		pyStr(std::string const& pyStr);
		pyStr() : _impl() {};
		
		// pyObj: //////////////////////////////////////////////////
		virtual std::string Type() const override;
		virtual operator std::string() const override;
		virtual operator double() const override;
		virtual operator float() const override;
		virtual operator int() const override;
		virtual operator bool() const override;

		virtual object & operator [](object const & key);
		virtual object FetchByIdx(int idx) const;
		virtual pyObjIterator begin() const;
		virtual pyObjIterator end() const;


		virtual object operator()(pyObj const& params) const { NOT_SUPPORTED; }

		virtual object Clone() const override;
		virtual bool operator <(pyObj const& other) const override;

		virtual object  operator ++(int) { NOT_SUPPORTED; };
		virtual pyObj& operator ++() { NOT_SUPPORTED; };
		virtual pyObj& operator += (pyObj const& other);
		virtual object  operator +(pyObj const& other)  const override;

		/////////////////////////////////////////////////////////////

		pyStr lower();
		bool endswith(pyStr const& ending);
		bool startswith(pyStr const& starts);

		template<typename Iterable>
		pyStr join(Iterable const& parts)
		{
			if (!parts)
				return "";

			std::string retStr = "";
			for (auto part : parts)
			{
				retStr = retStr + (std::string)(part) + (std::string)(*this);
			}
			retStr = std::string(retStr.begin(), retStr.end() - this->_impl.size());

			return retStr;
		}

		template<typename... Args>
		pyStr format(Args... args) const
		{
			return pyFormat(*this, args...);
		}

		//pyStr format(...);

		operator bool()
		{
			return !_impl.empty();
		}


		std::vector<std::string> chars();

		pyStr operator +(pyStr const& other)
		{
			return pyStr(std::string(*this) + std::string(other));
		}

		pyStr strip();
		std::vector<std::string> split(std::string const & delim = R"(\s+)", int maxsplit = -1);
		pyStr replace(const char& what, const char& withWhat);
		pyStr replace(std::string const& what, std::string const & withWhat);

	};
}

namespace py
{
	int MatchesAmount(std::string const& s, std::string const& re);

	//template<typename T>
	//std::string toString(const T& t) { return std::to_string(t); }
	std::string toString(bool const& t);
	std::string toString(int const& t);
	std::string toString(double const & t);
	std::string toString(const char* t);
	std::string toString(const char& t);
	std::string toString(const std::string& t);
	std::string toString(const py::pyStr & t);
	std::string toString(const py::pyObj& t);
	//std::string toString(const std::shared_ptr<py::pyObj> & t);

	std::string pyFormat(std::string const & s);

	std::string ReplaceAll(std::string const& orig, std::string const& pattern, std::string const& replacement, bool isOnlyFirst = false);
	

	template<typename T, typename... Args>
	std::string pyFormat(std::string const & s, T value, Args... args)
	{
		using namespace std;
		int placeHolders = MatchesAmount(s, "\\{.*?\\}");
		if (placeHolders > 0)
		{
			smatch currMatch;
			if (!regex_search(s, currMatch, regex("\\{(.*?)\\}")))
				throw "BUG";

			std::string nextToken = currMatch[1];
			std::string newS;
			std::string valueStr = toString(value);
			if (!nextToken.empty())
				newS = ReplaceAll(s, "{" + nextToken + "}", valueStr);
			else
				newS = ReplaceAll(s, "{" + nextToken + "}", valueStr, /*only first=*/true);

			return pyFormat(newS, args...);
		}
		else
			return s;
	}
}