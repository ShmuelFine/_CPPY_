#pragma once

#include "object.h"

namespace py
{
	bool is_same(object const& one, object const& other);
	bool is_not(object const& one, object const& other);
	bool is_ofType(object const& one, object const& other);
	
	bool is_in(object const& one, object const& other);
	bool not_in(object const& one, object const& other);

	template<typename T>
	T sign(T const& numeric)
	{
		return numeric < 0 ? (T)-1 : (T)1;
	}

}