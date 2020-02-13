#pragma once

#include "object.h"

namespace py
{
	class tuple : public object
	{
	public:
		tuple();
		tuple(std::vector<object> const& v);

	protected:
		void AddAttributes();
	};


}
