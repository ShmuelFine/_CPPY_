#pragma once

#include "object.h"

namespace py
{
	class list : public object
	{
	public:
		list();
		list(std::vector<object> const& v);

	protected:
		void AddAttributes();
	};


}