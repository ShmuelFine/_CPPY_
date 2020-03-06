#include "set.h"
#include "FunDefs.h"
#include "pySet.h"

namespace py
{
	namespace py_set
	{

		/*'', '', '', '', '', '',
		'', '', '', '', '',
		'', '', '', '', '', ''*/
		FUN_DEF(add);
		PARAM(self, );
		PARAM(o, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		meAsSet->_impl.insert(o);
		END_FUN(add);

		FUN_DEF(clear);
		PARAM(self, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		meAsSet->_impl.clear();
		END_FUN(clear);

		FUN_DEF(copy);
		PARAM(self, );
		return pySet(self);
		END_FUN(copy);

		FUN_DEF(difference);
		PARAM(self, );
		PARAM(others, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		auto other = dynamic_cast<pySet*>(others._ptr.get());
		return pyObjPtr(meAsSet->difference(*other));
		END_FUN(difference);

		FUN_DEF(discard);
		PARAM(self, );
		PARAM(value, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		meAsSet->_impl.erase(value);
		END_FUN(discard);

		FUN_DEF(difference_update);
		PARAM(self, );
		PARAM(others, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		auto other = dynamic_cast<pySet*>(others._ptr.get());
		meAsSet->difference_update(*other);
		END_FUN(difference_update);

		FUN_DEF(intersection);
		PARAM(self, );
		PARAM(others, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		auto other = dynamic_cast<pySet*>(others._ptr.get());
		return pyObjPtr(meAsSet->intersection(*other));
		END_FUN(intersection);

		FUN_DEF(intersection_update);
		PARAM(self, );
		PARAM(others, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		auto other = dynamic_cast<pySet*>(others._ptr.get());
		meAsSet->intersection_update(*other);
		END_FUN(intersection_update);

		FUN_DEF(issuperset);
		PARAM(self, );
		PARAM(others, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		auto other = dynamic_cast<pySet*>(others._ptr.get());
		return meAsSet->issuperset(*other);
		END_FUN(issuperset);

		FUN_DEF(isdisjoint);
		PARAM(self, );
		PARAM(others, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		auto other = dynamic_cast<pySet*>(others._ptr.get());
		return meAsSet->isdisjoint(*other);
		END_FUN(isdisjoint);

		FUN_DEF(issubset);
		PARAM(self, );
		PARAM(others, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		auto other = dynamic_cast<pySet*>(others._ptr.get());
		return meAsSet->issubset(*other);
		END_FUN(issubset);

		FUN_DEF(pop);
		PARAM(self, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		object o = *(meAsSet->_impl.begin());
		meAsSet->_impl.erase(meAsSet->_impl.begin());
		return o;
		END_FUN(pop);

		FUN_DEF(set_union);
		PARAM(self, );
		PARAM(others, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		auto other = dynamic_cast<pyList*>(others._ptr.get());
		return pyObjPtr(meAsSet->setUnion(*other));
		END_FUN(set_union);

		FUN_DEF(symmetric_difference);
		PARAM(self, );
		PARAM(others, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		auto other = dynamic_cast<pySet*>(others._ptr.get());
		return pyObjPtr(meAsSet->symmetric_difference(*other));
		END_FUN(symmetric_difference);

		FUN_DEF(symmetric_difference_update);
		PARAM(self, );
		PARAM(others, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		auto other = dynamic_cast<pySet*>(others._ptr.get());
		meAsSet->symmetric_difference_update(*other);
		END_FUN(symmetric_difference_update);

		FUN_DEF(remove);
		PARAM(self, );
		PARAM(o, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		meAsSet->remove(o);
		END_FUN(remove);

		FUN_DEF(update);
		PARAM(self, );
		PARAM(others, );
		auto meAsSet = dynamic_cast<pySet*>(self._ptr.get());
		auto other = dynamic_cast<pySet*>(others._ptr.get());
		*meAsSet  = pySet(meAsSet->setUnion(*other));
		END_FUN(update);
	}
};

namespace py
{
	void set::AddAttributes()
	{
		(*this).attr(add) = py_set::add;
		(*this).attr(discard) = py_set::discard;
		(*this).attr(remove) = py_set::remove;
		(*this).attr(update) = py_set::update;
		(*this).attr(clear) = py_set::clear;
		(*this).attr(pop) = py_set::pop;
		(*this).attr(difference) = py_set::difference;
		(*this).attr(difference_update) = py_set::difference_update;
		(*this).attr(intersection) = py_set::intersection;
		(*this).attr(intersection_update) = py_set::intersection_update;
		(*this).attr(issuperset) = py_set::issuperset;
		(*this).attr(isdisjoint) = py_set::isdisjoint;
		(*this).attr(issubset) = py_set::issubset;
		(*this).attr(union) = py_set::set_union;
		(*this).attr(symmetric_difference) = py_set::symmetric_difference;
		(*this).attr(symmetric_difference_update) = py_set::symmetric_difference_update;
	}

	set::set() : object(pySet())
	{
		AddAttributes();
	}

	set::set(std::vector<object> const& s)
		: object(pySet(std::vector<pyObjPtr>(s.begin(), s.end())))
	{
		AddAttributes();
	}

}
