#pragma once


#include "PyBinary.h"

namespace py
{
	class pyByteArray : public pyBinary
	{
	public:
		pyByteArray(std::initializer_list<unsigned char> const& v)
		{
			for (auto elem : v) _impl.push_back(new pyByte(elem));
			for (pyObjPtr& i : _impl) i.IsAssignable = true;
		}

		pyByteArray(std::vector<unsigned char> const& v)
		{
			for (auto elem : v) _impl.push_back(new pyByte(elem));
			for (pyObjPtr& i : _impl) i.IsAssignable = true;
		}
	};


}
