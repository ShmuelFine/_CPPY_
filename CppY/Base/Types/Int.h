#pragma once
#include "object.h"

namespace py
{
	class Int : public object
	{
	public:
		Int(object const& o);
		Int(int i);

	protected:
		void AddAttributes();

	};


}