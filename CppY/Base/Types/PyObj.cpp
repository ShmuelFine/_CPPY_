#include "PyObj.h"
#include "pyNone.h"
#include "pyNums.h"
#include "pyExceptions.h"

namespace py
{

	pyObjIterator::pyObjIterator(pyObj* container, int startIdx)
		:currIdx(startIdx), iterable(container)
	{
	}

	pyObjIterator& pyObjIterator::operator++()
	{
		currIdx++;
		return *this;
	}

	bool pyObjIterator::operator!=(pyObjIterator const& other)
	{
		return !operator == (other);
	}


	bool pyObjIterator::operator==(pyObjIterator const& other)
	{
		return currIdx == other.currIdx
			&&
			iterable == other.iterable;
	}

	pyObjPtr pyObjIterator::operator*()
	{
		return (*iterable).FetchByIdx(currIdx)->Clone();
	}

	pyObjPtr& pyObjPtr::operator=(pyObjPtr const& other)
	{
		THROW_UNLESS(IsAssignable, "Can't assign type " + (get() ? get()->Type() : "<Unknown>"));

		::std::shared_ptr<pyObj>::operator=(other);
		attributes = other.attributes;
		return *this;
	}

	bool pyObjPtr::operator==(pyObjPtr const& other)
	{
		if (!*this && !other)
			return true;
		else if (!*this || !other)
			return false;

		return (**this) == (*other);
	}


	pyObjPtr  pyObj::operator +(pyObj const& other)  const { NOT_SUPPORTED_PAIR(other); }
	pyObjPtr  pyObj::operator -(pyObj const& other)  const { NOT_SUPPORTED_PAIR(other); }
	pyObjPtr  pyObj::operator *(pyObj const& other)  const { NOT_SUPPORTED_PAIR(other); }
	pyObjPtr  pyObj::operator /(pyObj const& other) const { NOT_SUPPORTED_PAIR(other); }


	pyObj::operator uint8_t() const
	{
		NOT_IMPL;
	}

}