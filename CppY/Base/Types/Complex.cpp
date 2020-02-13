#include "Complex.h"
#include "pyNums.h"

namespace py
{

	void Complex::AddAttributes()
	{

	}

	Complex::Complex(object const& o) : object(pyInt((int)o))
	{

	}

}