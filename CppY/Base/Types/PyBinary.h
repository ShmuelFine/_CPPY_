#pragma once

#include "PyObj.h"
#include "pyNums.h"
#include "pyList.h"
#include <vector>
#include <set>

namespace py
{
	class pyBinary : public pyList
	{
	public:
		pyBinary() : pyList() {};
		pyBinary(std::initializer_list<unsigned char> const& v)
		{
			for (auto elem : v) _impl.push_back(new pyByte(elem));
			for (pyObjPtr& i : _impl) i.IsAssignable = false;

		}

		pyBinary(std::vector<unsigned char> const& v)
		{
			for (auto elem : v) _impl.push_back(new pyByte(elem));
			for (pyObjPtr& i : _impl) i.IsAssignable = false;
		}
		//pyBinary(std::string const& v)
		//{
		//	for (auto elem : v) _impl.push_back(new pyBinary(elem));
		//	for (pyObjPtr& i : _impl) i.IsAssignable = false;
		//}

		// copy ctors
		pyBinary(pyBinary const& other) : pyList(other) {};
		
		virtual std::string Type() const override;
		std::string hex();
		std::string decode(std::string encoding, std::string errors);

	public:
		// Shouldn't be used. if the other is a list, use As<list>
		pyBinary(pyObjPtr const& other) = delete;
	};

	}
