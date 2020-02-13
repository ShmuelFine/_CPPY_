//class not needed yet 
#pragma once

#include "PyBinary.h"

namespace py
{
	class pyBytes : public pyBinary
	{
	public:
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

		
	};


}