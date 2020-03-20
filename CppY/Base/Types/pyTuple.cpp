#include "pyTuple.h"
namespace py
{
	pyTuple::pyTuple() :pyList()
	{
	}

	pyTuple::pyTuple(std::vector<object> const& values)
		:pyList(values)
	{
	}

	pyTuple::pyTuple(std::pair<object, object> const& values)
		: pyList({ values.first,values.second })
	{
	}

	std::string pyTuple::Type() const
	{
		return "Tuple";
	}

	pyObjPtr pyTuple::Clone() const
	{
		return pyObjPtr(new pyTuple(_impl));
	}

	bool pyTuple::operator < (pyObj const& other) const
	{
		EXPLICIT_COMPARE(Type(), other.Type());
		pyTuple const* otherPtr = dynamic_cast<pyTuple const*>(&other);
		return pyList::operator <(other);
	}

}