#pragma once

#include "pyBytes.h"
#include "object.h"

namespace py
{
	class bytes : public object
	{
	public:
		bytes();
		bytes(object const & other);
		bytes(std::vector<unsigned char> const& v);
		bytes(std::string const& v);
		bytes(const char * v_str);

	protected:
		void AddAttributes();
	};

}