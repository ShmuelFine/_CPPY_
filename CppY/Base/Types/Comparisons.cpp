#include "Comparisons.h"
namespace py
{
	bool is_same(object const& one, object const& other) { return one.get() == other.get(); }

	bool is_not(object const& one, object const& other) { return not is_same(one, other); }

	bool is_ofType(object const& one, object const& other) { return one.Type() == other.Type(); }

	bool is_in(object const& one, object const& other)
	{
		for (auto elem : other)
			if (one == elem)
				return true;

		return false;
	}

	bool not_in(object const& one, object const& other)
	{
		return not is_in(one, other);
	}
}