#pragma once
#include <string>
#include <memory>
#include <map>
#include <stdexcept>
#include "ComparisonUtils.h"
#include "booleanOperators.h"

#ifndef __FUNCTION_NAME__
#ifdef WIN32   //WINDOWS
#define __FUNCTION_NAME__   __FUNCTION__  
#else          //*NIX
#define __FUNCTION_NAME__   __func__ 
#endif
#endif

namespace py
{

	class pyObj;
	typedef std::shared_ptr<pyObj> pyObjPtr;
	class pyObjIterator;
};

namespace py
{
	class object;
	// For pretty syntax coloring, using two step definition:
	extern const object _True;
	extern const object _False;
	extern const object _None;

#define False _False
#define True _True
#define None _None
}


namespace py
{
#define CHECK_PTR { if (! get()) throw std::runtime_error("Can't invoke null object"); }


	class object : public pyObjPtr
	{
	public:
		object();
		object(pyObjPtr const& other);
		object(pyObj const& other);
		object(pyObj * other);
		object(int i);
		object(float i);
		object(double i);
		object(std::string const& i);
		object(const char* i);
		object(size_t sz);
		object(uint8_t i);
		//object(object const& other);
		object(std::initializer_list<object> const& v);

		operator std::string() const;
		operator double() const;
		operator float() const;
		operator int() const;
		operator bool() const;
		operator unsigned char() const;

		pyObj& operator *();
		pyObj const & operator *() const;
	public:

		class ObjectIterator
		{
		protected:
			std::shared_ptr<pyObjIterator> _statePtr;

		public:
			ObjectIterator(std::shared_ptr<pyObjIterator> state);
			ObjectIterator& operator ++();
			bool operator != (ObjectIterator const& other);
			bool operator == (ObjectIterator const& other);
			object operator *();
		};
		ObjectIterator begin() const;
		ObjectIterator end() const;
		std::string Type() const;

		template <typename T> object operator [](T const& key)
		{
			CHECK_PTR;
			return get()->operator[](object(key));
		}
		virtual object operator()(object params = None) const;


		bool operator <(object const& other) const;
		object operator ++(int x);
		object operator ++();
		object& operator += (object const& other);
		bool operator == (object const& other) const;
		bool operator != (object const& other) const;

		// Using templates for supporting all POD types: ////////////////////////
		template <typename T> bool operator == (T const& other) const { return operator ==(object(other)); }
		template <typename T> bool operator != (T const& other) const { return operator !=(object(other)); }
		template <typename T> object operator +(T const& other) const { CHECK_PTR;  return get()->operator +(*object(other).get()); }
		template <typename T> object operator -(T const& other) const { CHECK_PTR;  return get()->operator -(*object(other).get()); }
		template <typename T> object operator *(T const& other) const { CHECK_PTR;  return get()->operator *(*object(other).get()); }
		template <typename T> object operator /(T const& other) const { CHECK_PTR;  return get()->operator /(*object(other).get()); }
		template <typename T> bool operator < (T const& other) const { CHECK_PTR;  return get()->operator < (*object(other).get()); }



		///////////// Generic attributes: ////////////////////////
		void setattr(std::string const& what, object toWhat);
		bool hasattr(std::string const& what);
		object getattr(std::string const& what, object defaultValue = None);

		// direct access / definition:
		object& _attr_(std::string const& key);

	};

#define attr(name) _attr_(#name)
}

namespace py
{
	typedef std::map<std::string, object> AttrDict;

	class pyObjIterator
	{
	protected:
		int currIdx;
		pyObj* iterable;

	public:
		pyObjIterator(pyObj* container, int startIdx);
		pyObjIterator& operator ++();
		bool operator != (pyObjIterator const& other);
		bool operator == (pyObjIterator const& other);
		object operator *();
	};

	class pyObj
	{
	public:
		AttrDict attributes;
		
	public:
		virtual operator std::string() const = 0;
		virtual operator double() const = 0;
		virtual operator float() const = 0;
		virtual operator int() const = 0;
		virtual operator bool() const = 0;
		virtual operator uint8_t() const;

		virtual object& operator [](object const& key) = 0;

		virtual object FetchByIdx(int idx) const = 0;
		virtual pyObjIterator begin() const = 0;
		virtual pyObjIterator end() const = 0;

		virtual object operator()(pyObj const & params) const = 0;

		virtual std::string Type() const = 0;

		virtual object Clone() const = 0;

		// for using as key in sets, maps, etc:
		virtual bool operator <(pyObj const& other) const = 0;
		virtual bool operator == (pyObj const& other) const { return !(*this < other || other < *this); }
		virtual bool operator != (pyObj const& other) const { return (*this < other || other < *this); }

		virtual object  operator ++(int) = 0;
		virtual pyObj& operator ++() = 0;
		virtual pyObj& operator += (pyObj const& other) = 0;


		virtual object  operator +(pyObj const& other)  const;
		virtual object  operator -(pyObj const& other)  const;
		virtual object  operator *(pyObj const& other)  const;
		virtual object  operator /(pyObj const& other) const;
	};

}

