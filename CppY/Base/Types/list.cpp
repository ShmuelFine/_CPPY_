#include "list.h"
#include "FunDefs.h"
#include "Comparisons.h"
#include "pyType.h"

namespace py
{
	namespace py_list
	{
		FUN_DEF(count);
		PARAM(self, );
		PARAM(whatToCount, );
		PARAM(start, None);
		PARAM(end, None);
		auto meAsList = dynamic_cast<pyList*>(self.get());
		if (is_ofType(start, None))
			start = 0;
		if (is_ofType(end, None))
			end = meAsList->_impl.size();
		return meAsList->count(whatToCount, start, end);
		END_FUN(count);


		FUN_DEF(extend);
		PARAM(self, );
		PARAM(iterable, );
		std::vector<pyObjPtr> toAdd;
		for (auto elem : iterable) {
			toAdd.push_back(elem);
		}
		self += object(pyList(toAdd));

		END_FUN(extend);

		FUN_DEF(append);
		PARAM(self, );
		PARAM(toAppend, );
		self += list({ toAppend });
		END_FUN(append);

		FUN_DEF(clear);
		PARAM(self, );
		auto meAsList = dynamic_cast<pyList*>(self.get());
		meAsList->_impl.clear();
		END_FUN(clear);

		// '', 'copy', '', '', '-index-', '-insert-', '-pop-', '-remove-', 'reverse', '-sort-'
		FUN_DEF(sort);
		PARAM(self, );
		auto meAsList = dynamic_cast<pyList*>(self.get());
		std::sort(meAsList->_impl.begin(), meAsList->_impl.end(),
			[](pyObjPtr const& a, pyObjPtr const& b) { return (*a) < (*b); });

		END_FUN(sort);

		FUN_DEF(pop);
		PARAM(self, );
		PARAM(indexObj, -1);
		auto meAsList = dynamic_cast<pyList*>(self.get());
		int index = indexObj;
		return meAsList->pop(index);
		END_FUN(pop);

		FUN_DEF(insert);
		PARAM(self, );
		PARAM(indexObj, );
		PARAM(obj, );
		auto meAsList = dynamic_cast<pyList*>(self.get());
		meAsList->insert(indexObj, obj);
		END_FUN(insert);

		FUN_DEF(remove);
		PARAM(self, );
		PARAM(x, );
		auto meAsList = dynamic_cast<pyList*>(self.get());
		meAsList->remove(x);
		END_FUN(remove);

		FUN_DEF(find);
		PARAM(self, );
		PARAM(x, );
		PARAM(i, None);
		PARAM(j, None);
		auto meAsList = dynamic_cast<pyList*>(self.get());
		if (is_ofType(i, None))
			i = 0;
		if (is_ofType(j, None))
			j = meAsList->_impl.size();
		return meAsList->index(x, i, j, false);
		END_FUN(find);

		FUN_DEF(index);
		PARAM(self, );
		PARAM(x, );
		PARAM(i, None);
		PARAM(j, None);
		auto meAsList = dynamic_cast<pyList*>(self.get());
		if (is_ofType(i, None))
			i = 0;
		if (is_ofType(j, None))
			j = meAsList->_impl.size();
		return meAsList->index(x, i, j, true);
		END_FUN(index);

		FUN_DEF(reverse);
		PARAM(self, );
		auto meAsList = dynamic_cast<pyList*>(self.get());
		meAsList->reverse();
		END_FUN(reverse);

		FUN_DEF(copy);
		PARAM(self, );
		auto meAsList = dynamic_cast<pyList*>(self.get());
		return meAsList->copy();
		END_FUN(copy);

		FUN_DEF(__len__);
		PARAM(self, );
		auto meAsList = dynamic_cast<pyList*>(self.get());
		return meAsList->_impl.size();
		END_FUN(__len__);

		FUN_DEF(__init__);
		PARAM(self, );
		PARAM(iterable, );
		auto meAsList = dynamic_cast<pyList*>(self.get());
		for (auto o : iterable)
		{
			meAsList->_impl.push_back(o);
		}
		END_FUN(__init__);
	}
};

namespace py
{
	listType::listType() : object(pyList())
	{
		attr(count) = py_list::count;
		attr(extend) = py_list::extend;
		attr(append) = py_list::append;
		attr(clear) = py_list::clear;
		attr(sort) = py_list::sort;
		attr(insert) = py_list::insert;
		attr(find) = py_list::find;
		attr(index) = py_list::index;
		attr(pop) = py_list::pop;
		attr(remove) = py_list::remove;
		attr(reverse) = py_list::reverse;
		attr(copy) = py_list::copy;
		attr(__len__) = py_list::__len__;
		attr(__init__) = py_list::__init__;
	}

	object listType::operator()(object params) const
	{
		//auto meAsType = dynamic_cast<pyType*>(get());
		//object result = meAsType->Template->Clone();
		object result = get()->Clone();
		result->attributes = get()->attributes;
		MEM_FUN(result, __init__).A(params));
		return result;
	}

	listType list;
}
