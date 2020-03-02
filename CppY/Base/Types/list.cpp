#include "list.h"
#include "FunDefs.h"
#include "Comparisons.h"

namespace py
{
	namespace py_list
	{
		FUN_DEF(count);
		PARAM(self, );
		PARAM(whatToCount, );
		PARAM(start, None);
		PARAM(end, None);
		auto meAsList = reinterpret_cast<pyList*>(self._ptr.get());
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
		auto meAsList = reinterpret_cast<pyList*>(self._ptr.get());
		meAsList->_impl.clear();
		END_FUN(clear);

		// '', 'copy', '', '', '-index-', '-insert-', '-pop-', '-remove-', 'reverse', '-sort-'
		FUN_DEF(sort);
		PARAM(self, );
		auto meAsList = reinterpret_cast<pyList*>(self._ptr.get());
		std::sort(meAsList->_impl.begin(), meAsList->_impl.end(),
			[](pyObjPtr const& a, pyObjPtr const& b) { return (*a) < (*b); });

		END_FUN(sort);

		FUN_DEF(pop);
		PARAM(self, );
		PARAM(indexObj, -1);
		auto meAsList = reinterpret_cast<pyList*>(self._ptr.get());
		int index = indexObj;
		return meAsList->pop(index);
		END_FUN(pop);

		FUN_DEF(insert);
		PARAM(self, );
		PARAM(indexObj, );
		PARAM(obj, );
		auto meAsList = reinterpret_cast<pyList*>(self._ptr.get());
		meAsList->insert(indexObj, obj);
		END_FUN(insert);

		FUN_DEF(remove);
		PARAM(self, );
		PARAM(x, );
		auto meAsList = reinterpret_cast<pyList*>(self._ptr.get());
		meAsList->remove(x);
		END_FUN(remove);

		FUN_DEF(find);
		PARAM(self, );
		PARAM(x, );
		PARAM(i, None);
		PARAM(j, None);
		auto meAsList = reinterpret_cast<pyList*>(self._ptr.get());
		if (is_ofType(i, None))
			i = 0;
		if (is_ofType(j, None))
			j = meAsList->_impl.size();
		return meAsList->index(x, i, j,false);
		END_FUN(find);

		FUN_DEF(index);
		PARAM(self, );
		PARAM(x, );
		PARAM(i, None);
		PARAM(j, None);
		auto meAsList = reinterpret_cast<pyList*>(self._ptr.get());
		if (is_ofType(i, None))
			i = 0;
		if (is_ofType(j, None))
			j = meAsList->_impl.size();
		return meAsList->index(x, i, j,true);
		END_FUN(index);

		FUN_DEF(reverse);
		PARAM(self, );
		auto meAsList = reinterpret_cast<pyList*>(self._ptr.get());
		meAsList->reverse();
		END_FUN(reverse);

		FUN_DEF(copy);
		PARAM(self, );
		auto meAsList = reinterpret_cast<pyList*>(self._ptr.get());
		return meAsList->copy();
		END_FUN(copy);

		FUN_DEF(__len__);
		PARAM(self, );
		auto meAsList = reinterpret_cast<pyList*>(self._ptr.get());
		return meAsList->_impl.size();
		END_FUN(__len__);
	}


	object dict(std::vector<std::vector<object>> const& v)
	{
		pyDict result;
		for (auto kvp : v)
		{
			THROW_UNLESS(kvp.size() == 2, "Key Value Pair has more than two values !");
			result._impl[kvp[0]] = kvp[1];
		}
		return result;
	}

	object str(object const& v)
	{
		return v.operator pyStr();
	}
};

namespace py
{
	void list::AddAttributes()
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
	}

	list::list() : object(pyList())
	{
		AddAttributes();
	}

	list::list(std::vector<object> const& v)
		: object(pyList(std::vector<pyObjPtr>(v.begin(), v.end())))
	{
		AddAttributes();
	}

}
