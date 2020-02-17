#pragma once

#include "PyObj.h"
#include "pyStr.h"
#include "pyNums.h"
#include "pyList.h"
#include "pyTuple.h"
#include <vector>
#include <set>
#include <sstream>
#include <iomanip>
#include <iterator>

namespace py
{
	class pyBytes : public pyList
	{
	public:
		pyBytes() : pyList() {};
		pyBytes(std::initializer_list<unsigned char> const& v)
		{
			for (auto elem : v) _impl.push_back(new pyByte(elem));
			for (pyObjPtr& i : _impl) i.IsAssignable = false;

		}

		pyBytes(std::vector<unsigned char> const& v)
		{
			for (auto elem : v) _impl.push_back(new pyByte(elem));
			for (pyObjPtr& i : _impl) i.IsAssignable = false;
		}

		pyBytes(std::string const& v) {
			auto numStrs = pyStr(v).split(R"(\\x)");
			//std::vector<unsigned char> res;
			for (auto singleNumStr : numStrs)
			{
				std::stringstream ss;
				ss << std::hex << singleNumStr;
				int tst;
				ss >> tst;
				//res.push_back(tst);
				_impl.push_back(new pyByte(tst));
			}
			for (pyObjPtr& i : _impl) i.IsAssignable = false;
			//*this=pyBytes(res);
		}

		// copy ctors
		pyBytes(pyBytes const& other) : pyList(other) {};

		virtual std::string Type() const override;
		std::string hex();
		std::string decode(std::string encoding, std::string errors);
		bool endswith(pyBytes const& ending);
		bool startswith(pyBytes const& open);
		pyList replace(pyBytes const& what, pyBytes const& withWhat);
		pyBytes center(int width, pyByte fillByte);
		pyTuple partition(pyByte const& delimitor);

	public:
		// Shouldn't be used. if the other is a list, use As<list>
		pyBytes(pyObjPtr const& other) = delete;
	};

}
