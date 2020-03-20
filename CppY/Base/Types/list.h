#pragma once

#include "object.h"

namespace py
{
	namespace py_list
	{
		extern object count;
		extern object index;
		extern object find;
	}

	class listType : public object
	{
	public:
		listType();
		void INIT();
		virtual object operator()(object params = None) const override;
	};
	
	extern listType list;

}