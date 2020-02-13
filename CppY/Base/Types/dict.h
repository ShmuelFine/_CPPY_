#pragma once

#include "object.h"

namespace py
{
	class dict : public object
	{
	public:
		dict();
		dict(std::map<object, object> const& m);

	protected:
		void AddAttributes();
	};


}