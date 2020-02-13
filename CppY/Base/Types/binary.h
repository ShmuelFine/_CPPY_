#pragma once

#include "pyBinary.h"
#include "object.h"

namespace py
{
	class binary : public object
	{
	public:
		binary();
		//Binary(unsigned char const& o);
		binary(std::vector<unsigned char> const& v);


	protected:
		void AddAttributes();
	};

}