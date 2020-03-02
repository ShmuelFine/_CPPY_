#include "Built_In_Functions.h"
#include "FunDefs.h"
#include <iostream>

namespace py
{
	FUN_DEF(abs);
	PARAM(x, );
	return MEM_FUN(x, __abs__));
	END_FUN(abs);

	FUN_DEF(all);
	PARAM(iterable, );
	for (auto elem : iterable)
		if (!elem)
			return False;

	return True;	
	END_FUN(all);

	FUN_DEF(any);
	PARAM(iterable, );
	for (auto element : iterable)
		if (element)
			return True;
	
	return False;
	END_FUN(any);
	
	FUN_DEF(len);
	PARAM(obj, );
	return MEM_FUN(obj, __len__));
	END_FUN(len);

	FUN_DEF(repr);
	PARAM(obj, );
	return MEM_FUN(obj, __repr__));
	END_FUN(repr);

	FUN_DEF(print);
	PARAM(value, )
	PARAM_BY_KEY(sep, " ");
	PARAM_BY_KEY(end, "\n");
	std::cout << (std::string)value;
	while (HAS_MORE_POS_PARAMS())
	{
		PARAM(next, );
		std::cout << (std::string)sep << (std::string)next;
	}
	std::cout << (std::string)end;

	END_FUN(print);

	//FUN_DEF(bin);
	//PARAM(value, );
	//return MEM_FUN(value, bin));
	//END_FUN(bin);
	
}
