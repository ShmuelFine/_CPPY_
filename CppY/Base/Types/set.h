#pragma once
#include "object.h"

namespace py
{
	class set : public object
	{
	public:
		set();
		set(std::vector<object> const& s);

	protected:
		void AddAttributes();
	};


}

