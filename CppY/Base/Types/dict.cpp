#include "dict.h"
#include "FunDefs.h"

namespace py
{
	namespace py_list
	{
	}
};

namespace py
{
	void dict::AddAttributes()
	{
	}

	dict::dict() : object(pyList()) 
	{
		AddAttributes();
	}

	dict::dict(std::map<object, object> const& m)
		:object(pyDict(std::map<object, object>(m.begin(), m.end())))
	{
		AddAttributes();
	}

}
