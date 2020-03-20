#pragma once

#include "object.h"
#include "PyObj.h"

namespace py
{
	class range : public object
	{
	public:
		range();
		range(int start, int stop, int step = 1);
		range(int stop);

	protected:
		void AddAttributes();
	};


}