#include "range.h"
#include "pyRange.h"
#include "FunDefs.h"
#include "Comparisons.h"
#include "Int.h"

namespace py
{
	
	namespace py_range
	{
		FUN_DEF(count);
		PARAM(self, );
		PARAM(whatToCount, );
		auto meAsRange = reinterpret_cast<pyRange*>(self._ptr.get());
		int start = meAsRange->_start;
		int step = meAsRange->_step;
		int stop = meAsRange->_stop;
		
		if (not is_ofType(whatToCount, Int(1)))
			return 0;

		int who = whatToCount;
		// check that "who" divides without reminder:
		bool isOK = start + ((who - start) / step) * step == who;
		// check that "who" is within range:
		double ratio = (double)(who - start) / (double)(stop - start);
		bool isInRange = 0 <= ratio and ratio <= 1;
		isOK = isOK and isInRange;
		return isOK ? 1 : 0;
		
		END_FUN(count);

		
		FUN_DEF(index);
		PARAM(self, );
		PARAM(value, );
		auto meAsRange = reinterpret_cast<pyRange*>(self._ptr.get());
		int step = meAsRange->_step;
		int start = meAsRange->_start;
		if (count(ARGS(self).A(value)) == 0)
			THROW("ValueError: value is not in range");
		else
			return ((value - start)/step);
		END_FUN(index);

	}
};

namespace py
{
	void range::AddAttributes()
	{
		int startVal = reinterpret_cast<pyRange*>(get())->_start;
		int stepVal = reinterpret_cast<pyRange*>(get())->_step;
		int stopVal = reinterpret_cast<pyRange*>(get())->_stop;
		(*this).attr(start) = pyInt(startVal);
		(*this).attr(start)._ptr.IsAssignable = false;
		(*this).attr(step) = pyInt(stepVal);
		(*this).attr(step)._ptr.IsAssignable = false;
		(*this).attr(stop) = pyInt(stopVal);
		(*this).attr(stop)._ptr.IsAssignable = false;
		(*this).attr(count) = py_range::count;
		(*this).attr(index) = py_range::index;
	}

	//TODO: If step is zero, ValueError is raised.
	range::range() : object(pyRange(0,0,1)) 
	{
		AddAttributes();
	}

	range::range(int start, int stop, int step)
		:object(pyRange(start, stop, step))
	{
		AddAttributes();
	}

	range::range(int stop)
		: object(pyRange(0, stop, 1))
	{
		AddAttributes();
	}

}
