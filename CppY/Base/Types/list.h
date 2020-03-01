#pragma once

#include "object.h"

namespace py
{
	namespace py_list
	{
		extern object count;
		extern object index;
		extern object find;
		//extern object list;
	}
	class list : public object
	{
	public:
		list();
		list(std::vector<object> const& v);
		list(std::vector<std::string> const& v);
		list(std::initializer_list<std::string> const& v);

	protected:
		void AddAttributes();
	};


}