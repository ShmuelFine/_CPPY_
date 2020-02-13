#include "Built_In_Functions.h"
#include "FunDefs.h"

namespace py
{
	object abs(object x)
	{
		return MEM_FUN(x, __abs__));
	}

	object all(object iterable)
	{
		for (auto elem : iterable)
			if (!elem)
				return false;
		
		return true;
	}

	object len(object iterable)
	{
		return MEM_FUN(iterable, __len__));
	}


}
