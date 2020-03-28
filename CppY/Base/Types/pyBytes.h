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
	class pyBytes;
	typedef std::shared_ptr<pyBytes> pyBytesPtr;

	class pyBytes : public pyList
	{
	public:
		pyBytes() : pyList() {};
		pyBytes(std::initializer_list<unsigned char> const& v);
		pyBytes(std::vector<unsigned char> const& v);
		pyBytes(std::string const& v);
		pyBytes(pyBytes const& other) : pyList(other) {};

		// Overrides:
		virtual std::string Type() const override;
		virtual operator std::string() const override;
		virtual object Clone() const override;

	public:
		std::string hex() const;
		std::string decode(std::string encoding, std::string errors);
		bool endswith(pyBytes const& ending);
		bool startswith(pyBytes const& open);
		pyBytes replace(pyBytes const& what, pyBytes const& withWhat,int howMany);
		pyBytes addPading(int width, pyByte fillByte, std::string pos);
		pyTuple partition(pyByte const& delimitor);
		int rindex(pyBytes whoToSearch, int startIdx, int endIdx, bool throwEx);
		pyBytes strip(pyByte chars, std::string pos);


	public:
		// Shouldn't be used. if the other is a list, use As<list>
		pyBytes(object const& other) = delete;
	};

}
