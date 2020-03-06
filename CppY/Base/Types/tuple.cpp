#include "tuple.h"
#include "FunDefs.h"
#include "Comparisons.h"

namespace py
{

	namespace py_tuple
	{
		FUN_DEF(count);
		PARAM(self, );
		PARAM(whatToCount, );
		int counter = 0;
		for (auto elem : self) {
			if (elem == whatToCount)
				counter++;
		}
		return counter;
		END_FUN(count);

		FUN_DEF(index);
		PARAM(self, );
		PARAM(value, );
		PARAM(start, None);
		PARAM(stop, None);
		auto meAsTuple = dynamic_cast<pyTuple*>(self._ptr.get());
		if (is_ofType(start, None))
			start = 0;
		if (is_ofType(stop, None))
			stop = meAsTuple->_impl.size();
		return meAsTuple->index(value, start, stop, true);
		END_FUN(index);
	}
}

namespace py
{
	void tuple::AddAttributes()
	{

		//doesn't really belong here but trying to DRY the code
		auto myTuple = dynamic_cast<pyTuple*>(get());

		for (pyObjPtr& elem : myTuple->_impl)
			elem.IsAssignable = false;

		(*this).attr(count) = py_tuple::count;
		(*this).attr(index) = py_tuple::index;
	}

	tuple::tuple() : object(pyTuple())
	{
		AddAttributes();
	}

	tuple::tuple(std::vector<object> const& v)
		: object(pyTuple(std::vector<pyObjPtr>(v.begin(), v.end())))
	{
		AddAttributes();
	}



}