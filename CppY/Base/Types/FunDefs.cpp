#include "FunDefs.h"
#include "object.h"

namespace py
{
	ARGS::ARGS(object const& key, object const& value)
	{
		IsNamed = true;
		ArgCounter = 0;
		(*this)._impl[key] = value;
	}

	ARGS::ARGS(object const& value)
 	{
		IsNamed = false;
		ArgCounter = 0;
		(*this)._impl[pyInt(ArgCounter++).Clone()] = value;
	}

	ARGS & ARGS::A(object const& key, object const& value)
	{
		IsNamed = true;
		(*this)._impl[key] = value;
		return *this;
	}

	ARGS & ARGS::A(object const& value)
	{
		THROW_IF(IsNamed, "You can't pass un-named argument after passing several named ones");
		(*this)._impl[pyInt(ArgCounter++).Clone()] = value;
		return *this;
	}

	// Shallow copy:
	ARGS::operator object()
	{
		std::shared_ptr<pyDict> result(new pyDict());
		for (auto const currPair : this->_impl)
		{
			result->_impl[currPair.first] = currPair.second;
		}
		return object(result);
	}
	
}