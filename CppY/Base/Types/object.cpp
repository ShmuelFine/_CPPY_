#include "object.h"

namespace py
{
	object::ObjectIterator::ObjectIterator(pyObjIterator state)
		:_state(state)
	{
	}

	object::ObjectIterator& object::ObjectIterator::operator++()
	{
		++_state;
		return *this;
	}

	bool object::ObjectIterator::operator!=(ObjectIterator const& other)
	{
		return _state != other._state;
	}

	bool object::ObjectIterator::operator==(ObjectIterator const& other)
	{
		return _state == other._state;
	}

	object object::ObjectIterator::operator*()
	{
		return object(*_state);
	}

	/////////////// operators: /////////////////

	object object::operator()(object params) const 
	{
		THROW_UNLESS(get(), "Can't invoke null object");
		return get()->operator()(*params.get()); 
	}
	
	std::string object::Type() const 
	{
		return get()->Type() + "__PTR"; 
	};

	object object::Clone() const
	{
		// Deep copy
		return object(get()->Clone());
	}

	bool object::operator <(object const& other) const
	{
		pyObj* mine = this->get();
		pyObj* his = other.get();
		return *mine < *his;
	}

	object object::operator ++(int x) { return get()->operator++(x); };
	object object::operator ++() { return get()->operator++(); };
	object & object::operator += (object const& other) { get()->operator+=(*other.get()); return *this; };
	bool object::operator == (object const& other) const { return get()->operator ==(*other.get()); }
	bool object::operator != (object const& other) const { return !operator==(other); }



}