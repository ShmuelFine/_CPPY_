#include "FunDefs.h"
#include "object.h"

namespace py
{
	ARGS::ARGS(object const& key, object const& value)
		:_(new pyDefaultDict())
	{
		IsNamed = true;
		ArgCounter = 0;
		(*_)[key] = value;
	}

	ARGS::ARGS(object const& value)
		:_(new pyDefaultDict())
	{
		IsNamed = false;
		ArgCounter = 0;
		(*_)[pyInt(ArgCounter++).Clone()] = value;
	}

	ARGS & ARGS::A(object const& key, object const& value)
	{
		IsNamed = true;
		(*_)[key] = value;
		return *this;
	}

	ARGS & ARGS::A(object const& value)
	{
		THROW_IF(IsNamed, "You can't pass un-named argument after passing several named ones");
		(*_)[pyInt(ArgCounter++).Clone()] = value;
		return *this;
	}

	// Shallow copy:
	ARGS::operator object()
	{
		return object(_);
	}
	
}