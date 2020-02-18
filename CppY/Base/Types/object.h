#pragma once

#include "PyObj.h"

#include "pyNums.h"
#include "pyStr.h"
#include "pyNone.h"
#include "pyList.h"
#include "pyFunc.h"
#include "pyDict.h"
#include "PyBytes.h"
#include <memory>
#include <map>
#include "booleanOperators.h"

namespace py
{
#define CHECK_PTR THROW_UNLESS(get(), "Can't invoke null object")

	class object
	{
	public:
		pyObjPtr _tmp_impl; // needed in the cases that object is not constructed from reference.
		pyObjPtr& _ptr;


	public:
		object()						: _ptr(_tmp_impl)	{ _ptr = nullptr; }
		object(pyObjPtr const& other)	: _ptr(_tmp_impl)	{ _ptr = other; }
		object(pyObjPtr& other)			: _ptr(other)		{ _tmp_impl = nullptr; }
		object(pyObj const& other)		: _ptr(_tmp_impl)	{ _ptr = other.Clone(); }

		object(int i)					: _ptr(_tmp_impl){_ptr = pyObjPtr(new pyInt(i))		;}
		object(float i)					: _ptr(_tmp_impl){_ptr = pyObjPtr(new Float(i))	;}
		object(double i)				: _ptr(_tmp_impl){_ptr = pyObjPtr(new Double(i))	;}
		object(std::string const& i)	: _ptr(_tmp_impl){_ptr = pyObjPtr(new pyStr(i))	;}

		object(const char * i)			: _ptr(_tmp_impl){_ptr = pyObjPtr(new pyStr(i))	;}
		object(size_t sz)				: _ptr(_tmp_impl){_ptr = pyObjPtr(new pyInt((int)sz))	;}
		object(uint8_t i) : _ptr(_tmp_impl) { _ptr = pyObjPtr(new pyByte(i)); }

		object(object const& other)		: _ptr(_tmp_impl) { _ptr = other._ptr; }

		pyObj* get() const				{ return _ptr.get(); }
		operator pyObjPtr() const		{ return _ptr; }

		///////////// virtuals forwarding: /////////////////////////////////////////
		operator std::string() const	 { CHECK_PTR; return get()->operator std::string(); };
		operator double() const			 { CHECK_PTR; return get()->operator double(); };
		operator float() const			 { CHECK_PTR; return get()->operator float(); };
		operator int() const 			 { CHECK_PTR; return get()->operator int(); };
		operator bool() const			 { CHECK_PTR; return get()->operator bool(); };
		//operator int8_t() const			 { CHECK_PTR; return get()->operator int8_t(); };
	
	public:

		class ObjectIterator
		{
		protected:
			pyObjIterator _state;

		public:
			ObjectIterator(pyObjIterator state);
			ObjectIterator& operator ++();
			bool operator != (ObjectIterator const& other);
			bool operator == (ObjectIterator const& other);
			object operator *();
		};
		ObjectIterator begin() const	{ CHECK_PTR; return get()->begin(); }
		ObjectIterator end() const		{ CHECK_PTR; return get()->end(); }



		template <typename T> object operator [](T const& key)
		{
			CHECK_PTR;
			return get()->operator[](object(key));
		}

		object operator()(object params) const;
		std::string Type() const;
		object Clone() const;
		

		bool operator <(object const& other) const;
		object operator ++(int x);
		object operator ++();
		object & operator += (object const& other);
		bool operator == (object const& other) const;
		bool operator != (object const& other) const;
		virtual object& operator = (object const& other) 
		{
			_ptr = other._ptr;
			return *this;
		}

		///////// Additional conversions ////////////////////////////////////////
		operator Double() const { CHECK_PTR;  return get()->operator double(); };
		operator Float() const	{ CHECK_PTR;  return get()->operator float(); };
		operator pyInt() const	{ CHECK_PTR;  return get()->operator int(); };
		operator Bool() const	{ CHECK_PTR;  return get() != nullptr && get()->operator bool(); }
		operator pyStr() const	{ CHECK_PTR;  return get()->operator std::string(); }
		operator size_t() const { CHECK_PTR;  return get()->operator int(); }
		operator pyByte() const { CHECK_PTR;  return get()->operator uint8_t(); }

		// Using templates for supporting all POD types: ////////////////////////
		template <typename T> bool operator == (T const& other) const { return operator ==(object(other)); }
		template <typename T> bool operator != (T const& other) const { return operator !=(object(other)); }
		template <typename T> object operator +(T const& other) const	{CHECK_PTR;  return get()->operator +(*object(other).get()); }
		template <typename T> object operator -(T const& other) const	{CHECK_PTR;  return get()->operator -(*object(other).get()); }
		template <typename T> object operator *(T const& other) const	{CHECK_PTR;  return get()->operator *(*object(other).get()); }
		template <typename T> object operator /(T const& other) const	{CHECK_PTR;  return get()->operator /(*object(other).get()); }
		template <typename T> bool operator < (T const& other) const	{CHECK_PTR;  return get()->operator < (*object(other).get());}

		
		
		///////////// Generic attributes: ////////////////////////
		void setattr(std::string const& what, object toWhat)
		{
			_ptr.attributes[what] = toWhat;
		}
		
		bool hasattr(std::string const& what)
		{
			return _ptr.attributes.count(what) > 0;
		}

		object getattr(std::string const& what, object defaultValue = pyNone())
		{
			if (_ptr.attributes.count(what))
				return _ptr.attributes[what];
			else
				return defaultValue;
		}

		// direct access / definition:
		object _attr_(std::string const& key)
		{
			return _ptr.attributes[key];
		}

	};

#define attr(name) _attr_(#name)
	
	
	// For pretty syntax coloring, using two step definition:
	static const py::object _False = py::object(Bool(false));
	static const py::object _True = py::object(Bool(true));
	static const py::object _None = py::object(py::pyNone());
	
#define False _False
#define True _True
#define None _None

}
