#pragma once

#include "object.h"
#include <string>

namespace py
{
	class str : public object
	{
	public:
		str();
		str(const char* cstr);
		str(std::string const& stdStr);

	protected:
		void AddAttributes();
	};


}
