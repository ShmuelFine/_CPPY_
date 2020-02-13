#include "pyStr.h"
#include "pyNums.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <sstream>
#include <stdarg.h>
#include <regex>

#include "pyExceptions.h"

namespace py
{
	using namespace std;

	std::string pyStr::Type() const
	{
		return "str";
	}

	bool pyStr::operator<(pyObj const& other) const
	{
		if (Type() != other.Type())
			return Type() < other.Type();
		pyStr const * otherPtr = reinterpret_cast<pyStr const *>(&other);
		return (string)_impl < (string)*otherPtr;
	}

	pyStr::operator std::string() const
	{
		return _impl;
	}

	pyStr::operator double() const
	{
		char * p;
		double converted = strtod(_impl.c_str(), &p);
		if (*p) {
			THROW(pyStr("{} is not a number").format(_impl));
		}
		else {
			return converted;
		}
	}

	pyStr::operator float() const
	{
		char* p;
		float converted = strtof(_impl.c_str(), &p);
		if (*p) {
			THROW(pyStr("{} is not a number").format(_impl));
		}
		else {
			return converted;
		}
	}

	pyStr::operator int() const
	{
		// TODO: memory leak in strtol - p is not freed
		char* p;
		int converted = (int)strtol(_impl.c_str(), &p, 10);
		if (*p) {
			THROW(pyStr("{} is not a number").format(_impl));
		}
		else {
			return converted;
		}
	}

	pyStr::operator bool() const
	{
		return !_impl.empty();
	}

	pyObjPtr & pyStr::operator [](pyObjPtr const & key)
	{
		NOT_IMPL; // for implementing we should add ordinary operator [] that returns ptr without ref.
		//return FetchByIdx(key->operator int());
	}

	pyObjPtr pyStr::FetchByIdx(int idx) const
	{
		char theChar = _impl.operator[](idx);
		pyStr result = std::string(1, theChar);
		return result.Clone();
	}


	pyObjIterator pyStr::begin() const
	{
		return pyObjIterator((pyObj*)this, 0);
	}

	pyObjIterator pyStr::end() const
	{
		return pyObjIterator((pyObj*)this, (int)_impl.size());
	}




	pyStr::pyStr(const char* cstr)
		:_impl(cstr)
	{
	}

	pyStr::pyStr(std::string const& pyStr)
		: _impl(pyStr)
	{
	}

	pyObjPtr pyStr::Clone() const
	{
		return make_shared<pyStr>(_impl);
	}


	pyObj& pyStr::operator+=(pyObj const& other)
	{
		if (Type() != other.Type())
			NOT_SUPPORTED_PAIR(other);

		_impl += (std::string)other;
		return *this;
	}

	pyObjPtr pyStr::operator+(pyObj const& other) const
	{
		pyStr result = _impl;
		result += other;
		return result.Clone();
	}

	pyStr pyStr::lower()
	{
		std::string data = _impl;
		std::transform(data.begin(), data.end(), data.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return data;
	}

	bool pyStr::endswith(pyStr const& ending)
	{
		if (ending._impl.size() > this->_impl.size()) return false;
		return std::equal(ending._impl.rbegin(), ending._impl.rend(), _impl.rbegin());
	}

	//pyStr pyStr::join(pyList const& parts)
	//{
	//	string retStr = "";
	//	for (int i = 0; i < parts._impl.size() - 1; i++)
	//	{
	//		retStr = retStr + parts[i]->operator std::string() + _impl;
	//	}
	//	retStr = retStr + parts[parts._impl.size() - 1]->operator std::string();
	//
	//	return retStr;
	//}


	pyStr pyStr::replace(const char& what, const char& withWhat)
	{
		auto pyStr = _impl;
		std::replace(pyStr.begin(), pyStr.end(), what, withWhat);
		return pyStr;
	}

	std::vector<std::string> pyStr::chars()
	{
		std::vector<std::string> result;
		for (auto ch : _impl)
			result.push_back(std::string(1,ch));
		
		return result;
	}

	pyStr pyStr::strip()
	{
		std::string _str = _impl;
		std::string::const_iterator it = _str.begin();
		while (it != _str.end() && isspace(*it))
		{
			it++;
		}

		std::string::const_reverse_iterator rit = _str.rbegin();
		while (rit.base() != it && isspace(*rit))
		{
			rit++;
		}

		return std::string(it, rit.base());
	}

	std::vector<std::string> pyStr::split(std::string const& delim, int maxsplit)
	{
		// https://stackoverflow.com/questions/27927913/find-index-of-first-match-using-c-regex
		vector<string> splits;
		smatch m;
		string s = _impl;
		auto r = regex(delim);

		while (regex_search(s, m, r))  
		{
			int split_on = (int) m.position();
			splits.push_back(s.substr(0, split_on));
			s = s.substr(split_on + m.length());
			if (splits.size() == maxsplit)
				break;
		}

		if (!s.empty()) {
			splits.push_back(s);
		}

		return splits;
	}
}

namespace py
{
	using namespace std;

	int MatchesAmount(std::string const& s, std::string const& re)
	{
		regex words_regex(re);
		auto words_begin = sregex_iterator(s.begin(), s.end(), words_regex);
		auto words_end = sregex_iterator();
		return (int) distance(words_begin, words_end);
	}

	std::string toString(bool const& t)
	{
		return t ? "True" : "False";
	}

	std::string toString(int const& t)
	{
		return std::to_string(t);
	}

	std::string toString(double const& t)
	{
		return std::to_string(t);
	}

	std::string toString(const char* t) {
		return t;
	}

	std::string toString(const char& t) {
		return std::string(1, t);
	}

	std::string toString(const std::string& t) {
		return t;
	}

	std::string toString(const py::pyObj& t)
	{
		return (string)t;
	}

	//std::string toString(const std::shared_ptr<py::pyObj>& t)
	//{
	//	return toString(*t);
	//}

	std::string toString(const py::pyStr& t)
	{
		return t;
	}

	std::string pyFormat(std::string const & s)
	{
		return s;
	}

}