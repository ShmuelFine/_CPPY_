#pragma once

#include "pyBytes.h"
#include "object.h"

namespace py
{
	class bytes : public object
	{
	public:
		bytes();
		//bytes(unsigned char const& o);
		bytes(std::vector<unsigned char> const& v);
		bytes(std::string const& v);

	protected:
		void AddAttributes();
	};

}