#include "pyNone.h"
#include "pyExceptions.h"
#include <cmath>

namespace py
{
	pyNone::operator std::string() const {
		return "None";
	}

	pyNone::operator double() const
	{
		return NAN;// NOT_IMPL;
	}

	pyNone::operator float() const
	{
		return NAN;// NOT_IMPL;
	}

	pyNone::operator int() const
	{
		return (int)NAN;// NOT_IMPL;
	}

	pyNone::operator bool() const
	{
		return false;
	}


	std::string pyNone::Type() const
	{
		return "None";
	}

	bool pyNone::operator<(pyObj const& other) const
	{
		// See object operator == for understanding the following:

		// Be equal with other None,
		if (other.Type() == Type())
			return false;
		// but distinct from any other type.
		return true;
	}

	pyObjPtr pyNone::Clone() const
	{
		return std::shared_ptr<pyObj>(new pyNone());
	}
}