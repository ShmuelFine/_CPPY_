#pragma once
#include "object.h"

namespace py
{
	class Complex : public object
	{
	public:
		Complex(object const & o);

	protected:
		void AddAttributes();

	};


}