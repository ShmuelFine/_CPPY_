#include "PyObj.h"
#include "pyNone.h"
#include "pyNums.h"
#include "pyExceptions.h"
#include "pyStr.h"
#include "pyList.h"
#include "FunDefs.h"

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

	object pyObjIterator::operator*()
	{
		return (*iterable).FetchByIdx(currIdx)->Clone();
	}


	object  pyObj::operator +(pyObj const& other)  const { NOT_SUPPORTED_PAIR(other); }
	object  pyObj::operator -(pyObj const& other)  const { NOT_SUPPORTED_PAIR(other); }
	object  pyObj::operator *(pyObj const& other)  const { NOT_SUPPORTED_PAIR(other); }
	object  pyObj::operator /(pyObj const& other) const { NOT_SUPPORTED_PAIR(other); }


	pyObj::operator uint8_t() const
	{
		NOT_IMPL;
	}

}

namespace py
{
	object::ObjectIterator::ObjectIterator(std::shared_ptr<pyObjIterator> state)
		:_statePtr(state)
	{
	}

	object::ObjectIterator& object::ObjectIterator::operator++()
	{
		++(*_statePtr);
		return *this;
	}

	bool object::ObjectIterator::operator!=(ObjectIterator const& other)
	{
		return *_statePtr != *other._statePtr;
	}

	bool object::ObjectIterator::operator==(ObjectIterator const& other)
	{
		return *_statePtr == *other._statePtr;
	}

	object object::ObjectIterator::operator*()
	{
		return object(**_statePtr);
	}

	/////////////// operators: /////////////////

	object::object() : pyObjPtr(new pyInt(0)) {}

	//object::object(object const& other) : pyObjPtr(other) {}
	
	object::object(pyObjPtr const& other) : pyObjPtr(other) {}

	object::object(pyObj const& other) : pyObjPtr(other.Clone()) {}

	object::object(pyObj* other) : pyObjPtr(other) {}

	object::object(int i) : pyObjPtr(new pyInt(i)) {}

	object::object(float i) : pyObjPtr(new Float(i)) {}

	object::object(double i) : pyObjPtr(new Double(i)) {}

	object::object(std::string const& i) : pyObjPtr(new pyStr(i)) {}

	object::object(const char* i) : pyObjPtr(new pyStr(i)) {}

	object::object(size_t sz) : pyObjPtr(new pyInt((int)sz)) {}

	object::object(uint8_t i) : pyObjPtr(new pyByte(i)) {}

	object::object(std::initializer_list<object> const& v) : pyObjPtr(new pyList(v)) {}

	///////////// virtuals forwarding: /////////////////////////////////////////

	object::operator std::string() const 
	{
		CHECK_PTR;
		return get()->attributes.at("__str__")(ARGS(*this));
	}

	object::operator double() const { CHECK_PTR; return get()->operator double(); }

	object::operator float() const { CHECK_PTR; return get()->operator float(); }

	object::operator int() const { CHECK_PTR; return get()->operator int(); }

	object::operator bool() const { CHECK_PTR; return get()->operator bool(); }
	
	object::operator unsigned char() const { CHECK_PTR; return get()->operator unsigned char(); }

	pyObj& object::operator *() { return *get(); }
	pyObj const& object::operator *() const { return *get(); }

	object::ObjectIterator object::begin() const { CHECK_PTR; return std::make_shared<pyObjIterator>(get()->begin()); }

	object::ObjectIterator object::end() const { CHECK_PTR; return std::make_shared<pyObjIterator>(get()->end()); }

	object object::operator()(object params) const
	{
		THROW_UNLESS(get(), "Can't invoke null object");
		return get()->operator()(*params.get());
	}

	std::string object::Type() const
	{
		return get()->Type() + "__PTR";
	};

	//object object::Clone() const
	//{
	//	// Deep copy
	//	return object(get()->Clone());
	//}


	bool object::operator<(object const& other) const
	{
		pyObj* mine = this->get();
		pyObj* his = other.get();
		return *mine < *his;
	}
	object object::operator ++(int x) { return get()->operator++(x); };
	object object::operator ++() { return get()->operator++(); };
	object& object::operator += (object const& other) { get()->operator+=(*other.get()); return *this; };
	bool object::operator == (object const& other) const 
	{
		return get()->operator ==(*other.get()); 
	}
	bool object::operator != (object const& other) const { return !operator==(other); }

	///////////// Generic attributes: ////////////////////////

	void object::setattr(std::string const& what, object toWhat)
	{
		get()->attributes[what] = toWhat;
	}

	bool object::hasattr(std::string const& what)
	{
		return get()->attributes.count(what) > 0;
	}

	object object::getattr(std::string const& what, object defaultValue)
	{
		if (get()->attributes.count(what))
			return get()->attributes[what];
		else
			return defaultValue;
	}

	// direct access / definition:

	object& object::_attr_(std::string const& key)
	{
		return get()->attributes[key];
	}

}

namespace py
{
	const py::object _False = py::object(Bool(false));
	const py::object _True = py::object(Bool(true));
	const py::object _None = py::object(py::pyNone());
}